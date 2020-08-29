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

#include "Shader.h"
#include "Camera.h"
#include "Mouse.h"

std::function<void()> loop;
void main_loop() { loop(); }

int wnd_width = 640, wnd_height = 480;
ME::Camera camera;
ME::Mouse mouse;

void resize(int width, int height) {
    wnd_width = width;
    wnd_height = height;
    glViewport(0, 0, wnd_width, wnd_height);

    camera.updateProjMat(width, height);
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
        else if (e.key.keysym.scancode == SDL_SCANCODE_F2) {
            camera = ME::Camera::create(wnd_width, wnd_height);
        }
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ SDL_Init ] failed : %s" << SDL_GetError() << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Anti-Alisiing
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wnd_width, wnd_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
    //glDisable(GL_CULL_FACE);
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
    glEnable(GL_MULTISAMPLE);

    //ME::Shader shader = ME::Shader::create("resources/shaders/testV.glsl", "resources/shaders/testF.glsl");
    ME::Shader shader = ME::Shader::create("resources/shaders/simpleV.glsl", "resources/shaders/simpleF.glsl");

    GLuint vbo;  
    glGenBuffers(1, &vbo);

    long long size = 8;
    GLfloat* vertices = new GLfloat[3 * size];

    vertices[0] = -1.0f;    vertices[1] = -1.0f;    vertices[2] = 1.0f;
    vertices[3] = 1.0f;     vertices[4] = -1.0f;    vertices[5] = 1.0f;
    vertices[6] = 1.0f;     vertices[7] = 1.0f;     vertices[8] = 1.0f;
    vertices[9] = -1.0f;    vertices[10] = 1.0f;    vertices[11] = 1.0f;
    vertices[12] = -1.0f;   vertices[13] = -1.0f;   vertices[14] = -1.0f;
    vertices[15] = 1.0f;    vertices[16] = -1.0f;   vertices[17] = -1.0f;
    vertices[18] = 1.0f;    vertices[19] = 1.0f;    vertices[20] = -1.0f;
    vertices[21] = -1.0f;   vertices[22] = 1.0f;    vertices[23] = -1.0f;
    
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * size, vertices, GL_STATIC_DRAW);    // @BUGFIX : sizeof(ptr) returns 8!
    int attrib = shader.getAttrLoc("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    delete[] vertices;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 3. Create a Element Buffer Object
    GLuint ebo;
    glGenBuffers(1, &ebo);

    size = 12;  // 2 * 6 for cube

    unsigned int* faces = new unsigned int[3 * size];

    faces[0] = 0; faces[1] = 1; faces[2] = 2;
    faces[3] = 0; faces[4] = 2; faces[5] = 3;

    int add = 6;
    faces[add + 0] = 4; faces[add + 1] = 6; faces[add + 2] = 5;
    faces[add + 3] = 4; faces[add + 4] = 7; faces[add + 5] = 6;

    add = 12;
    faces[add + 0] = 3; faces[add + 1] = 2; faces[add + 2] = 6;
    faces[add + 3] = 3; faces[add + 4] = 6; faces[add + 5] = 7;

    add = 18;
    faces[add + 0] = 4; faces[add + 1] = 0; faces[add + 2] = 3;
    faces[add + 3] = 4; faces[add + 4] = 3; faces[add + 5] = 7;

    add = 24;
    faces[add + 0] = 0; faces[add + 1] = 5; faces[add + 2] = 1;
    faces[add + 3] = 0; faces[add + 4] = 4; faces[add + 5] = 5;

    add = 30;
    faces[add + 0] = 5; faces[add + 1] = 6; faces[add + 2] = 2;
    faces[add + 3] = 5; faces[add + 4] = 2; faces[add + 5] = 1;

    // Face
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * size, faces, GL_STATIC_DRAW);

    delete[] faces;
        
    camera = ME::Camera::create(wnd_width, wnd_height);
    shader.setUnifMat4("modelMat", glm::mat4(1.0f));    // Identity matrix

    // Main loop.
    bool done = false;
    bool show_demo_window = true;
    loop = [&]
    {
        // Poll and handle events (inputs, window resize, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            mouseEvent(event);
            windowEvent(event, done);
            keyboardEvent(event, done);
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // Rendering
        // Clear the screen to black
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.setUnifMat4("viewMat", camera.getViewMatC());
        shader.setUnifMat4("projMat", camera.getProjMatC());

        shader.enable();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        int attrib = shader.getAttrLoc("position");
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        shader.setUnifVec3("color", { 0.7f, 0.3f, 0.3f });
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 3 * (int)size, GL_UNSIGNED_INT, 0);

        shader.setUnifVec3("color", { 0.0f, 0.0f, 0.0f });
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 3 * (int)size, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        shader.disable();

        SDL_GL_SwapWindow(window);
    };
    while (!done)
        main_loop();
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}