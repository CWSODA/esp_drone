#include <string_view>

enum class VertexShader {
    Standard,
    Color,
};

enum class FragmentShader { Orange, Yellow, Uniform, Color };

constexpr std::string_view toFilePath(VertexShader vertexShader) {
    switch (vertexShader) {
        case VertexShader::Standard:
            return "../src/shaders/vertex_shader.vert";
        case VertexShader::Color:
            return "../src/shaders/vs2.vert";
        default:
            return "";
    }
}

constexpr std::string_view toFilePath(FragmentShader fragShader) {
    switch (fragShader) {
        case FragmentShader::Orange:
            return "../src/shaders/fragOrange.frag";
        case FragmentShader::Yellow:
            return "../src/shaders/fragYellow.frag";
        case FragmentShader::Uniform:
            return "../src/shaders/fragUniform.frag";
        case FragmentShader::Color:
            return "../src/shaders/fs2.frag";
        default:
            return "";
    }
}