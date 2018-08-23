//#include "win32_game.h"
//
//#include "win32_dx12.h"
//
//#include "win32_application.h"
//#include "win32_game.h"
//#include "win32_window.h"
//
//bool Game::Initialize()
//{
//    // Check for DirectX Math library support.
//    if (!DirectX::XMVerifyCPUSupport())
//    {
//        MessageBoxA(NULL, "Failed to verify DirectX Math library support.", "Error", MB_OK | MB_ICONERROR);
//        return false;
//    }
//
//    m_pWindow = Application::Get().CreateRenderWindow(m_Name, m_Width, m_Height, m_vSync);
//    m_pWindow->RegisterCallbacks(shared_from_this());
//    m_pWindow->Show();
//
//    return true;
//}
//
//void Game::Destroy()
//{
//    Application::Get().DestroyWindow(m_pWindow);
//    m_pWindow.reset();
//}