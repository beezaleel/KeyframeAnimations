
#include "globalOpenGL.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "globalThings.h"

#include "cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightHelper.h"
#include "cVAOManager/c3DModelFileLoader.h"
#include "cBasicTextureManager/cBasicTextureManager.h"
#include "SteelingBehaviorSystem.h"
#include "AnimationManager.h"
#include "AnimationLoader.h"

glm::vec3 g_cameraEye = glm::vec3(4555.0f, 347.0f, 4020.0f);
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

cBasicTextureManager* g_pTextureManager = NULL;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void DrawObject(cMeshObject* pCurrentMeshObject,
    glm::mat4x4 mat_PARENT_Model,
    GLuint shaderID, 
    cBasicTextureManager* pTextureManager,
    cVAOManager* pVAOManager,
    GLint mModel_location, 
    GLint mModelInverseTransform_location);


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float getRandomFloatInRange(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

bool LoadModelTypesIntoVAO(std::string fileTypesToLoadName,
    cVAOManager* pVAOManager,
    GLuint shaderID)
{
    std::ifstream modelTypeFile(fileTypesToLoadName.c_str());
    if (!modelTypeFile.is_open())
    {
        return false;
    }

    std::string PLYFileNameToLoad;
    std::string friendlyName;
    bool bKeepReadingFile = true;

    const unsigned int BUFFER_SIZE = 1000;
    char textBuffer[BUFFER_SIZE];

    memset(textBuffer, 0, BUFFER_SIZE);

    while (bKeepReadingFile)
    {
        modelTypeFile.getline(textBuffer, BUFFER_SIZE);
        PLYFileNameToLoad.clear();
        PLYFileNameToLoad.append(textBuffer);

        if (PLYFileNameToLoad == "EOF")
        {
            bKeepReadingFile = false;
            continue;
        }

        memset(textBuffer, 0, BUFFER_SIZE);
        modelTypeFile.getline(textBuffer, BUFFER_SIZE);
        friendlyName.clear();
        friendlyName.append(textBuffer);

        sModelDrawInfo motoDrawInfo;

        c3DModelFileLoader fileLoader;

        std::string errorText = "";
        if (fileLoader.LoadPLYFile_Format_XYZ_N_RGBA_UV(PLYFileNameToLoad, motoDrawInfo, errorText))
        {
            std::cout << "Loaded the file OK" << std::endl;
        }
        else
        {
            std::cout << errorText;
        }

        if (pVAOManager->LoadModelIntoVAO(friendlyName, motoDrawInfo, shaderID))
        {
            std::cout << "Loaded the " << friendlyName << " model" << std::endl;
        }

    }

    return true;
}


void DrawConcentricDebugLightObjects(void);

cMeshObject* pDebugSphere_1 = NULL;
cMeshObject* pDebugSphere_2 = NULL;
cMeshObject* pDebugSphere_3 = NULL;
cMeshObject* pDebugSphere_4 = NULL;
cMeshObject* pDebugSphere_5 = NULL;
cMeshObject* pMainLego = NULL;
cMeshObject* pChild1 = NULL;
cMeshObject* pChild2 = NULL;
cMeshObject* pChild3 = NULL;
cMeshObject* pBirdOfPrey = NULL;
AnimationManager animationManager;

float currentTime = 0.0f;
float previousTime = 0.0f;
float timeDelta = 0.0f;

// Load animations from file
void LoadAnimationData() {
    // for each animation data in xml file
    //AnimationLoader loader = AnimationLoader();
   // std::map<std::string, AnimationData> animations;
    //loader.LoadFromXML(animations);
    //for (std::map<std::string, AnimationData>::iterator it = animations.begin(); it != animations.end(); it++) {
        //animationManager.Load(it->first, it->second);
    //}
    AnimationData easeInAnimation;
    easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2970.0, 120, 500), 0.0f, EaseIn));
    easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(3121, 120, 5581), 0.15f, EaseIn));
    easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-158, 120, 5581), 0.25f, EaseIn));
    easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-158.0, 120, 8978), 0.50f, EaseIn));
    easeInAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5201.0, 120, 8978), 1.0f));
    easeInAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(800.0f), 0.0f));
    easeInAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 0), 0.0f, true));
    easeInAnimation.ColorKeyFrames.push_back(ColorKeyFrame(glm::vec3(1, 0, 0), 0.0f));
    easeInAnimation.Duration = 15.0f;
    animationManager.Load("EaseInAnimation", easeInAnimation);

    AnimationData easeOutAnimation;
    easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2900.0, 120, 500), 0.0f, EaseOut));
    easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(3000, 120, 5500), 0.25f, EaseOut));
    easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-148, 120, 5481), 0.5f, EaseOut));
    easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-138.0, 120, 7878), 0.75f, EaseOut));
    easeOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5301.0, 120, 8178), 1.0f));
    easeOutAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(800.0f), 0.0f));
    easeOutAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 0), 0.0f, true));
    easeOutAnimation.ColorKeyFrames.push_back(ColorKeyFrame(glm::vec3(1, 1, 0), 0.0f));
    easeOutAnimation.Duration = 15.0f;
    animationManager.Load("EaseOutAnimation", easeOutAnimation);

    AnimationData noEasingAnimation;
    noEasingAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2700.0, 120, 500), 0.0f, None));
    noEasingAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2800, 120, 5500), 0.25f, None));
    noEasingAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-128, 120, 5381), 0.5f, None));
    noEasingAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-118.0, 120, 7778), 0.75f, None));
    noEasingAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(5401.0, 120, 8078), 1.0f));
    noEasingAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(800.0f), 0.0f));
    noEasingAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 0), 0.0f, true));
    noEasingAnimation.ColorKeyFrames.push_back(ColorKeyFrame(glm::vec3(1, 1, 1), 0.0f));
    noEasingAnimation.Duration = 15.0f;
    animationManager.Load("NoEasingAnimation", noEasingAnimation);

    AnimationData easeInOutAnimation;
    easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2600.0, 120, 300), 0.0f, EaseInOut));
    easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2700, 120, 5400), 0.25f, EaseInOut));
    easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-108, 120, 5281), 0.5f, EaseInOut));
    easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-98.0, 120, 7678), 0.75f, EaseInOut));
    easeInOutAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(4901.0, 120, 7578), 1.0f));
    easeInOutAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(800.0f), 0.0f));
    easeInOutAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0, 0), 0.0f, true));
    easeInOutAnimation.ColorKeyFrames.push_back(ColorKeyFrame(glm::vec3(0, 1, 0), 0.0f));
    easeInOutAnimation.Duration = 15.0f;
    animationManager.Load("EaseInOutAnimation", easeInOutAnimation);

    AnimationData birdOfPreyAnimation;
    birdOfPreyAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2970.0f, 1500.0f, 2500.0f), 0.0f, None));
    birdOfPreyAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(2970.0f, 2600.0f, 2500.0f), 0.7f, EaseIn));
    birdOfPreyAnimation.ScaleKeyFrames.push_back(ScaleKeyFrame(glm::vec3(7.0f), 0.0f));
    birdOfPreyAnimation.RotationKeyFrames.push_back(RotationKeyFrame(glm::quat(0, 0, 0.45, 0), 0.80f, true));
    birdOfPreyAnimation.ColorKeyFrames.push_back(ColorKeyFrame(glm::vec3(0, 0, 0), 0.0f));
    birdOfPreyAnimation.PositionKeyFrames.push_back(PositionKeyFrame(glm::vec3(-5970.0f, 2900.0f, 2500.0f), 0.9f, EaseOut));
    birdOfPreyAnimation.Duration = 5.0f;
    animationManager.Load("BirdOfPreyAnimation", birdOfPreyAnimation);
}

int main(int argc, char* argv[])
{
    std::cout << "starting up..." << std::endl;

    GLFWwindow* window;
    GLuint vertex_buffer = 0;

    GLuint shaderID = 0;
    GLint vpos_location = 0;
    GLint vcol_location = 0;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(640, 480, "KeyFrame Animation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::cout << "Window created." << std::endl;

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    cShaderManager* pTheShaderManager = new cShaderManager();
    cShaderManager::cShader vertexShader01;
    cShaderManager::cShader fragmentShader01;

    vertexShader01.fileName = "assets/shaders/vertexShader01.glsl";
    fragmentShader01.fileName = "assets/shaders/fragmentShader01.glsl";

    if (!pTheShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01))
    {
        std::cout << "Didn't compile shaders" << std::endl;
        std::string theLastError = pTheShaderManager->getLastError();
        std::cout << "Because: " << theLastError << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Compiled shader OK." << std::endl;
    }

    pTheShaderManager->useShaderProgram("Shader_1");

    shaderID = pTheShaderManager->getIDFromFriendlyName("Shader_1");

    glUseProgram(shaderID);




    ::g_pTheLightManager = new cLightManager();

    cLightHelper* pLightHelper = new cLightHelper();

    // Set up the uniform variable (from the shader
    ::g_pTheLightManager->LoadLightUniformLocations(shaderID);
    //::g_pTheLightManager->vecTheLights[0].position = glm::vec4(10.0f, 10.0f, 0.0f, 1.0f); main
    ::g_pTheLightManager->vecTheLights[0].position = glm::vec4(-1430.0f, 570.0f, 4800.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[0].atten = glm::vec4(0.1f, 0.2f, 1.4e-7f, 1.0f);

    //    ::g_pTheLightManager->vecTheLights[0].param2.x = 1.0f;
    ::g_pTheLightManager->vecTheLights[0].TurnOn();
    ::g_pTheLightManager->vecTheLights[0].param1.x = 0.0f;  // 1 means spot light

    // In the shader Feeney gave you, the direciton is relative
    ::g_pTheLightManager->vecTheLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);

    // inner and outer angles
    ::g_pTheLightManager->vecTheLights[0].param1.y = 10.0f;     // Degrees
    ::g_pTheLightManager->vecTheLights[0].param1.z = 20.0f;     // Degrees
    //::g_pTheLightManager->vecTheLights[1].param1.x = 2.0f;  // 2 means directional

    // No position or attenuation
    ::g_pTheLightManager->vecTheLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[1].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[1].TurnOn();
    ::g_pTheLightManager->vecTheLights[1].param2.x = 2.0f;  // 2 means directional

    // No position or attenuation
    ::g_pTheLightManager->vecTheLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[1].position = glm::vec4(-1430.0f, 10000.0f, 4800.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[1].atten = glm::vec4(0.1f, 0.000001f, 0.00000001f, 1.0f);
    ::g_pTheLightManager->vecTheLights[1].TurnOn();


    /// light 2
    ::g_pTheLightManager->vecTheLights[2].position = glm::vec4(-1680.0f, 630.0f, 4800.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[2].diffuse = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[2].atten = glm::vec4(1.0f, 0.0001f, 0.00001f, 1.0f);
    ::g_pTheLightManager->vecTheLights[2].TurnOn();
    ::g_pTheLightManager->vecTheLights[2].param2.x = 1.0f;
    ::g_pTheLightManager->vecTheLights[2].direction = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    ::g_pTheLightManager->vecTheLights[2].param1.y = 10.0f;
    ::g_pTheLightManager->vecTheLights[2].param1.z = 20.0f;
    ::g_pTheLightManager->vecTheLights[2].TurnOn();



        // Load the models
    cVAOManager* pVAOManager = new cVAOManager();

    if (!LoadModelTypesIntoVAO("assets/PLYFilesToLoadIntoVAO.txt", pVAOManager, shaderID))
    {
        std::cout << "Error: Unable to load list of models to load into VAO file" << std::endl;
    }

    cMeshObject* pSkyBox = new cMeshObject();
    pSkyBox->meshName = "Skybox_Sphere";
    pSkyBox->friendlyName = "skybox";

    LoadAnimationData();

    cMeshObject* pIslandTerrain = new cMeshObject();
    pIslandTerrain->meshName = "Island";     //  "Terrain";
    pIslandTerrain->friendlyName = "island";
    pIslandTerrain->position = glm::vec3(0.0f, -25.0f, 0.0f);
    //pIslandTerrain->bDoNotLight = false;
    pIslandTerrain->bIsVisible = true;
    pIslandTerrain->bUse_RGBA_colour = true;
    pIslandTerrain->RGBA_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pIslandTerrain->scaleXYZ = glm::vec3(200.0f, 200.0f, 200.0f);
    pIslandTerrain->isWireframe = false;
    vec_pMeshObjects.push_back(pIslandTerrain);

    pMainLego = new cMeshObject();
    pMainLego->meshName = "Lego";
    pMainLego->friendlyName = "Lego";
    pMainLego->bUse_RGBA_colour = true;
    pMainLego->bIsVisible = true;
    pMainLego->bDoNotLight = true;
    pMainLego->isWireframe = false;
    pMainLego->position = glm::vec3(2970.0f, 120.0f, 100.0f);
    pMainLego->scaleXYZ.x = 800.0f;
    pMainLego->scaleXYZ.y = 800.0f;
    pMainLego->scaleXYZ.z = 800.0f;
    pMainLego->enabled = true;
    pMainLego->animation.AnimationTime = 0.f;
    pMainLego->animation.IsLooping = true;
    pMainLego->animation.IsPlaying = true;
    pMainLego->hasParent = false;
    pMainLego->animation.AnimationType = "EaseInAnimation";
    pMainLego->animation.Speed = 0.03f;
    vec_pMeshObjects.push_back(pMainLego);

    pChild1 = new cMeshObject();
    pChild1->meshName = "Lego";
    pChild1->friendlyName = "Lego";
    pChild1->bUse_RGBA_colour = true;
    pChild1->bIsVisible = true;
    pChild1->bDoNotLight = true;
    pChild1->isWireframe = false;
    pChild1->position = glm::vec3(2570.0f, 120.0f, 100.0f);
    pChild1->scaleXYZ.x = 800.0f;
    pChild1->scaleXYZ.y = 800.0f;
    pChild1->scaleXYZ.z = 800.0f;
    pChild1->enabled = true;
    pChild1->animation.AnimationTime = 0.f;
    pChild1->animation.IsLooping = true;
    pChild1->animation.IsPlaying = true;
    pChild1->animation.AnimationType = "EaseOutAnimation";
    pChild1->animation.Speed = 0.03f;
    vec_pMeshObjects.push_back(pChild1);

    pChild2 = new cMeshObject();
    pChild2->meshName = "Lego";
    pChild2->friendlyName = "Lego";
    pChild2->bUse_RGBA_colour = true;
    pChild2->bIsVisible = true;
    pChild2->bDoNotLight = true;
    pChild2->isWireframe = false;
    pChild2->position = glm::vec3(2570.0f, 120.0f, 100.0f);
    pChild2->scaleXYZ.x = 800.0f;
    pChild2->scaleXYZ.y = 800.0f;
    pChild2->scaleXYZ.z = 800.0f;
    pChild2->enabled = true;
    pChild2->animation.AnimationTime = 0.f;
    pChild2->animation.IsLooping = true;
    pChild2->animation.IsPlaying = true;
    pChild2->animation.AnimationType = "NoEasingAnimation";
    pChild2->animation.Speed = 0.03f;
    vec_pMeshObjects.push_back(pChild2);

    pChild3 = new cMeshObject();
    pChild3->meshName = "Lego";
    pChild3->friendlyName = "Lego";
    pChild3->bUse_RGBA_colour = true;
    pChild3->bIsVisible = true;
    pChild3->bDoNotLight = true;
    pChild3->isWireframe = false;
    pChild3->position = glm::vec3(2570.0f, 120.0f, 100.0f);
    pChild3->scaleXYZ.x = 800.0f;
    pChild3->scaleXYZ.y = 800.0f;
    pChild3->scaleXYZ.z = 800.0f;
    pChild3->enabled = true;
    pChild3->animation.AnimationTime = 0.f;
    pChild3->animation.IsLooping = true;
    pChild3->animation.IsPlaying = true;
    pChild3->animation.AnimationType = "EaseInOutAnimation";
    pChild3->animation.Speed = 0.03f;
    vec_pMeshObjects.push_back(pChild3);

    pBirdOfPrey = new cMeshObject();
    pBirdOfPrey->meshName = "Bird";
    pBirdOfPrey->friendlyName = "Bird";
    pBirdOfPrey->bUse_RGBA_colour = false;
    pBirdOfPrey->bIsVisible = true;
    pBirdOfPrey->bDoNotLight = false;
    pBirdOfPrey->isWireframe = false;
    pBirdOfPrey->position = glm::vec3(2970.0f, 1500.0f, 2500.0f);
    pBirdOfPrey->textureRatios[0] = 1.0f;
    pBirdOfPrey->textures[0] = "Bird-Of-Prey.bmp";
    pBirdOfPrey->scaleXYZ.x = 8.0f;
    pBirdOfPrey->scaleXYZ.y = 8.0f;
    pBirdOfPrey->scaleXYZ.z = 8.0f;
    pBirdOfPrey->enabled = true;
    pBirdOfPrey->animation.AnimationTime = 0.f;
    pBirdOfPrey->animation.IsLooping = true;
    pBirdOfPrey->animation.IsPlaying = true;
    pBirdOfPrey->animation.AnimationType = "BirdOfPreyAnimation";
    pBirdOfPrey->animation.Speed = 0.10f;
    vec_pMeshObjects.push_back(pBirdOfPrey);

    cMeshObject* pMoon = new cMeshObject();
    pMoon->meshName = "Moon"; 
    pMoon->friendlyName = "Moon";
    pMoon->bUse_RGBA_colour = false;
    pMoon->bIsVisible = true;
    pMoon->bDoNotLight = true;
    pMoon->isWireframe = false;
    pMoon->position = glm::vec3(-33455.0f, 6437.0f, 1160.0f);
    pMoon->textureRatios[0] = 1.0f;
    pMoon->textures[0] = "lroc-color-poles-4k.bmp";
    pMoon->scaleXYZ.x = 1200.0f;
    pMoon->scaleXYZ.y = 1200.0f;
    pMoon->scaleXYZ.z = 1200.0f;
    vec_pMeshObjects.push_back(pMoon);


    pDebugSphere_1 = new cMeshObject();
    pDebugSphere_1->meshName = "ISO_Sphere_1";
    pDebugSphere_1->friendlyName = "Debug Sphere 1";
    pDebugSphere_1->bUse_RGBA_colour = true;
    pDebugSphere_1->RGBA_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pDebugSphere_1->isWireframe = true;
    pDebugSphere_1->SetUniformScale(1.0f);
    pDebugSphere_1->scaleXYZ.x = 30.0f;
    pDebugSphere_1->scaleXYZ.y = 30.0f;
    pDebugSphere_1->scaleXYZ.z = 30.0f;

    pDebugSphere_1->bDoNotLight = true;

    vec_pMeshObjects.push_back(pDebugSphere_1);

    pDebugSphere_2 = new cMeshObject();
    pDebugSphere_2->meshName = "ISO_Sphere_1";
    pDebugSphere_2->friendlyName = "Debug Sphere 2";
    pDebugSphere_2->bUse_RGBA_colour = true;
    pDebugSphere_2->RGBA_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    pDebugSphere_2->isWireframe = true;
    pDebugSphere_2->SetUniformScale(1.0f);
    pDebugSphere_2->bDoNotLight = true;
    vec_pMeshObjects.push_back(pDebugSphere_2);

    pDebugSphere_3 = new cMeshObject();
    pDebugSphere_3->meshName = "ISO_Sphere_1";
    pDebugSphere_3->friendlyName = "Debug Sphere 3";
    pDebugSphere_3->bUse_RGBA_colour = true;
    pDebugSphere_3->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    pDebugSphere_3->isWireframe = true;
    pDebugSphere_3->SetUniformScale(1.0f);
    pDebugSphere_3->bDoNotLight = true;
    vec_pMeshObjects.push_back(pDebugSphere_3);

    pDebugSphere_4 = new cMeshObject();
    pDebugSphere_4->meshName = "ISO_Sphere_1";
    pDebugSphere_4->friendlyName = "Debug Sphere 4";
    pDebugSphere_4->bUse_RGBA_colour = true;
    pDebugSphere_4->RGBA_colour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    pDebugSphere_4->isWireframe = true;
    pDebugSphere_4->SetUniformScale(1.0f);
    pDebugSphere_4->bDoNotLight = true;
    vec_pMeshObjects.push_back(pDebugSphere_4);

    pDebugSphere_5 = new cMeshObject();
    pDebugSphere_5->meshName = "ISO_Sphere_1";
    pDebugSphere_5->friendlyName = "Debug Sphere 5";
    pDebugSphere_5->bUse_RGBA_colour = true;
    pDebugSphere_5->RGBA_colour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    pDebugSphere_5->isWireframe = true;
    pDebugSphere_5->SetUniformScale(1.0f);
    pDebugSphere_5->bDoNotLight = true;
    vec_pMeshObjects.push_back(pDebugSphere_5);

    GLint mvp_location = glGetUniformLocation(shaderID, "MVP");
    GLint mModel_location = glGetUniformLocation(shaderID, "mModel");
    GLint mView_location = glGetUniformLocation(shaderID, "mView");
    GLint mProjection_location = glGetUniformLocation(shaderID, "mProjection");
    GLint mModelInverseTransform_location = glGetUniformLocation(shaderID, "mModelInverseTranspose");

    ::g_pTextureManager = new cBasicTextureManager();

    ::g_pTextureManager->SetBasePath("assets/textures");
    if (!::g_pTextureManager->Create2DTextureFromBMPFile("lroc-color-poles-4k.bmp"))
    {
        std::cout << "Didn't load texture" << std::endl;
    }
    else
    {
        std::cout << "texture loaded" << std::endl;
    }
    ::g_pTextureManager->Create2DTextureFromBMPFile("Turquoise.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Bandit_Colour_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("White.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Bandit_Specular_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Red_Bandit_Colour_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Blue_Bandit_Colour_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Pink_Bandit_Colour_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Green_Bandit_Colour_Map.bmp");
    ::g_pTextureManager->Create2DTextureFromBMPFile("Bird-Of-Prey.bmp");


    std::string errorString = "";
    if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
        "SpaceBox_back6_negZ.bmp",
        "SpaceBox_bottom4_negY.bmp", 
        "SpaceBox_front5_posZ.bmp", 
        "SpaceBox_left2_negX.bmp",
        "SpaceBox_right1_posX.bmp",
        "SpaceBox_top3_posY.bmp",
        true, errorString))
    {
        std::cout << "Loaded the Spacebox cube map OK" << std::endl;
    }
    else
    {
        std::cout << "ERROR: Failed to load the Spacebox cube map. How sad." << std::endl;
        std::cout << "(because: " << errorString << std::endl;
    }

    std::cout.flush();

    currentTime = glfwGetTime();
    previousTime = currentTime;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        timeDelta = currentTime - previousTime;
        previousTime = currentTime;

        animationManager.Update(vec_pMeshObjects, timeDelta);
        ::g_pTheLightManager->CopyLightInformationToShader(shaderID);

        DrawConcentricDebugLightObjects();

        float ratio;
        int width, height;

        glm::mat4x4 matProjection;
        glm::mat4x4 matView;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        matView = glm::lookAt(::g_cameraEye,
            ::g_cameraTarget,
            upVector);

        GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

        glUniform4f(eyeLocation_UniLoc,
            ::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);

        matProjection = glm::perspective(
            0.6f,
            ratio,
            0.1f,
            100000.0f);
 
        glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));


        for (std::vector< cMeshObject* >::iterator itCurrentMesh = vec_pMeshObjects.begin();
            itCurrentMesh != vec_pMeshObjects.end();
            itCurrentMesh++)
        {
            cMeshObject* pCurrentMeshObject = *itCurrentMesh;

            if (!pCurrentMeshObject->bIsVisible)
            {
                continue;
            }

            // This allows the camera to follow the player around.
            ::g_cameraEye = glm::vec3(pMainLego->position.x - 385,
                607.0f,
                pMainLego->position.z - 5620) + glm::vec3(20 * glm::cos(30), 10, 20 * sin(30));
            ::g_cameraTarget = pMainLego->position;

            glm::mat4x4 matModel = glm::mat4x4(1.0f);

            DrawObject(pCurrentMeshObject,
                matModel,
                shaderID, ::g_pTextureManager,
                pVAOManager, mModel_location, mModelInverseTransform_location);
        }

        {
            GLint bIsSkyboxObject_UL = glGetUniformLocation(shaderID, "bIsSkyboxObject");
            glUniform1f(bIsSkyboxObject_UL, (GLfloat)GL_TRUE);

            glm::mat4x4 matModel = glm::mat4x4(1.0f);
            pSkyBox->position = ::g_cameraEye;
            pSkyBox->SetUniformScale(50000.0f);

            DrawObject(pSkyBox,
                matModel,
                shaderID, ::g_pTextureManager,
                pVAOManager, mModel_location, mModelInverseTransform_location);

            glUniform1f(bIsSkyboxObject_UL, (GLfloat)GL_FALSE);
        }
                                                       
        glfwSwapBuffers(window);
        glfwPollEvents();

        std::stringstream ssTitle;
        ssTitle << "Camera (x,y,z): "
            << ::pMainLego->position.x << ", "
            << ::pMainLego->position.y << ", "
            << ::pMainLego->position.z
            << "  Light #0 (xyz): "
            << ::g_pTheLightManager->vecTheLights[0].position.x << ", "
            << ::g_pTheLightManager->vecTheLights[0].position.y << ", "
            << ::g_pTheLightManager->vecTheLights[0].position.z
            << " linear: " << ::g_pTheLightManager->vecTheLights[0].atten.y
            << " quad: " << ::g_pTheLightManager->vecTheLights[0].atten.z;

        std::string theText = ssTitle.str();

        glfwSetWindowTitle(window, ssTitle.str().c_str());

    }

    delete pTheShaderManager;
    delete ::g_pTheLightManager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void DrawConcentricDebugLightObjects(void)
{
    extern bool bEnableDebugLightingObjects;

    if (!bEnableDebugLightingObjects)
    {
        pDebugSphere_1->bIsVisible = false;
        pDebugSphere_2->bIsVisible = false;
        pDebugSphere_3->bIsVisible = false;
        pDebugSphere_4->bIsVisible = false;
        pDebugSphere_5->bIsVisible = false;
        return;
    }

    pDebugSphere_1->bIsVisible = true;
    pDebugSphere_2->bIsVisible = true;
    pDebugSphere_3->bIsVisible = true;
    pDebugSphere_4->bIsVisible = true;
    pDebugSphere_5->bIsVisible = true;

    cLightHelper theLightHelper;

    pDebugSphere_1->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    pDebugSphere_2->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    pDebugSphere_3->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    pDebugSphere_4->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    pDebugSphere_5->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);

    {
        float distance75percent = theLightHelper.calcApproxDistFromAtten(
            0.75f,  // 75%
            0.001f,
            100000.0f,
            ::g_pTheLightManager->vecTheLights[0].atten.x,
            ::g_pTheLightManager->vecTheLights[0].atten.y,
            ::g_pTheLightManager->vecTheLights[0].atten.z);

        //pDebugSphere_2->SetUniformScale(distance75percent);
        pDebugSphere_2->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    }

    { 
        float distance50percent = theLightHelper.calcApproxDistFromAtten(
            0.50f,  // 75%
            0.001f,
            100000.0f,
            ::g_pTheLightManager->vecTheLights[0].atten.x,
            ::g_pTheLightManager->vecTheLights[0].atten.y,
            ::g_pTheLightManager->vecTheLights[0].atten.z);

        //pDebugSphere_3->SetUniformScale(distance50percent);
        pDebugSphere_3->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    }

    {
        float distance25percent = theLightHelper.calcApproxDistFromAtten(
            0.25f,  // 75%
            0.001f,
            100000.0f,
            ::g_pTheLightManager->vecTheLights[0].atten.x,
            ::g_pTheLightManager->vecTheLights[0].atten.y,
            ::g_pTheLightManager->vecTheLights[0].atten.z);

        //pDebugSphere_4->SetUniformScale(distance25percent);
        pDebugSphere_4->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    }

    {
        float distance5percent = theLightHelper.calcApproxDistFromAtten(
            0.05f,  // 75%
            0.001f,
            100000.0f,
            ::g_pTheLightManager->vecTheLights[0].atten.x,
            ::g_pTheLightManager->vecTheLights[0].atten.y,
            ::g_pTheLightManager->vecTheLights[0].atten.z);

        //pDebugSphere_5->SetUniformScale(distance5percent);
        pDebugSphere_5->position = glm::vec3(::g_pTheLightManager->vecTheLights[0].position);
    }
    return;
}

cMeshObject* findObjectByFriendlyName(std::string nameToFind,
    std::vector< cMeshObject* > vec_pMeshObjects,
    bool bSearchChildren)
{

    for (std::vector< cMeshObject* >::iterator itCurrentMesh = vec_pMeshObjects.begin();
        itCurrentMesh != vec_pMeshObjects.end();
        itCurrentMesh++)
    {
        cMeshObject* pCurrentMesh = *itCurrentMesh;

        if (pCurrentMesh->friendlyName == nameToFind)
        {
            return pCurrentMesh;
        }

        cMeshObject* pChildMesh = findObjectByFriendlyName(nameToFind,
            pCurrentMesh->vecChildMeshes,
            bSearchChildren);
        if (pChildMesh)
        {
            return pChildMesh;
        }
    }

    return NULL;
}