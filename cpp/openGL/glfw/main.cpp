#include <iostream>
#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include "stb/stb_image.h"

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

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

    // the distance between lower t points
    float a { 0.9 };
    // top points y
    float b { (static_cast<float>(std::sqrt(3)) * a) / 2 };

    GLfloat vertices[] = 
    {
    //  |      coords     |     |      color      |     |   img   |
         0.1f,  0.1f, 0.0f,     0.85f, 0.1f, 0.26f,     1.0f, 1.0f, // top right
         0.1f, -0.1f, 0.0f,     0.85f, 0.1f, 0.26f,     1.0f, 0.0f, // bottom right
        -0.1f, -0.1f, 0.0f,     0.85f, 0.1f, 0.26f,     0.0f, 0.0f, // bottom left
        -0.1f,  0.1f, 0.0f,     0.85f, 0.1f, 0.26f,     0.0f, 1.0f, // top left

    };

    GLuint indices[] = 
    {
        0, 1, 2, // right triangle
        2, 3, 0, // left triangle
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

    GLint uniID { glGetUniformLocation(shaderProgram.ID, "scale") };

    int widthImg, heightImg, numColorCh;
    unsigned char *bytes = stbi_load("textures/squareObel.jpg", &widthImg, &heightImg, &numColorCh, 1);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_INT, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

    // main loop
    while(!glfwWindowShouldClose(window))
    {
        // proccess events
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        glUniform1f(uniID, 3.5f);
        glBindTexture(GL_TEXTURE_2D, texture);
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
    }

    // clean up
    glfwDestroyWindow(window);
    window = nullptr;

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glDeleteTextures(1, &texture);

    glfwTerminate();
    return 0;
}
