#pragma once

#include <cstdint>
#include <vector>

#include <GLFW/glfw3.h>

#include "Core/Events.h"

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    virtual ~Window();

    void OnUpdate();

    bool HasEventReady(Event& event);

    void SetVSync(bool enabled);
    bool GetVSync() const { return m_windowData.vsync; }

    uint32_t GetWidth() const { return m_windowData.width; }
    uint32_t GetHeight() const { return m_windowData.height; }
private:
    GLFWwindow* m_window;

    struct WindowData
    {
        uint32_t width, height;
        bool vsync;
        std::vector<Event> events;

        WindowData(uint32_t width_, uint32_t height_)
        {
            width = width_;
            height = height_;
            vsync = true;
        }
    };

    WindowData m_windowData;
};