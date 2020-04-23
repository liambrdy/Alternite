#include "Renderer/Shader.h"

#include "Core/Common.h"

#include <glad/glad.h>

#include <fstream>

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    else if (type == "fragment")
        return GL_FRAGMENT_SHADER;

    ASSERT(false, "Unknown shader type: {0}!", type);
    return 0;
}

Shader::Shader(const std::string& filepath)
{
    std::string file = ReadFile(filepath);
    auto shaderSources = PreProcess(file);
    Compile(shaderSources);
}

Shader::~Shader()
{
    glDeleteProgram(m_rendererID);
}

void Shader::Bind()
{
    glUseProgram(m_rendererID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::SetFloat(const std::string& name, float value)
{
    
}

void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
{
    
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    
}

void Shader::SetInt(const std::string& name, int value)
{
    
}

void Shader::SetIntArray(const std::string& name, int* value)
{
    
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value)
{
    
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value)
{
    
}

std::string Shader::ReadFile(const std::string& filename)
{
    std::string result;
    std::ifstream in(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (in.is_open())
    {
        size_t size = in.tellg();
        if (size != -1)
        {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
            in.close();
        }
        else 
        {
            LOG_ERROR("Could not read file '{0}'", filename);
        }
    }
    else 
    {
        LOG_ERROR("Could not open file '{0}'", filename);
    }

    return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "//type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        ASSERT(eol != std::string::npos, "Syntax Error!");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        ASSERT(nextLinePos != std::string::npos, "Syntex error!");
        pos = source.find(typeToken, nextLinePos);

        shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    m_rendererID = glCreateProgram();

    std::array<GLuint, 2> shaderIDs;
    int glShaderIDIndex = 0;
    for (auto& kv : shaderSources)
    {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            LOG_ERROR("{0}", infoLog.data());
            ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(m_rendererID, shader);
        shaderIDs[glShaderIDIndex++] = shader;
    }

    glLinkProgram(m_rendererID);

    GLint isLinked = 0;
    glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_rendererID);
        
        for (auto id : shaderIDs)
            glDeleteShader(id);

        LOG_ERROR("{0}", infoLog.data());
        ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : shaderIDs)
    {
        glDetachShader(m_rendererID, id);
        glDeleteShader(id);
    }
}


int32_t Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocations.contains(name))
        return m_uniformLocations[name];
    else 
    {
        int32_t location = glGetUniformLocation(m_rendererID, name.c_str());
        ASSERT(location != -1, "Failed to find uniform location: {0}", name);
        m_uniformLocations[name] = location;
        return location;
    }
}