#include <glad/glad.h>
#include "AWindow.h"

#include "AMouseListener.h"
#include "AKeyListener.h"
#include "../util/ATime.h"
#include "../util/ADefines.h"
#include "../util/AAssetPool.h"
#include "../renderer/ADebugDraw.h"

#include "ALevelEditorScene.h"
#include "ALevelScene.h"

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
    delete instance->propertiesWindow;
    delete instance->mousePicking;
    delete instance->framebuffer;
    delete currentScene;
    delete instance;
    printf("window deleted\n");
}

void AWindow::changeScene(int index)
{
    if(currentScene)
    {
        delete currentScene;
    }
    switch(index)
    {
        case 0:
        {
            currentScene = new ALevelEditorScene();
            currentScene->init();
            currentScene->start();
        }break;
        case 1:
        {
            currentScene = new ALevelScene();
            currentScene->init();
            currentScene->start();
        }break;
        default:
        {
            currentScene = nullptr;
        }break;
    }
}

AScene *AWindow::getScene()
{
    return get()->currentScene;
}

void AWindow::run()
{
    init();
    loop();
    
    currentScene->close();

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
    //ImGui::StyleColorsLight();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    framebuffer = new AFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
    mousePicking = new AMousePicking(WINDOW_WIDTH, WINDOW_HEIGHT);
    propertiesWindow = new APropertiesWindow();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
     
    changeScene(0);

}

void AWindow::loop()
{

    float beginTime = ATime::getTime();
    float endTime = ATime::getTime();
    float dt = -1.0f;

    AShader *defaultShader = AAssetPool::getShader("default");
    AShader *pickingShader = AAssetPool::getShader("picking");

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
            currentScene->update(dt);
            currentScene->render();
        }
        framebuffer->unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        setupDockSpace();

        currentScene->imgui();
        bool show_demo_window = true;
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        } 
        AGameViewWindow::imgui();
        propertiesWindow->update(dt, currentScene);
        propertiesWindow->imgui();
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

void AWindow::onNotify(AGameObject *obj, AEvent *event)
{
    switch(event->type)
    {
        case GAME_ENGINE_START_PLAY:
        {
            printf("starting play\n"); 
        }break;
        case GAME_ENGINE_STOP_PLAY:
        {
            printf("ending play\n");  
        }break;
    }
}

