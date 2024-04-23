#include "Renderer.hpp"

#include <iostream>

int main() {
    std::unique_ptr<Renderer::Window> windowPtr (new Renderer::Window("Fraktale"));
    return windowPtr->CreateContext();
}