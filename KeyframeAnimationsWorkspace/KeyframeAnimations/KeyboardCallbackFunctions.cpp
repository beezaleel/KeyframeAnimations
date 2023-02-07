#include "globalOpenGL.h"
#include "globalThings.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

extern glm::vec3 g_cameraEye;
extern glm::vec3 g_cameraTarget;
extern cMeshObject* pMainLego;
extern cMeshObject* pChild1;
extern cMeshObject* pChild2;
extern cMeshObject* pChild3;
extern cMeshObject* pBirdOfPrey;
extern float timeDelta;


enum eEditMode
{
    MOVING_CAMERA,
    MOVING_LIGHT,
    MOVING_SELECTED_OBJECT
};

eEditMode theEditMode = MOVING_SELECTED_OBJECT;
unsigned int selectedLightIndex = 0;

bool bEnableDebugLightingObjects = true;

float g_EngineThrust = 1.0f;

void key_callback(GLFWwindow* window,
                         int key, int scancode,
                         int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        theEditMode = MOVING_CAMERA;
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        theEditMode = MOVING_LIGHT;

    }
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        theEditMode = MOVING_SELECTED_OBJECT;

    }

    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        // Check for the mods to turn the spheres on or off
        bEnableDebugLightingObjects = false;
    }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        // Check for the mods to turn the spheres on or off
        bEnableDebugLightingObjects = true;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (::pMainLego->animation.IsPlaying) {
            ::pMainLego->animation.IsPlaying = false;
            ::pChild1->animation.IsPlaying = false;
            ::pChild2->animation.IsPlaying = false;
            ::pChild3->animation.IsPlaying = false;
            ::pBirdOfPrey->animation.IsPlaying = false;
        }
        else {
            ::pMainLego->animation.IsPlaying = true;
            ::pChild1->animation.IsPlaying = true;
            ::pChild2->animation.IsPlaying = true;
            ::pChild3->animation.IsPlaying = true;
            ::pBirdOfPrey->animation.IsPlaying = true;
        }
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        ::pMainLego->speed *= 1.0f;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        ::pMainLego->speed *= 2.0f;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        ::pMainLego->speed *= 3.0f;
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        ::pMainLego->speed *= 4.0f;
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        ::pMainLego->speed *= 5.0f;
    }


    switch (theEditMode)
    {
    case MOVING_CAMERA:
        {
            float CAMERA_MOVE_SPEED = 1500.0f;
            if ( (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT )
            {
                CAMERA_MOVE_SPEED *= 1.0f;
            }


            if (key == GLFW_KEY_A)
            {
                move(::pMainLego, 1, 0, CAMERA_MOVE_SPEED, timeDelta);
            }
            if (key == GLFW_KEY_D)
            {
                move(::pMainLego, -1, 0, CAMERA_MOVE_SPEED, timeDelta);
            }
            if (key == GLFW_KEY_W)
            {
                move(::pMainLego, 0, 1, CAMERA_MOVE_SPEED, timeDelta);
            }
            if (key == GLFW_KEY_S)
            {
                move(::pMainLego, 0, -1, CAMERA_MOVE_SPEED, timeDelta);
            }
        }
        break;

    case MOVING_SELECTED_OBJECT:
    {
        
    }
    break;

    case MOVING_LIGHT:
        {
            const float LIGHT_MOVE_SPEED = 200.0f;
            if (key == GLFW_KEY_A)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.x -= LIGHT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_D)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.x += LIGHT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_W)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.z += LIGHT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_S)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.z -= LIGHT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_Q)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.y -= LIGHT_MOVE_SPEED;
            }
            if (key == GLFW_KEY_E)
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].position.y += LIGHT_MOVE_SPEED;
            }

            if (key == GLFW_KEY_PAGE_DOWN)        
            {
                if ( selectedLightIndex > 0 )
                {
                    selectedLightIndex--;
                }
            }
            if (key == GLFW_KEY_PAGE_UP)
            {
                if ( selectedLightIndex < (::g_pTheLightManager->vecTheLights.size() - 1) )
                {
                    selectedLightIndex++;
                }
            }

            if (key == GLFW_KEY_1 )
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.y *= 0.99f;
            }
            if (key == GLFW_KEY_2 )
            {
                ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.y *= 1.01f;
            }
            if (key == GLFW_KEY_3 )
            {
                if (mods == GLFW_MOD_SHIFT)
                {   
                    ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.z *= 0.99f;
                }
                else
                {
                    ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.z *= 0.999f;
                }
            }
            if (key == GLFW_KEY_4 )
            {
                if (mods == GLFW_MOD_SHIFT)
                {   
                    ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.z *= 1.01f;
                }
                else
                {
                    ::g_pTheLightManager->vecTheLights[selectedLightIndex].atten.z *= 1.001f;
                }
            }

        }
        break;

    }

    return;
}
