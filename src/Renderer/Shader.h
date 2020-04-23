#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glad/glad.h>

class Shader
{
public:
    Shader(const std::string& filepath);
    virtual ~Shader();

    void Bind();
    void Unbind();

    void SetFloat(const std::string& name, float value);
    void SetFloat2(const std::string& name, const glm::vec2& value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetInt(const std::string& name, int value);
    void SetIntArray(const std::string& name, int* value, uint32_t count);
    void SetMat3(const std::string& name, const glm::mat3& value);
    void SetMat4(const std::string& name, const glm::mat4& value);
private:
    std::string ReadFile(const std::string& filename);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& sources);

    int32_t GetUniformLocation(const std::string& name);
private:
    uint32_t m_rendererID;

    std::unordered_map<std::string, int32_t> m_uniformLocations;
};