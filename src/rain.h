#pragma once


class InputManager;

class Rain
{
    static void Init();
    static float GetDeltaTime();
    static InputManager* Input();
};
