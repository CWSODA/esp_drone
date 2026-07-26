#include <vector>
#include <unordered_map>
#include <string>

enum class ShapeType { TRIFORCE_I, TRIFORCE, LEFT_TRI, RIGHT_TRI, SINGLE_TRI };

struct Shape {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

std::unordered_map<ShapeType, Shape> shapeMap = {
    {ShapeType::TRIFORCE_I,
     {{
          -0.5f, -0.5f, 0.0f,  // bottom row
          0.0f, -0.5f, 0.0f,   //
          0.5f, -0.5f, 0.0f,   //
          -0.25f, 0.0f, 0.0f,  // middle row
          0.25f, 0.0f, 0.0f,   //
          0.0f, 0.5f, 0.0f,    // top vertex
      },
      {
          0, 1, 3,  // bottom left tri
          1, 2, 4,  // bottom right tri
          3, 4, 5,  // top tri
      }}},

    {ShapeType::TRIFORCE,
     {{
          -0.5f,  -0.5f, 0.0f,  // bottom left tri
          0.0f,   -0.5f, 0.0f,  //
          -0.25f, 0.0f,  0.0f,  //
          0.0f,   -0.5f, 0.0f,  // bottom right tri
          0.5f,   -0.5f, 0.0f,  //
          0.25f,  0.0f,  0.0f,  //
          -0.25f, 0.0f,  0.0f,  // top tri
          0.25f,  0.0f,  0.0f,  //
          0.0f,   0.5f,  0.0f,  //
      },
      {}}},

    {ShapeType::LEFT_TRI,
     {{
          -0.5f, -0.5f, 0.0f,  //
          0.0f, -0.5f, 0.0f,   //
          -0.25f, 0.5f, 0.0f,  //
      },
      {}}},

    {ShapeType::RIGHT_TRI,
     {{
          0.5f, -0.5f, 0.0f,  //
          0.0f, -0.5f, 0.0f,  //
          0.25f, 0.5f, 0.0f,  //
      },
      {}}},

    {ShapeType::SINGLE_TRI,
     {{
          0.5f, -0.5f, 0.0f,   //
          -0.5f, -0.5f, 0.0f,  //
          0.0f, 0.5f, 0.0f,    //
      },
      {}}},
};

void assign_shape(ShapeType target_shape, float** vertices,
                  size_t* vertices_size, unsigned int** indices,
                  size_t* indices_size) {
    Shape& shape = shapeMap[target_shape];

    *vertices = shape.vertices.data();
    *vertices_size = shape.vertices.size() * sizeof(float);
    *indices = shape.indices.empty() ? nullptr : shape.indices.data();
    *indices_size = shape.indices.size() * sizeof(unsigned int);
}

std::string read_shader(const std::string_view filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open shader file: " << filename << "\n";
        return NULL;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    return buffer.str();
}

unsigned int load_shaders(VertexShader vShader, FragmentShader fShader) {
    // read shaders from files
    const std::string vertexShaderInput = read_shader(toFilePath(vShader));
    const std::string fragmentShaderInput = read_shader(toFilePath(fShader));

    const char* vertexShaderSource = vertexShaderInput.c_str();
    const char* fragmentShaderSource = fragmentShaderInput.c_str();

    int success;
    char infoLog[512];

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}