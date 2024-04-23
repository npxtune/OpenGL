#include "Renderer.hpp"

namespace Renderer {
    int Window::CreateContext() {

        glfwMakeContextCurrent(this->window);
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == GL_FALSE) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        std::cerr << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

        // Fix for DPI
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        std::cerr << "Window Width: " << width << "\n" << "Window Height: " << height << std::endl;
        glViewport(0, 0, width, height);

        initVBO();

        initVertex();
        initFragment();
        postShader();

        initVAO();

        while (!glfwWindowShouldClose(this->window)) {
            processInput(this->window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
        return 0;
    }

    void Window::initVBO() {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, std::size(this->vertices), this->vertices, GL_STATIC_DRAW);
    }

    void Window::initVertex() {
        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(this->vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(this->vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void Window::initFragment() {
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "\n"
                                           "void main()\n"
                                           "{\n"
                                           "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\0";

        this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(this->fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(this->fragmentShader);

        int success;
        char infoLog[512];
        glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(this->vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void Window::postShader() {
        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, this->vertexShader);
        glAttachShader(this->shaderProgram, this->fragmentShader);
        glLinkProgram(this->shaderProgram);

        int success;
        char infoLog[512];
        glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(this->shaderProgram, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
        }

        glUseProgram(this->shaderProgram);
        glDeleteShader(this->vertexShader);
        glDeleteShader(this->fragmentShader);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    }

    void Window::initVAO() {
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    }

    void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}