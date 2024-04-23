#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#ifndef OPENGL_RENDERER_HPP
#define OPENGL_RENDERER_HPP

namespace Renderer {
    class Window {
    public:
        explicit Window(const std::string &windowName) : windowName(windowName) {
            this->windowName = windowName;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            window = glfwCreateWindow(800, 600, this->windowName.c_str(), nullptr, nullptr);
            if (window == nullptr) {
                glfwTerminate();
                throw std::runtime_error("Failed to create GLFW window");
            }
            glfwSetWindowUserPointer(window, this);
            (void) glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        }

        ~Window() {
            glDeleteBuffers(1, &this->VBO);
            glDeleteProgram(this->shaderProgram);
            glDeleteVertexArrays(1, &this->VAO);

            glfwDestroyWindow(window);
            glfwTerminate();

            std::cerr << "Terminated OpenGL Context" << std::endl;
        }

        int CreateContext();

    private:
        std::string windowName;
        GLFWwindow *window;
        GLuint VBO{}, VAO{}, vertexShader{}, fragmentShader{}, shaderProgram{};
        float vertices[9] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
        };

        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

        static void processInput(GLFWwindow *window);

        void initVBO();

        void initVertex();

        void initFragment();

        void postShader();

        void initVAO();
    };
}

#endif //OPENGL_RENDERER_HPP