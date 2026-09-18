#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // Initialize GLFW
    glfwInit();

    // Let GLFW know what OpenGL profile is used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Coordinates for our points
    float vertices[] = { 
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "APPLPHY2", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Use created window
    glfwMakeContextCurrent(window);

    // Load OpenGL functions + error handling
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Create vertext shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Store vertext shader source into shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile
    glCompileShader(vertexShader);
    
    // Create fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Store fragment shader source into shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile
    glCompileShader(fragmentShader);

    // Create program object
    unsigned int shaderProgram = glCreateProgram();

    // Link shaders to program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup shader objects since they're now in program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Buffer to hold shader data and how to interpret that data
    unsigned int VAO, VBO;

    // Generate Buffer and Vertex Array
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  

    // Binds VAO
    glBindVertexArray(VAO);

    // Binds VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Stores our vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configures Vertex Array to tell OpenGL how to interpret data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable it
    glEnableVertexAttribArray(0);
    
    // Clean Up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Prepare to change the buffer with this color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Destroy window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();

    return 0;
}