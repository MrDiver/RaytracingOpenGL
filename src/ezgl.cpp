#include "ezgl.hpp"
#include <fstream>

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

        spdlog::info("Attr: {} size: {} type: {} stride: {} offset: {}", counter, p.second, p.first, stride,
        offset);

        offset += size;
        counter++;
    }
}

/* Program */

auto read_file(std::string_view path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    if (not stream) {
        throw std::ios_base::failure("file does not exist");
    }
    
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

Program::Program(std::string vertexPath, std::string fragmentPath){
    std::string vertexSource = read_file(vertexPath);
    std::string fragmentSource = read_file(fragmentPath);
    GLint vertShader, fragShader;
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    int  success;
    char infoLog[512];

    const char* tmp = vertexSource.c_str();
    glShaderSource(vertShader, 1, &tmp, NULL);
    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        spdlog::error("VertexShader compilation failed \n{}",infoLog);
        exit(EXIT_FAILURE);
    }

    tmp = fragmentSource.c_str();
    glShaderSource(fragShader, 1, &tmp, NULL);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if(!success)
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
    if(!success) {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        spdlog::error("Linking Program failed \n{}", infoLog);
        exit(EXIT_FAILURE);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);  
}

Program::~Program(){
    glDeleteProgram(this->id);
}

void Program::use(){
    glUseProgram(this->id);
}

} // namespace ez
