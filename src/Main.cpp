#include "OpenGLContext.hpp"

#include <iostream>

int main() {
    try {
        const std::unique_ptr<Rendering::OpenGLContext> contextPtr(new Rendering::OpenGLContext());
        return contextPtr->createContext();
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
