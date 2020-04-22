#include "Renderer/Window.h"

#include "Core/Common.h"

#include <glad/glad.h>

Window::Window(uint32_t width, uint32_t height)
    : m_windowData(width, height)
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

    m_window = glfwCreateWindow(m_windowData.width, m_windowData.height, "Alternite", nullptr, nullptr);
    ASSERT(m_window, "Failed to create GLFW window!");

    glfwMakeContextCurrent(m_window);

    res = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(res, "Failed to initialize glad!");

    glfwSetWindowUserPointer(m_window, &m_windowData);

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
            e.type = KeyPressed;
        else if (action == GLFW_RELEASE)
            e.type = KeyReleased;

        e.keyboard.key = key;
        e.keyboard.scancode = scancode;
        e.keyboard.mods = mods;
        data->events.push_back(e);
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        if (action == GLFW_PRESS)
            e.type = MousePressed;
        else if (action == GLFW_RELEASE)
            e.type = MouseReleased;
        e.mouse.button = button;
        e.mouse.mods = mods;
        data->events.push_back(e);
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        e.type = MouseMoved;
        e.pos.x = xPos;
        e.pos.y = yPos;
        data->events.push_back(e);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        e.type = MouseScrolled;
        e.scroll.x = xOffset;
        e.scroll.y = yOffset;
        data->events.push_back(e);
    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        e.type = WindowResized;
        e.size.width = width;
        e.size.height = height;
        data->events.push_back(e);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
    {
        Window::WindowData* data = static_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        Event e;
        e.type = WindowClosed;
        data->events.push_back(e);
    });

    SetVSync(true);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::HasEventReady(Event& event)
{   
    if (m_windowData.events.empty())
        return false;

    event = m_windowData.events.back();
    m_windowData.events.pop_back();
    return true;
}

void Window::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_windowData.vsync = enabled;
}
