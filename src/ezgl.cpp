#include "ezgl.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace ez
{
GLint getGLTypeSize(GLenum type)
{
    switch (type)
    {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        return sizeof(uint8_t);
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
        return sizeof(uint16_t);
    case GL_INT:
    case GL_UNSIGNED_INT:
        // case GL_FIXED:
        return sizeof(uint32_t);
    case GL_FLOAT:
        return sizeof(float);
    case GL_DOUBLE:
        return sizeof(double);
    }
    spdlog::error("Could not find size of type {} for GL Types", type);
    exit(EXIT_FAILURE);
}

/* VertexBuffer */

VertexBuffer::VertexBuffer(bool dynamic)
{
    glGenBuffers(1, &this->id);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

/* VertexArray */
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &this->id);
    this->bind();
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &this->id);
}

void VertexArray::bind()
{
    glBindVertexArray(this->id);
}

void VertexArray::attributes(std::initializer_list<std::pair<GLenum, GLint>> elements)
{
    spdlog::debug("Setting attributes for {}", this->id);
    GLint stride = 0;
    for (auto const &p : elements)
    {
        stride += getGLTypeSize(p.first) * p.second;
    }

    size_t offset = 0;
    GLint counter = 0;
    for (auto const &p : elements)
    {
        GLint size = getGLTypeSize(p.first) * p.second;
        glVertexAttribPointer(counter, p.second, p.first, GL_FALSE, stride, (GLvoid *)offset);
        glEnableVertexAttribArray(counter);

        spdlog::debug("VAO {}: Attr: {} size: {} type: {} stride: {} offset: {}", this->id, counter, p.second, p.first,
                      stride, offset);

        offset += size;
        counter++;
    }
}

/* Program */

std::string read_file(std::string_view path)
{
    spdlog::debug("Reading file {}", path.data());
    std::ifstream stream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream)
    {
        spdlog::error("File {} does not exist", path);
        exit(EXIT_FAILURE);
    }

    fs::path p(path);
    p = p.remove_filename();

    std::string content;
    std::string line;
    if (stream.is_open())
    {
        while (std::getline(stream, line))
        {
            if (line.starts_with("#include"))
            {
                size_t start = line.find("\"");
                size_t end = line.find("\"", start + 1);
                std::string sub(line.begin() + start + 1, line.begin() + end);
                spdlog::debug("Found include with name {} attempting read", p.string() + sub);
                std::string subfile = read_file(p.string() + sub);
                content += "// === START INCLUDE " + sub + " ===\n";
                content += subfile;
                content += "// === END INCLUDE " + sub + " ===\n";
            }
            else
            {
                content += line + "\n";
            }
        }
        stream.close();
    }
    else
    {
        spdlog::error("Unable to open file {}", path.data());
    }
    spdlog::debug("{}", content);

    return content;
}

Program::Program(std::string const &vertexPath, std::string const &fragmentPath)
{
    std::string vertexSource = read_file(vertexPath);
    std::string fragmentSource = read_file(fragmentPath);
    GLint vertShader, fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];

    const char *tmp = vertexSource.c_str();
    glShaderSource(vertShader, 1, &tmp, NULL);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        spdlog::error("VertexShader compilation failed \n{}", infoLog);
        exit(EXIT_FAILURE);
    }

    tmp = fragmentSource.c_str();
    glShaderSource(fragShader, 1, &tmp, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        spdlog::error("FragmentShader compilation failed \n{}", infoLog);
        exit(EXIT_FAILURE);
    }

    this->id = glCreateProgram();
    glAttachShader(this->id, vertShader);
    glAttachShader(this->id, fragShader);
    glLinkProgram(this->id);

    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        spdlog::error("Linking Program failed \n{}", infoLog);
        exit(EXIT_FAILURE);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

Program::~Program()
{
    glDeleteProgram(this->id);
}

void Program::use()
{
    glUseProgram(this->id);
}

void Program::setInt(std::string const &name, uint32_t value)
{
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}
void Program::setFloat(std::string const &name, float value)
{
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}
void Program::setVec2(std::string const &name, glm::vec2 const &value)
{
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y);
}
void Program::setVec2(std::string const &name, float v1, float v2)
{
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), v1, v2);
}
void Program::setVec3(std::string const &name, glm::vec3 const &value)
{
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z);
}
void Program::setVec3(std::string const &name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3);
}
void Program::setVec4(std::string const &name, glm::vec4 const &value)
{
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), value.x, value.y, value.z, value.w);
}
void Program::setVec4(std::string const &name, float v1, float v2, float v3, float v4)
{
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), v1, v2, v3, v4);
}

SSBO::SSBO()
{
    glGenBuffers(1, &this->id);
}
SSBO::~SSBO()
{
    glDeleteBuffers(1, &this->id);
}
void SSBO::bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
}
void SSBO::layout(GLint binding)
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, this->id);
}
} // namespace ez
