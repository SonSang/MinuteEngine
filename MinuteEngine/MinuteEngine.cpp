/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <iostream>
#include <string>
#include <functional>

// ME
#include "Shader/StandardShader.h"
#include "Shader/ShadowmapShader.h"
#include "Shader/SkyboxShader.h"
#include "Camera.h"
#include "Mouse.h"
#include "Geometry.h"
#include "Render.h"
#include "Scene.h"
#include "Timer.h"
#include "UI.h"

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define IMGUI_GLSL_VERSION "#version 330"

std::function<void()> loop;
void main_loop() { loop(); }

int wnd_width = 640, wnd_height = 480;
ME::Camera camera = ME::Camera::create(wnd_width, wnd_height);
ME::Mouse mouse;
ME::Scene scene = ME::Scene::create();

void resize(int width, int height) {
    wnd_width = width;
    wnd_height = height;
    glViewport(0, 0, wnd_width, wnd_height);

    camera.setWindowSize(wnd_width, wnd_height);
    camera.updateProjMat();
}

void mouseEvent(const SDL_Event& e) {
    if (e.button.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_RIGHT) {
            mouse.setRPress(true);
            mouse.setDirty(true);
            mouse.setCoords((float)e.button.x, (float)e.button.y);
        }            
        else if (e.button.button == SDL_BUTTON_LEFT) {
            mouse.setLPress(true);
            mouse.setDirty(true);
            mouse.setCoords((float)e.button.x, (float)e.button.y);
        }
    }
    else if (e.button.type == SDL_MOUSEBUTTONUP) {
        if (e.button.button == SDL_BUTTON_RIGHT) {
            mouse.setRPress(false);
            if (!mouse.getLPress())
                mouse.setDirty(false);
        }
        else if (e.button.button == SDL_BUTTON_LEFT) {
            mouse.setLPress(false);
            if(!mouse.getRPress())
                mouse.setDirty(false);
        }
    }
    else if (e.button.type == SDL_MOUSEMOTION) {
        mouse.moveCamera(camera, (float)e.button.x, (float)e.button.y);
    }
    else if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) {    
            glm::vec3 offset = camera.getEye() - camera.getCenter();
            offset *= -0.1f;
            camera.translate(offset);
        }
        else if (e.wheel.y < 0) {   
            glm::vec3 offset = camera.getEye() - camera.getCenter();
            offset *= 0.1f;
            camera.translate(offset);
        }
    }
}

void windowEvent(const SDL_Event& e, bool& done) {
    if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_CLOSE)
            done = true;
        else if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            resize(e.window.data1, e.window.data2);
    }
}

void keyboardEvent(const SDL_Event& e, bool& done) {
    switch (e.type) {
    case SDL_KEYDOWN:
        if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            done = true;
        }
        else if (e.key.keysym.scancode == SDL_SCANCODE_W) {
            camera.moveBackForth(1, 1.0f);
        }
        else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
            camera.moveBackForth(-1, 1.0f);
        }
        else if (e.key.keysym.scancode == SDL_SCANCODE_A) {
            camera.moveLeftRight(-1, 1.0f);
        }
        else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
            camera.moveLeftRight(1, 1.0f);
        }
    }
}

void lightSetting() {
    ME::Light directionalLight;
    ME::Light pointLight;

    directionalLight.setType(0);
    directionalLight.setPosition({ 10, 10, 0 });
    directionalLight.setDirection({ -1, -1, 0 });
    directionalLight.setUp({ 0, 1, 0 });
    directionalLight.shadow = ME::Shadow::create();
    directionalLight.shadow.setValid(true);

    /*pointLight.setType(1);
    pointLight.setDiffuse(ME::Color::green());
    pointLight.setSpecular(ME::Color::green());
    pointLight.setPosition({ 5, 1, 5 });
    pointLight.setDirection({ -5, -1, -5 });
    pointLight.setUp({ 0, 1, 0 });
    pointLight.shadow = ME::Shadow::create();
    pointLight.shadow.setValid(true);*/
    
    scene.getLightManager().addLight(directionalLight);
    //scene.getLightManager().addLight(pointLight);
}

void floorSetting() {
    auto object = ME::Object::createPtr("Floor");
    ME::Render::Vertex vertices[4];
    vertices[0].position = { -10, -1, -10 };
    vertices[1].position = { -10, -1, 10 };
    vertices[2].position = { 10, -1, 10 };
    vertices[3].position = { 10, -1, -10 };

    vertices[0].normal = { 0, 1, 0 };
    vertices[1].normal = { 0, 1, 0 };
    vertices[2].normal = { 0, 1, 0 };
    vertices[3].normal = { 0, 1, 0 };

    vertices[0].texcoord = { 0, 0, 0, 0 };
    vertices[1].texcoord = { 5, 0, 0, 0 };
    vertices[2].texcoord = { 5, 5, 0, 0 };
    vertices[3].texcoord = { 0, 5, 0, 0 };

    auto render = ME::QuadRender::createQuadPtr(vertices);
    render->getOption().diffuseMap.texture = ME::Texture2D::create("./resources/textures/parquet/diffuse.jpg");
    render->getOption().diffuseMap.valid = true;
    render->getOption().specularMap.texture = ME::Texture2D::create("./resources/textures/parquet/specular.jpg");
    render->getOption().specularMap.valid = true;
    render->getOption().normalMap.texture = ME::Texture2D::create("./resources/textures/parquet/normal.jpg");
    render->getOption().normalMap.valid = true;
    /*render->option.parallaxMap.texture = ME::Texture2D::create("./resources/textures/parquet/parallax.jpg");
    render->option.parallaxMap.valid = true;*/

    render->getOption().shadeMode = 2;
    render->getOption().drawEdge = false;

    auto prop = ME::Property::createPtr(render);
    prop->setName("Render");
    object->addProperty(prop);
    scene.addObject(object);
}

void sphereSetting() {
    {
        auto object = ME::Object::createPtr("Sphere");

        auto sphere = ME::Surface::createSphere({ 0, 0, 0 }, 1.0, 40, 40, true);

        auto vertexMap = ME::Render::VertexMap::create(sphere);
        auto render = ME::QuadRender::createSurfacePtr(vertexMap);
        render->getOption().shadeMode = 1;
        render->getOption().drawEdge = false;

        auto prop = ME::Property::createPtr(render);
        prop->setName("Render");
        object->addProperty(prop);
        scene.addObject(object);
    }
}

void cubeSetting() {
    auto object = ME::Object::createPtr("Cube");

    ME::Render::Vertex min, max;
    min.position = { -3, 0, -3 };
    max.position = { -2, 2, -2 };
    auto render = ME::QuadRender::createCubePtr(min, max);
    render->getOption().shadeMode = 1;
    render->getOption().drawEdge = false;
    render->getOption().emMode = true;
    render->getOption().emFactor = 0.5f;

    auto prop = ME::Property::createPtr(render);
    prop->setName("Render");
    object->addProperty(prop);
    scene.addObject(object);
}

void skyboxSetting() {
    std::string path[6];
    path[0] = std::string("./resources/textures/skybox3/right.jpg");
    path[1] = std::string("./resources/textures/skybox3/left.jpg");
    path[2] = std::string("./resources/textures/skybox3/top.jpg");
    path[3] = std::string("./resources/textures/skybox3/bottom.jpg");
    path[4] = std::string("./resources/textures/skybox3/front.jpg");
    path[5] = std::string("./resources/textures/skybox3/back.jpg");
    scene.getSkybox() = ME::Scene::Skybox::create(path);
    scene.getSkybox().valid = true;
}

void sceneSetting() {
    lightSetting();
    floorSetting();
    sphereSetting();
    cubeSetting();
    skyboxSetting();
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ SDL_Init ] failed : %s" << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Anti-Alisiing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wnd_width, wnd_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowResizable(window, SDL_TRUE);

    // Returns the OpenGL context associated with [ window ] or NULL on error.
    SDL_GLContext glc = SDL_GL_CreateContext(window);
    if (glc == nullptr) {
        std::cerr << "[ SDL_GL_CreateContext ] failed : %s" << SDL_GetError() << std::endl;
        exit(-1);
    }

    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "[ glewInit ] failed : %s" << glewGetErrorString(err) << std::endl;
        exit(-1);
    }

    // Disable v-sync.
    if (SDL_GL_SetSwapInterval(0) < 0)
        std::cerr << "[ SDL_GL_SetSwapInterval ] failed : %s" << SDL_GetError() << std::endl;

    // Returns a valid rendering context or NULL if there was an error.
    SDL_Renderer* rdr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (rdr == nullptr) {
        std::cerr << "[ SDL_CreateRenderer ] failed : %s" << SDL_GetError() << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    /*glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glEnable(GL_MULTISAMPLE);

    // =================================== UI ===========================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glc);
    ImGui_ImplOpenGL3_Init(IMGUI_GLSL_VERSION);
    // ==================================================================================
    ME::StandardShader standardShader = ME::StandardShader::create("Shader/glsl/330/standard.vert", "Shader/glsl/330/standard.frag");
    ME::ShadowmapShader shadowmapShader = ME::ShadowmapShader::create("Shader/glsl/330/shadowmap.vert", "Shader/glsl/330/shadowmap.frag");
    ME::SkyboxShader skyboxShader = ME::SkyboxShader::create("Shader/glsl/330/skybox.vert", "Shader/glsl/330/skybox.frag");
    scene = ME::Scene::create();

    sceneSetting();

    // Main loop.
    bool done = false;
    bool show_demo_window = true;

    ME::Timer timer;
    timer.setBeg();
    loop = [&]
    {
        // Poll and handle events (inputs, window resize, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            windowEvent(event, done);
            if(!io.WantCaptureMouse)
                mouseEvent(event);
            if(!io.WantCaptureKeyboard)
                keyboardEvent(event, done);
        }

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // Draw UI
        ME::UI::draw(camera, scene);

        // Update the scene
        timer.setEnd();
        camera.setFrameTime(timer.getElapsedTime());
        scene.update(timer.getElapsedTime());
        timer.setBeg();        

        // Clear the screen to black
        ImGui::Render();
        {
            camera.drawBG();

            // 1st pass : Render to shadow map
            shadowmapShader.draw(scene);

            // 2nd pass : Render to screen
            glViewport(0, 0, camera.getWindowWidth(), camera.getWindowHeight());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto skyboxViewMat = glm::mat4(glm::mat3(camera.getViewMatC()));
            skyboxShader.setUnifMat4(skyboxShader.uViewMat(), skyboxViewMat);
            skyboxShader.setUnifMat4(skyboxShader.uProjMat(), camera.getProjMatC());
            skyboxShader.draw(scene);
            
            standardShader.setUnifMat4(standardShader.uViewMat(), camera.getViewMatC());
            standardShader.setUnifMat4(standardShader.uProjMat(), camera.getProjMatC());
            standardShader.setUnifVec3(standardShader.uCameraPosition(), camera.getEye());
            standardShader.draw(scene);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    };
    while (!done)
        main_loop();
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}