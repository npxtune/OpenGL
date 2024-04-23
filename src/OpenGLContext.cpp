#include "OpenGLContext.hpp"

namespace Rendering {
    int OpenGLContext::createContext() {
        glfwMakeContextCurrent(this->window);
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == GL_FALSE) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        std::cerr << "Using OpenGL " << glGetString(GL_VERSION) << std::endl;

        // Fix for DPI
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        if (windowName.empty()) {
            if (const auto versionStr = reinterpret_cast<const char*>(glGetString(GL_VERSION)); versionStr != nullptr) {
                std::string version = "OpenGL ";
                (void)version.append(versionStr);
                glfwSetWindowTitle(window, version.c_str());
            }
            else {
                throw std::runtime_error("Failed to retrieve OpenGL version");
            }
        }

        createVertexShader();
        createFragmentShader();
        linkShaderProgram();

        initObjectBuffers(&this->VAO, &this->VBO, &this->EBO);

        //  DEBUG
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        while (glfwWindowShouldClose(this->window) == 0) {
            processInput(this->window);

            glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(this->VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(this->window);
            glfwPollEvents();
        }
        return 0;
    }

    void OpenGLContext::createVertexShader() {
        const auto vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(this->vertexShader);

        int success;
        glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(this->vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
        }
    }

    void OpenGLContext::createFragmentShader() {
        const auto fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(0.5f, 0.2f, 1.0f, 1.0f);\n"
            "}\0";

        this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(this->fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(this->fragmentShader);

        int success;
        glGetShaderiv(this->fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(this->vertexShader, 512, nullptr, infoLog);
            std::cerr << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
        }
    }

    void OpenGLContext::linkShaderProgram() {
        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, this->vertexShader);
        glAttachShader(this->shaderProgram, this->fragmentShader);
        glLinkProgram(this->shaderProgram);

        int success;
        glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(this->shaderProgram, 512, nullptr, infoLog);
            std::cerr << "ERROR: SHADER PROGRAM CREATION FAILED\n" << infoLog << std::endl;
        }

        glUseProgram(this->shaderProgram);
        glDeleteShader(this->vertexShader);
        glDeleteShader(this->fragmentShader);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGLContext::initObjectBuffers(GLuint *vbo, GLuint *vao, GLuint *ebo) const {
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glGenBuffers(1, ebo);
        glBindVertexArray(*vao);

        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OpenGLContext::framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void OpenGLContext::processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}
