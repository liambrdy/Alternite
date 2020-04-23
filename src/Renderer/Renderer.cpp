#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Core/Common.h"

#ifndef NDEBUG
static APIENTRY void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            LOG_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:          LOG_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW:             LOG_INFO(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    LOG_TRACE(message); return;
    }

    ASSERT(false, "Unknown severity level!");
}
#endif

void Renderer::Init()
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(DebugCallback, nullptr);
    // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
}

void Renderer::BeginFrame()
{

}

void Renderer::EndFrame()
{

}
