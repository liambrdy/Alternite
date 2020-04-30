#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(float left, float right, float bottom, float top);
    virtual ~Camera();

    void SetProjection(float left, float right, float bottom, float top);

    const glm::vec2& GetPosition() const { return m_position; }
    void SetPosition(const glm::vec2& position) { m_position = position; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
private:
    void RecalculateViewMatrix();
private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
    glm::mat4 m_viewProjectionMatrix;

    glm::vec2 m_position;
};