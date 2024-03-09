#pragma once

namespace Nit::Input
{
    bool IsKeyPressed(uint32_t key);
    void SetEnabled(bool bConsumed);
    bool IsEnabled();
    bool IsMouseButtonPressed(int button);
    Vector2 GetMousePosition();
    void Init(const UniquePtr<Window>& window);
}