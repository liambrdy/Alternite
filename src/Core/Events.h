#pragma once

#include <cstdint>

enum EventType
{
    KeyPressed = 0,
    KeyReleased,
    MousePressed,
    MouseReleased,
    MouseMoved,
    MouseScrolled,
    WindowResized,
    WindowClosed
};

struct Event
{
    EventType type;
    union
    {
        struct 
        {
            double x;
            double y;
        } pos;

        struct 
        {
            int width;
            int height;
        } size;

        struct 
        {
            double x;
            double y;
        } scroll;

        struct 
        {
            int key;
            int scancode;
            int mods;
        } keyboard;

        struct 
        {
            int button;
            int mods;
        } mouse;
    };
};