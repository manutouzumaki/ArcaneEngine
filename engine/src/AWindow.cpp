#include <glad/glad.h>
#include "AWindow.h"

#include "AMouseListener.h"
#include "AKeyListener.h"
#include "../util/ATime.h"
#include "../util/ADefines.h"
#include "../renderer/ADebugDraw.h"

#include "ALevelEditorScene.h"
#include "ALevelScene.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>

AWindow *AWindow::instance = nullptr;
AScene *AWindow::currentScene = nullptr;

void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

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
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

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

    // TODO: Imgui Test
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    
    changeScene(0);

}

void AWindow::loop()
{

    float beginTime = ATime::getTime();
    float endTime = ATime::getTime();
    float dt = -1.0f;

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ADebugDraw::beginFrame();

        glClearColor(0.9f, 0.9f, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if(dt >= 0.0f)
        {
            ADebugDraw::draw();
            currentScene->update(dt);
        }


        // TODO: ImGui Test
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        currentScene->sceneImgui();
        bool show_demo_window = true;
        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        } 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
        glfwSwapBuffers(window); 
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


static void frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    AWindow::setWidth(width);
    AWindow::setHeight(height);
    glViewport(0, 0, width, height);
}

