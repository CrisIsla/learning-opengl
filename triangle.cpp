#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Now, a vertex and a fragment shaders are needed.
    // Basic versions are stored at the begining of the code.
    // To use this shaders, first they have to be compiled. The compilation is made in
    // runtime with the glCompileShader function.
    // First, a shader object is created.
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // This function takes the shader object to compile to as its first argument, how many
    // strings we are passing as source code as the second argument, and the actual source code
    // as the third.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // It can be checked if compilation was successful
    int vertexSuccess;
    char vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    if (!vertexSuccess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << vertexInfoLog << std::endl;
    }

    // Now, the same process with the fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    int fragmentSuccess;
    char fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!fragmentSuccess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << fragmentInfoLog << std::endl;
    }

    // After compiling both shaders, the next step is to link them into
    // a shader program.
    // First, a shader program is created
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // Then, the shaders are attached and linked.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // It can also be checked if theres any linking error.
    int programSuccess;
    char programInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
    if (!programSuccess)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, programInfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << programInfoLog << std::endl;
    }

    // The shaders are deleted (they are now included in the program).
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // We define an array with the three vertices of the triangle.
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Every vertex attribute call will be stored inside a buffer. This can be useful when we
    // want to draw multiple objects.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // A buffer object is needed to pass as many vertex as we can at once to the GPU memory.
    unsigned int VBO;

    // First the buffer is generated.
    glGenBuffers(1, &VBO);

    // Then the buffer is binded to GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*
    This function is used to load data into a buffer. In this case, we load vertices into
    the GL_ARRAY_BUFFER target. That is specified in the first argument. The second argument
    is the size of the data, the third is the data and the fourth is how the data is going to
    be used in the GPU.
    There are three possible modes:

        GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        GL_STATIC_DRAW: the data is set only once and used many times.
        GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    Now, we have to specify how OpenGL should interpret the vertex data before rendering.
    glVertexAttribPointer receive the following parameters:
        1. The first parameter specifies which vertex attribute we want to configure. In this case
        we pass 0 because we want to pass data to the position vertex atributte, that we declare equal
        to zero in the shader.
        2. This argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so
        it is composed of 3 values.
        3. The third argument specifies the type of the data which in this case is GL_FLOAT (a vec* in
        GLSL consists of floating point values).
        4. This argument specifies if we want the data to be normalized.
        5. The fifth argument is known as the stride and tells us the space between consecutive vertex attributes.
        6. This last argument is the offset of where the position data begins in the buffer.

    Then, this vertex attribute is enabled with glEnableVertexAttribArray, giving the vertex attribute
    location as its argument.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Using the created program and the VAO, we use glDrawArrays to draw the triangle.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}