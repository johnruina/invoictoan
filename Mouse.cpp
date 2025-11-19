#include "Mouse.h"



void Mouse::OnMouseMove(unsigned int x, unsigned int y) noexcept
{
    X = x;
    Y = y;
}

void Mouse::MouseEnter() noexcept
{
    IsInWindow = true;
}

void Mouse::MouseExit() noexcept
{
    IsInWindow = false;
}

void Mouse::RightDown() noexcept
{
    RightIsDown = true;
}

void Mouse::RightUp() noexcept
{
    RightIsDown = false;
}

void Mouse::LeftDown() noexcept
{
    LeftIsDown = true;
}

void Mouse::LeftUp() noexcept
{
    LeftIsDown = false;
}
