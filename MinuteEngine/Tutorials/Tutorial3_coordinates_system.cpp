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

std::function<void()> loop;
void main_loop() { loop(); }

int wnd_width = 640, wnd_height = 480;

void resize(int width, int height) {
    wnd_width = width;
    wnd_height = height;
    glViewport(0, 0, wnd_width, wnd_height);
}

void mouse_event(const SDL_Event& e) {

}

void window_event(const SDL_Event& e, bool& done) {
    if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_CLOSE)
            done = true;
        else if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            resize(e.window.data1, e.window.data2);
    }
}

void keyboard_event(const SDL_Event& e, bool& done) {

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
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    ME::Shader shader = ME::Shader::create("resources/shaders/testV.glsl", "resources/shaders/testF.glsl");
    //ME::Shader shader = ME::Shader::create("resources/shaders/simpleV.glsl", "resources/shaders/simpleF.glsl");

    GLuint vbo[2];  // 0 for Position, 1 for normal.
    glGenBuffers(2, vbo);

    long long size = 3;
    GLfloat* vertices = new GLfloat[3 * size];
    GLfloat* normals = new GLfloat[3 * size];

    vertices[0] = -1.0f; vertices[1] = -1.0f; vertices[2] = 0.0f;
    vertices[3] = 1.0f; vertices[4] = -1.0f; vertices[5] = 0.0f;
    vertices[6] = -1.0f; vertices[7] = 1.0f; vertices[8] = 0.0f;

    normals[0] = 0.0f; normals[1] = 0.0f; normals[2] = 1.0f;
    normals[3] = 0.0f; normals[4] = 0.0f; normals[5] = 1.0f;
    normals[6] = 0.0f; normals[7] = 0.0f; normals[8] = 1.0f;
    
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * size, vertices, GL_STATIC_DRAW);    // @BUGFIX : sizeof(ptr) returns 8!
    int attrib = shader.getAttrLoc("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * size, normals, GL_STATIC_DRAW);
    attrib = shader.getAttrLoc("normal");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    delete[] vertices;
    delete[] normals;

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 3. Create a Element Buffer Object
    GLuint ebo[2];
    glGenBuffers(2, ebo);

    size = 1;

    unsigned int* faces = new unsigned int[3 * size];
    unsigned int* edges = new unsigned int[6 * size];

    faces[0] = 0;
    faces[1] = 1;
    faces[2] = 2;

    edges[0] = 0;
    edges[1] = 1;
    edges[2] = 1;
    edges[3] = 2;
    edges[4] = 2;
    edges[5] = 0;

    // Face
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * size, faces, GL_STATIC_DRAW);

    // Edge
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * size, edges, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] faces;
    delete[] edges;

    
    shader.setUnifVec3("color", { 1.0f, 0.0f, 0.0f });
    shader.setUnifMat4("modelMat", glm::mat4());    // Identity matrix
    shader.setUnifMat4("viewMat", glm::mat4());
    shader.setUnifMat4("projMat", glm::mat4());


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
            mouse_event(event);
            window_event(event, done);
            keyboard_event(event, done);
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // Rendering
        // Clear the screen to black
        glClearColor(0.8f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.enable();

        // Render by mode.
        /*if (C.M == C.PHONG)
            glUniform1i(get_shader_c().get_uniform_location("phong"), true);
        else
            glUniform1i(get_shader_c().get_uniform_location("phong"), false);*/

        //glBindVertexArrayOES(BO.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        int attrib = shader.getAttrLoc("position");
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        attrib = shader.getAttrLoc("normal");
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindVertexArrayOES(0);
        ME::Shader::disable();

        SDL_GL_SwapWindow(window);
    };
    while (!done)
        main_loop();
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}