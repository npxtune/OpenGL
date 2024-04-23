#pragma once
#include <iostream>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP

namespace Rendering {
    class OpenGLContext {
    public:
        explicit OpenGLContext(std::string windowName = "") : windowName(std::move(windowName)) {
            if (glfwInit() == GLFW_FALSE) {
                std::cerr << &glfwGetError << std::endl;
            }
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
            (void)glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        }

        int createContext();

        ~OpenGLContext() {
            glDeleteProgram(this->shaderProgram);
            glDeleteVertexArrays(1, &this->VAO);
            glDeleteBuffers(1, &this->VBO);
            glDeleteBuffers(1, &this->EBO);

            glfwDestroyWindow(window);
            glfwTerminate();

            std::cerr << "Terminated OpenGL Context" << std::endl;
        }

    private:
        std::string windowName;
        GLFWwindow *window;
        GLuint VBO{}, VAO{}, EBO{}, vertexShader{}, fragmentShader{}, shaderProgram{};
        const float vertices[24] = {
            0.85f, 0.5f, 0.0f, //   0  top right
            0.85f, -0.5f, 0.0f, //  1  bottom right
            0.25f, -0.5f, 0.0f, //  2  bottom left
            0.25f, 0.5f, 0.0f, //   3  top left

            -0.85f, -0.5f, 0.0f, // 4
            -0.85f, 0.5f, 0.0f, // 5
            -0.25f, 0.5f, 0.0f, // 6
            -0.25f, -0.5f, 0.0f // 7
        };
        const unsigned int indices[12] = {
            // note that we start from 0!
            0, 1, 3, // first triangle
            1, 2, 3, // second triangle
            4, 5, 7,
            5, 6, 7
        };

        void createVertexShader();

        void createFragmentShader();

        void linkShaderProgram();

        void initObjectBuffers(GLuint *vbo, GLuint *vao, GLuint *ebo) const;

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

        static void processInput(GLFWwindow *window);
    };
}

#endif OPENGL_CONTEXT_HPP
