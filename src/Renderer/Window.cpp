#include "Renderer/Window.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include <glad/glad.h>

Window::Window(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    int res = glfwInit();
    ASSERT(res, "Failed to initialize GLFW");

    glfwSetErrorCallback([](int error, const char* description)
    {
        LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, "Alternite", nullptr, nullptr);
    ASSERT(m_window, "Failed to create GLFW window!");

    glfwMakeContextCurrent(m_window);

    res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(res, "Failed to initialize glad!");

    Input::Init(m_window);

    SetVSync(true);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::OnUpdate()
{
    Input::Update();
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_vsync = enabled;
}
