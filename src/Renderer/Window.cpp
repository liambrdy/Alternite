#include "Renderer/Window.h"

#include "Core/Common.h"
#include "Core/Input.h"

#include "Renderer/Renderer.h"

#include <glad/glad.h>

Window* Window::s_instance;

Window::Window(uint32_t width, uint32_t height)
{
    ASSERT(!s_instance, "A window has already been created!");
    s_instance = this;

    m_data.width = width;
    m_data.height = height;
    m_data.mode = WindowMode::Windowed;

    int res = glfwInit();
    ASSERT(res, "Failed to initialize GLFW");

    glfwSetErrorCallback([](int error, const char* description)
    {
        LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    });

    m_primaryMonitor = glfwGetPrimaryMonitor();
    
    m_baseVideoMode = *(glfwGetVideoMode(m_primaryMonitor));
    LOG_INFO("Video Mode: {0}x{1}@{2}Hz (r{3}g{4}b{5})", m_baseVideoMode.width, m_baseVideoMode.height, m_baseVideoMode.refreshRate, m_baseVideoMode.redBits, m_baseVideoMode.greenBits, m_baseVideoMode.blueBits);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, "Alternite", nullptr, nullptr);
    ASSERT(m_window, "Failed to create GLFW window!");

    glfwMakeContextCurrent(m_window);

    m_oldWindowedParams.width = width;
    m_oldWindowedParams.height = height;
    glfwGetWindowPos(m_window, &m_oldWindowedParams.xPos, &m_oldWindowedParams.yPos);

    res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(res, "Failed to initialize glad!");

    SetWindowMode(WindowMode::Fullscreen, 1280, 720);

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

void Window::OnWindowResize(uint32_t width, uint32_t height)
{
    m_data.width = width;
    m_data.height = height;
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_data.vsync = enabled;
}

void Window::SetWindowMode(const WindowMode& mode, uint32_t width, uint32_t height)
{
    if (mode == m_data.mode)
        return;

    if (m_data.mode == WindowMode::Windowed)
    {
        m_oldWindowedParams.width = m_data.width;
        m_oldWindowedParams.height = m_data.height;
        glfwGetWindowPos(m_window, &m_oldWindowedParams.xPos, &m_oldWindowedParams.yPos);
    }

    GLFWmonitor* monitor = nullptr;

    if (mode == WindowMode::Windowed && (width == 0 || height == 0))
    {
        width = m_oldWindowedParams.width;
        height = m_oldWindowedParams.height;
    }
    else if (mode == WindowMode::Fullscreen)
    {
        width = m_baseVideoMode.width;
        height = m_baseVideoMode.height;
        monitor = m_primaryMonitor;
    }

    m_data.width = width;
    m_data.height = height;

    glViewport(0, 0, width, height);

    LOG_INFO("Changing window mode from {0} to {1}: [{2}, {3}]", m_data.mode == WindowMode::Windowed ? "Windowed" : "Fullscreen", mode == WindowMode::Windowed ? "Windowed" : "Fullscreen", width, height);

    m_data.mode = mode;

    glfwSetWindowMonitor(m_window, monitor, m_oldWindowedParams.xPos, m_oldWindowedParams.yPos, width, height, m_baseVideoMode.refreshRate);
}
