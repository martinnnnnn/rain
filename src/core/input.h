#pragma once


#include "singleton.h"
#include "event.h"

namespace rain
{
    class Input : public Singleton<Input>
    {
    public:
        //void RegisterCallbacks();
        virtual void OnRender(RenderEventArgs& e);
        virtual void OnKeyPressed(KeyEventArgs& e);
        virtual void OnKeyReleased(KeyEventArgs& e);
        virtual void OnMouseMoved(MouseMotionEventArgs& e);
        virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
        virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
        virtual void OnMouseWheel(MouseWheelEventArgs& e);
        virtual void OnResize(ResizeEventArgs& e);


    private:

    };
}