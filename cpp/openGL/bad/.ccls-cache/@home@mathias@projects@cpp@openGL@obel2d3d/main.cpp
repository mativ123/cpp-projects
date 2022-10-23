#include <iostream>
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "include/stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    // window dimensions
    int windowW { 1000 };
    int windowH { 1000 };

    // initialize glfw
    glfwInit();

    // tell glfw which opengl version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = 
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    GLuint indices[] = 
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    GLfloat lightVerts[] =
    {
        0.9f, 0.9f, 0.9f, // 0
        0.9f, 0.7f, 0.9f, // 1
        0.7f, 0.7f, 0.9f, // 2
        0.7f, 0.7f, 0.7f, // 3
        0.7f, 0.9f, 0.9f, // 4
        0.7f, 0.9f, 0.7f, // 5
        0.9f, 0.9f, 0.7f, // 6
        0.9f, 0.7f, 0.7f, // 7
    };

    GLuint lightInds[] =
    {
        0, 4, 6,
        4, 6, 5,
        0, 1, 7,
        7, 6, 0,
        0, 1, 2,
        2, 4, 0,
        4, 2, 5,
        5, 3, 2,
        5, 3, 7,
        7, 6, 5,
        1, 7, 3,
        1, 2, 3,
    };

    // create a window
    GLFWwindow *window = glfwCreateWindow(windowW, windowH, "balls", NULL, NULL);
    // check for erroes
    if(window == NULL)
    {
        std::cout << "Failed to create glfw window\n";
        glfwTerminate();
        return -1;
    }

    // put the window into the current context
    glfwMakeContextCurrent(window);

    // load glad
    gladLoadGL();

    // define which portion of the window to use
    glViewport(0, 0, windowW, windowH);

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    Shader lightShader("light.vert", "light.frag");

    VAO lightVAO;
    lightVAO.Bind();
    
    VBO lightVBO(lightVerts, sizeof(lightVerts));
    EBO lightEBO(lightInds, sizeof(lightInds));

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); 

    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec3

    // texture
    Texture obel("squareObel.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    obel.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(windowW, windowH, glm::vec3(0.0f, 0.0f, 2.0f));

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        // proccess events
        glfwPollEvents();

        glClearColor(0.11f, 0.96f, 0.73f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        shaderProgram.Activate();
        camera.Matrix(shaderProgram, "camMatrix");
        obel.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightInds)/sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }

    // clean up
    glfwDestroyWindow(window);
    window = nullptr;

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    obel.Delete();
    shaderProgram.Delete();

    glfwTerminate();
    return 0;
}
