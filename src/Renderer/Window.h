#pragma once

#include <cstdint>
#include <vector>

#include <GLFW/glfw3.h>

enum WindowMode
{
    Windowed,
    Fullscreen
};

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    virtual ~Window();

    void OnUpdate();
    void OnWindowResize(uint32_t width, uint32_t height);

    void SetVSync(bool enabled);
    bool GetVSync() const { return m_data.vsync; }

    void SetWindowMode(const WindowMode& mode, uint32_t width = 0, uint32_t height = 0);
    WindowMode GetWindowMode() const { return m_data.mode; }

    uint32_t GetWidth() const { return m_data.width; }
    uint32_t GetHeight() const { return m_data.height; }

    static Window* Get() { return s_instance; }
private:
    static Window* s_instance;

    GLFWwindow* m_window;
    GLFWmonitor* m_primaryMonitor;
    GLFWvidmode m_baseVideoMode;
    
    struct WindowData
    {
        uint32_t width, height;
        bool vsync;
        WindowMode mode;
    };

    WindowData m_data;

    struct WindowedModeParams
    {
        uint32_t width, height;
        int xPos, yPos;
    };

    WindowedModeParams m_oldWindowedParams;
};