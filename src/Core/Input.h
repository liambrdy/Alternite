#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <array>

class Input
{
public:
    static void Init(GLFWwindow* window);

    static void Update();

    static bool IsKeyPressed(uint32_t keycode);
    static bool IsKeyDown(uint32_t keycode);

    static bool IsMouseButtonPressed(uint32_t mousecode);
    static glm::vec2 GetMousePosition();    
    static glm::vec2 GetMouseScroll();

    static bool WindowShouldClose();
private:
    struct InputData
    {
        std::array<bool, GLFW_KEY_LAST> keysPressed;
        std::array<bool, GLFW_KEY_LAST> keysDown;
        std::array<bool, GLFW_MOUSE_BUTTON_LAST> mouseButtons;

        glm::vec2 mousePos;
        glm::vec2 mouseScroll;

        bool windowShouldClose;
    };

    static InputData s_data;
};