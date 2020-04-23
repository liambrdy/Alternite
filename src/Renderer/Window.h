#pragma once

#include <cstdint>
#include <vector>

#include <GLFW/glfw3.h>

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    virtual ~Window();

    void OnUpdate();

    void SetVSync(bool enabled);
    bool GetVSync() const { return m_vsync; }

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
private:
    GLFWwindow* m_window;
    bool m_vsync = true;
    uint32_t m_width, m_height;
};