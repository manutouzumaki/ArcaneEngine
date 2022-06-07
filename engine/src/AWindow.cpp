#include <glad/glad.h>
#include "AWindow.h"

#include "AMouseListener.h"
#include "AKeyListener.h"
#include "../util/ATime.h"
#include "../util/ADefines.h"
#include "../util/AAssetPool.h"
#include "../renderer/ADebugDraw.h"

#include "ALevelEditorSceneInitializer.h"

#include "../editor/AGameViewWindow.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>

AWindow *AWindow::instance = nullptr;
AScene *AWindow::currentScene = nullptr;

static bool condition = true;
void setupDockSpace()
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("Dockspace demo", &condition, windowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::DockSpace(ImGui::GetID("Dockspace"));
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

AWindow::AWindow() 
{
    AEventSystem::addObserver(this);
    runTimePlaying = false;
}

AWindow *AWindow::get()
{
    if(instance == nullptr)
    {
        instance = new AWindow();
    }
    return instance;
}

void AWindow::free()
{
    delete instance->menuBar;
    delete instance->propertiesWindow;
    delete instance->mousePicking;
    delete instance->framebuffer;
    delete currentScene;
    delete instance;
    printf("window deleted\n");
}

void AWindow::changeScene(ASceneInitializer *sceneInitializer)
{
    if(currentScene)
    {
        currentScene->destroy();
        delete currentScene;
    }
    get()->propertiesWindow->setActiveGameObject(nullptr);
    AGameObject::resetUIDCounter();
    currentScene = new AScene(sceneInitializer);
    currentScene->init();
    currentScene->start();
}

AScene *AWindow::getScene()
{
    return get()->currentScene;
}

void AWindow::run()
{
    init();
    loop();
    
    //currentScene->close();

    ADebugDraw::shutDown();

    // TODO: Imgui Test
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void AWindow::init()
{
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "test", nullptr, nullptr);
    // FULL SCREEN
    //window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "test", glfwGetPrimaryMonitor(), nullptr);
    if(window == nullptr)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    
    width = WINDOW_WIDTH;
    height = WINDOW_HEIGHT;

    glfwSetCursorPosCallback(window, AMouseListener::mousePosCallback);
    glfwSetMouseButtonCallback(window, AMouseListener::mouseButtonCallback);
    glfwSetScrollCallback(window, AMouseListener::mouseScrollCallback);
    glfwSetKeyCallback(window, AKeyListener::keyCallback);
    //glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwShowWindow(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsLight();
    //ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    framebuffer = new AFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
    mousePicking = new AMousePicking(WINDOW_WIDTH, WINDOW_HEIGHT);
    propertiesWindow = new APropertiesWindow();
    menuBar = new AMenuBar();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    loadResources(); 
    changeScene(new ALevelEditorSceneInitializer());

}

void AWindow::loop()
{

    float beginTime = ATime::getTime();
    float endTime = ATime::getTime();
    float dt = -1.0f;

    AShader *defaultShader = AAssetPool::getShader("default");
    AShader *pickingShader = AAssetPool::getShader("picking");

    ADebugDraw::init();
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render pass 1 mouse picking
        glDisable(GL_BLEND);
        mousePicking->enableWritting();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ARenderer::bindShader(pickingShader);
        currentScene->render();

        mousePicking->disableWritting();
        glEnable(GL_BLEND);

        // Render pass 2 actual game
        ADebugDraw::beginFrame();
        framebuffer->bind();
        glClearColor(0.8f, 0.8f, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if(dt >= 0.0f)
        {
            ADebugDraw::draw();
            ARenderer::bindShader(defaultShader);
            if(runTimePlaying)
            {
                currentScene->update(dt);
            }
            else
            {
                currentScene->editorUpdate(dt);
            }
            currentScene->render();
        }
        framebuffer->unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setupDockSpace();

        currentScene->imgui();
        bool show_demo_window = false;
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        AGameViewWindow::imgui();
        propertiesWindow->update(dt, currentScene);

        ImGui::Begin("Properties");
        propertiesWindow->imgui();
        ImGui::End();
        
        menuBar->imgui();
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window);

        AMouseListener::endFrame();

        endTime = ATime::getTime();
        dt = endTime - beginTime;
        beginTime = endTime;
    }
}

int AWindow::getWidth()
{
    return get()->width;
}

int AWindow::getHeight()
{
    return get()->height;
}

void AWindow::setWidth(int width)
{
    get()->width = width;
}
void AWindow::setHeight(int height)
{
    get()->height = height;
}

AFramebuffer *AWindow::getFramebuffer()
{
    return get()->framebuffer;
}

float AWindow::getTargetAspectRatio()
{
    return 16.0f / 9.0f;
}

AMousePicking *AWindow::getMousePicking()
{
    return get()->mousePicking;
}

APropertiesWindow *AWindow::getPropertiesWindow()
{
    return get()->propertiesWindow;
}

static void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    AWindow::setWidth(width);
    AWindow::setHeight(height);
    glViewport(0, 0, width, height);
}

void AWindow::loadResources()
{
    AAssetPool::addShader("default", "../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
    AAssetPool::addShader("lines", "../assets/shaders/lineVertex.glsl", "../assets/shaders/lineFragment.glsl");
    AAssetPool::addShader("picking", "../assets/shaders/pickingVertex.glsl", "../assets/shaders/pickingFragment.glsl");
    
    AAssetPool::addTexture("heroTexture", "../assets/textures/poolGuy.png");
    AAssetPool::addTexture("blocksTexture", "../assets/textures/blocks.png");
    AAssetPool::addTexture("characterTexture", "../assets/textures/character.png");
    AAssetPool::addTexture("gizmoTexture", "../assets/textures/gizmos.png");

    AAssetPool::addSpritesheet("characterSpritesheet",
                               new ASpritesheet("characterSpritesheet", AAssetPool::getTexture("characterTexture"), 77, 77, 32, 0));
    AAssetPool::addSpritesheet("blocksSpritesheet",
                               new ASpritesheet("blocksSpritesheet", AAssetPool::getTexture("blocksTexture"), 16, 16, 81, 0));
    AAssetPool::addSpritesheet("gizmoSpritesheet",
                               new ASpritesheet("gizmoSpritesheet", AAssetPool::getTexture("gizmoTexture"), 24, 48, 3, 0));
}

void AWindow::onNotify(AGameObject *obj, AEvent *event)
{
    switch(event->type)
    {
        case GAME_ENGINE_START_PLAY:
        {
            runTimePlaying = true;
            currentScene->save();
            AWindow::changeScene(new ALevelEditorSceneInitializer()); 
        }break;
        case GAME_ENGINE_STOP_PLAY:
        {
            runTimePlaying = false;
            AWindow::changeScene(new ALevelEditorSceneInitializer()); 
        }break;
        case LOAD_LEVEL:
        {
            AWindow::changeScene(new ALevelEditorSceneInitializer()); 
        }break;
        case SAVE_LEVEL:
        {
            currentScene->save();
        }break;
    }
}

