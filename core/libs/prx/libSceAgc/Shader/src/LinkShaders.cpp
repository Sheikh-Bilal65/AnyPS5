#include "prx/libSceAgc/Shader/include/PrimState.hpp"
#include "prx/libSceAgc/Shader/include/LinkShaders.hpp"
#include "prx/libSceAgc/Shader/include/InterpolantMapping.hpp"
#include "SceShaders.hpp"
#include "prx/libSceAgcDriver/Execution/include/ShaderPreparationScope.hpp"
#include "prx/libSceAgcDriver/Execution/include/Driver.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libSceAgc/Shader/include/ShaderConstants.hpp"
#include <array>
#include <algorithm>

extern "C" int APS5_VABI sceAgcLinkShaders(ShaderRegister* context, ShaderRegister* primitive,
    const void* reserved, const Shader* vertex, const Shader* pixel, std::uint32_t primitiveType) {
    using namespace ShaderRegs;
    if (!context || !primitive || reserved || !vertex) return GRAPHICS5_ERROR_INVALID_SHADER_PROGRAM;
    const auto* const vertexShader = vertex;
    const auto* const pixelShader = pixel;
    auto* const outContext = context;
    auto* const outPrimitive = primitive;
    if (vertexShader->type != static_cast<std::uint8_t>(ShaderBinaryType::Gs) ||
        (vertexShader->num_output_semantics && !vertexShader->output_semantics) ||
        (pixelShader && (pixelShader->type != static_cast<std::uint8_t>(ShaderBinaryType::Ps) ||
                         pixelShader->num_input_semantics > 32 ||
                         (pixelShader->num_input_semantics && !pixelShader->input_semantics))))
        return GRAPHICS5_ERROR_INVALID_SHADER_PROGRAM;

    AgcDriver::ShaderPreparationScope transaction({vertexShader, pixelShader});
    std::array<ShaderRegister, 34> contextValues{};
    std::array<ShaderRegister, 3> primitiveValues{};
    sceAgcCreatePrimState(contextValues.data(), primitiveValues.data(), nullptr, vertexShader, primitiveType);
    sceAgcCreateInterpolantMapping_0100(contextValues.data() + 2, vertexShader, pixelShader);
    if (pixelShader != nullptr) {
        const std::array<const Shader*, 2> stages{vertexShader, pixelShader};
        AgcDriverResolveGraphicsStagesAbi_nid_postfix(stages, contextValues, primitiveValues);
    }
    AgcDriverResolveGraphicsAbi_nid_postfix(vertexShader, pixelShader, primitiveType);
    transaction.Commit();
    std::copy(contextValues.begin(), contextValues.end(), outContext);
    std::copy(primitiveValues.begin(), primitiveValues.end(), outPrimitive);
    return 0;
}
