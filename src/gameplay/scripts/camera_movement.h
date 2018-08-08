#pragma once

#include "ecs/ecs_component.h"
#include "gfx/gfx_camera.h"

namespace rain
{
    struct CameraMovement : public Component
    {
        Camera* camera;
        void Init(void);
        void Update(void);
        void Shutdown(void);
    };


    void InitCameraMovement()
    {

    }

    void UpdateCameraMovement()
    {

    }

    void ShutdownCameraMovement()
    {

    }

    CameraMovement* CreateCameraMovement(Camera* _camera)
    {
        CameraMovement* cameraMovement = (CameraMovement*)malloc(sizeof(CameraMovement));
        cameraMovement->componentType = ComponentType::SCRIPT;
        cameraMovement->Init = InitCameraMovement;
        cameraMovement->Update = UpdateCameraMovement;
        cameraMovement->Shutdown = ShutdownCameraMovement;
        return cameraMovement;
    }

    void InitCameraMovement(CameraMovement* _cameraMovement, void(*Init)(void), void(*Update)(void), void(*Shutdown)(void))
    {
        CameraMovement* _cameraMovement = (CameraMovement*)malloc(sizeof(CameraMovement));
        _cameraMovement->componentType = ComponentType::SCRIPT;
        _cameraMovement->Init = Init;
        _cameraMovement->Update = Update;
        _cameraMovement->Shutdown = Shutdown;
    }
}