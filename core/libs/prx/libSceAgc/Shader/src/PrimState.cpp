#include "prx/libSceAgc/Shader/include/PrimState.hpp"

#include <cstdio>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <prx/libc/include/General.hpp>

#include "SceShaders.hpp"
#include "prx/libSceAgcDriver/Execution/include/Driver.hpp"
#include "prx/libSceAgcDriver/Execution/include/ShaderPreparationScope.hpp"
#include "prx/libSceAgc/Shader/include/ShaderUtils.hpp"
#include "prx/libSceAgc/Shader/include/ShaderConstants.hpp"

namespace {

void RequireSpecialOffset(const ShaderRegister& reg, std::uint32_t expected, const char* name) {
    if (reg.offset != expected) {
        char message[160];
        std::snprintf(message, sizeof(message), "sceAgcCreatePrimState: %s special register at offset 0x%x, expected 0x%x", name, static_cast<unsigned>(reg.offset), static_cast<unsigned>(expected));
        throw std::runtime_error(message);
    }
}

}

extern "C" {

int APS5_VABI sceAgcCreatePrimState(ShaderRegister* cx_regs, ShaderRegister* uc_regs, const Shader* hs, const Shader* gs, std::uint32_t prim_type) {
    if (cx_regs == nullptr && uc_regs == nullptr) {
        return 0;
    }
    if (gs == nullptr) {
        throw std::runtime_error(std::string(__func__) + ": gs is null");
    }

    const auto* const gsShader = gs;
    const auto* const hsShader = hs;
    auto* const outCx = cx_regs;
    auto* const outUc = uc_regs;

    if (gsShader->type != static_cast<std::uint8_t>(ShaderRegs::ShaderBinaryType::Gs) || gsShader->specials == nullptr || (hsShader != nullptr && (hsShader->type != static_cast<std::uint8_t>(ShaderRegs::ShaderBinaryType::Hs) || hsShader->specials == nullptr))) {
        throw std::runtime_error(std::string(__func__) + ": invalid shader type or missing special registers");
    }
    (void)GraphicsPrimTypeToGsOut(prim_type);
    RequireSpecialOffset(gsShader->specials->vgt_shader_stages_en, ShaderRegs::VGT_SHADER_STAGES_EN, "VGT_SHADER_STAGES_EN");
    if (hsShader != nullptr) RequireSpecialOffset(hsShader->specials->vgt_shader_stages_en, ShaderRegs::VGT_SHADER_STAGES_EN, "VGT_SHADER_STAGES_EN");
    RequireSpecialOffset(gsShader->specials->ge_cntl, ShaderRegs::GE_CNTL, "GE_CNTL");
    std::array<ShaderRegister, 2> contextValues{gsShader->specials->vgt_shader_stages_en, gsShader->specials->vgt_gs_out_prim_type};
    if (hsShader != nullptr) {
        contextValues[0].value |= hsShader->specials->vgt_shader_stages_en.value;
    }
    if ((contextValues[0].value & ShaderRegs::VGT_SHADER_STAGES_GS_BIT) != 0) {
        RequireSpecialOffset(gsShader->specials->vgt_gs_out_prim_type, ShaderRegs::VGT_GS_OUT_PRIM_TYPE, "VGT_GS_OUT_PRIM_TYPE");
    } else if (hsShader != nullptr) {
        RequireSpecialOffset(hsShader->specials->vgt_gs_out_prim_type, ShaderRegs::VGT_GS_OUT_PRIM_TYPE, "VGT_GS_OUT_PRIM_TYPE");
        contextValues[1] = hsShader->specials->vgt_gs_out_prim_type;
    } else {
        contextValues[1] = {ShaderRegs::VGT_GS_OUT_PRIM_TYPE, GraphicsPrimTypeToGsOut(prim_type)};
    }
    std::array<ShaderRegister, 3> primitiveValues{gsShader->specials->ge_cntl, hsShader != nullptr ? hsShader->specials->ge_user_vgpr_en : gsShader->specials->ge_user_vgpr_en, ShaderRegister{ShaderRegs::VGT_PRIMITIVE_TYPE, prim_type}};
    const std::array<const Shader*, 2> stages{hsShader, gsShader};
    const bool patch = prim_type == static_cast<std::uint32_t>(ShaderRegs::PrimitiveType::Patch);
    if (patch == ((contextValues[0].value & ShaderRegs::VGT_SHADER_STAGES_HS_BIT) != 0)) {
        AgcDriver::ShaderPreparationScope transaction({hsShader, gsShader});
        AgcDriverResolveGraphicsStagesAbi_nid_postfix(std::span(stages).subspan(hsShader == nullptr ? 1u : 0u), contextValues, primitiveValues);
        transaction.Commit();
    }
    if (outCx != nullptr) std::copy(contextValues.begin(), contextValues.end(), outCx);
    if (outUc != nullptr) std::copy(primitiveValues.begin(), primitiveValues.end(), outUc);
    return 0;
}

int APS5_VABI sceAgcUpdatePrimState(ShaderRegister* cx_regs, ShaderRegister* uc_regs, std::uint32_t prim_type) {
    auto* const outCx = cx_regs;
    auto* const outUc = uc_regs;
    if (outCx != nullptr && (outCx[0].value & (ShaderRegs::VGT_SHADER_STAGES_GS_BIT | ShaderRegs::VGT_SHADER_STAGES_HS_BIT)) == 0) {
        outCx[1].value &= ~0x7u;
        outCx[1].value |= GraphicsPrimTypeToGsOut(prim_type);
    }

    if (outUc != nullptr) {
        outUc[2].value &= ~0x1Fu;
        outUc[2].value |= prim_type;
    }

    return 0;
}

}
