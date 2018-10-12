#pragma once

#include <vector>
#include <algorithm>

#include "singleton.h"
#include "event.h"

namespace rain
{
    class Input : public Singleton<Input>
    {
    public:
        void update()
        {
            m_keyPressedEventArgs.clear();
            m_keyReleasedEventArgs.clear();
            m_mouseMotionEventArgs.clear();
            m_mouseButtonPressedEventArgs.clear();
            m_mouseButtonReleasedEventArgs.clear();
            m_mouseWheelEventArgs.clear();
            m_resizeEventArgs.clear();
        }

        virtual void OnKeyPressed(KeyEventArgs& e)
        {
            m_keyPressedEventArgs.push_back(e);
        }
        
        virtual void OnKeyReleased(KeyEventArgs& e)
        {
            m_keyReleasedEventArgs.push_back(e);
        }
        
        virtual void OnMouseMoved(MouseMotionEventArgs& e)
        {
            m_mouseMotionEventArgs.push_back(e);
        }

        virtual void OnMouseButtonPressed(MouseButtonEventArgs& e)
        {
            m_mouseButtonPressedEventArgs.push_back(e);
        }

        virtual void OnMouseButtonReleased(MouseButtonEventArgs& e)
        {
            m_mouseButtonReleasedEventArgs.push_back(e);
        }

        virtual void OnMouseWheel(MouseWheelEventArgs& e)
        {
            m_mouseWheelEventArgs.push_back(e);
        }

        virtual void OnResize(ResizeEventArgs& e)
        {
            m_resizeEventArgs.push_back(e);
        }

        bool IsPressed(const KeyEventArgs& e)
        {
            return std::find(m_keyPressedEventArgs.begin(), m_keyPressedEventArgs.end(), e) != m_keyPressedEventArgs.end();
        }

        bool IsReleased(const KeyEventArgs& e)
        {
            return std::find(m_keyReleasedEventArgs.begin(), m_keyReleasedEventArgs.end(), e) != m_keyReleasedEventArgs.end();
        }


    private:
        std::vector<KeyEventArgs> m_keyPressedEventArgs;
        std::vector<KeyEventArgs> m_keyReleasedEventArgs;
        std::vector<MouseMotionEventArgs> m_mouseMotionEventArgs;
        std::vector<MouseButtonEventArgs> m_mouseButtonPressedEventArgs;
        std::vector<MouseButtonEventArgs> m_mouseButtonReleasedEventArgs;
        std::vector<MouseWheelEventArgs> m_mouseWheelEventArgs;
        std::vector<ResizeEventArgs> m_resizeEventArgs;
    };
}