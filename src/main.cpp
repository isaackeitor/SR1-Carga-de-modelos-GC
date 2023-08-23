#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "triangle.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;

std::vector<glm::vec3> vertices;
std::vector<glm::ivec3> faces;

bool loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the OBJ file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "f") {
            glm::ivec3 face;
            iss >> face.x >> face.y >> face.z;
            faces.push_back(face - glm::ivec3(1));  // Los índices en OBJ comienzan en 1, no en 0
        }
    }

    file.close();
    return true;
}

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setColor(const Color& color) {
    currentColor = color;
}

void render() {
    for (const auto& face : faces) {
        Triangle triangle(vertices[face.x], vertices[face.y], vertices[face.z], currentColor);
        triangle.draw(renderer);
    }
}

int main(int argc, char* argv[]) {
    init();

    if (!loadOBJ("path_to_your_model.obj")) {  // Cambie esto por la ruta de su modelo
        return -1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear(); 

        setColor(Color(255, 255, 0));  // pueden cambiar los colores
        render();

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
