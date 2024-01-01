#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// This function will be called every time the window is resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// This function is called in the main loop
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // Initializes the window
    glfwInit();
    // Window configuration
    // We use version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // And only the core profile (just core functionalities)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window object. The function requires the window width, hight and title.
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Make the window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Sets the viewport. The function requires the coordinates
    // of the lower left corner, and the width and height of the rendering window (in pixels)
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Set function for window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleans up all the resourses allocated by GLFW
    glfwTerminate();
    return 0;
}