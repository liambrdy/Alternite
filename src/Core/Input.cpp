#include "Core/Input.h"

#include <cstring>

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

Input::InputData Input::s_data;

void Input::Init(GLFWwindow* window_)
{
    memset(&s_data, 0, sizeof(Input::InputData));

    glfwSetWindowUserPointer(window_, &s_data);

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Input::InputData& data = *(Input::InputData*)glfwGetWindowUserPointer(window);

        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            if (!data.keysDown[key])
                data.keysPressed[key] = true;

            data.keysDown[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            data.keysDown[key] = false;
            data.keysPressed[key] = false;
        }
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
    {
        Input::InputData& data = *(Input::InputData*)glfwGetWindowUserPointer(window);

        if (action == GLFW_PRESS)
        {
            data.mouseButtons[button] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            data.mouseButtons[button] = false;
        }
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x, double y)
    {
        Input::InputData& data = *(Input::InputData*)glfwGetWindowUserPointer(window);

        data.mousePos.x = x;
        data.mousePos.y = y;
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        Input::InputData& data = *(Input::InputData*)glfwGetWindowUserPointer(window);

        data.mouseScroll.x = xOffset;
        data.mouseScroll.y = yOffset;
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        Input::InputData& data = *(Input::InputData*)glfwGetWindowUserPointer(window);

        data.windowShouldClose = true;
    });

    glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        Renderer::OnWindowResize(width, height);
        Window::Get()->OnWindowResize(width, height);
    });
}

void Input::Update()
{
    for (bool& key : s_data.keysPressed)
        key = false;

    s_data.mouseScroll = glm::vec2(0.0f);
}

bool Input::IsKeyPressed(uint32_t keycode)
{
    return s_data.keysPressed[keycode];
}

bool Input::IsKeyDown(uint32_t keycode)
{
    return s_data.keysDown[keycode];
}

bool Input::IsMouseButtonPressed(uint32_t mousecode)
{
    return s_data.mouseButtons[mousecode];   
}

glm::vec2 Input::GetMousePosition()
{
    return s_data.mousePos;
}

glm::vec2 Input::GetMouseScroll()
{
    return s_data.mouseScroll;
}

bool Input::WindowShouldClose()
{
    return s_data.windowShouldClose;
}