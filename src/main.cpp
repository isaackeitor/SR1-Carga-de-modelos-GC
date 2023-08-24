#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 680;

struct Color {
    uint8_t r, g, b, a;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor = {0, 0, 0, 255};
Color clearColor = {255, 255, 255, 255};

std::vector<glm::vec3> vertices;
std::vector<glm::ivec3> faces;

float rotationAngle = 0.0f;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SR 1 - Carga de modelos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void setColor(const Color& color) {
    currentColor = color;
}

void clear() {
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);
}

bool loadOBJ(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the OBJ file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (type == "f") {
            glm::ivec3 face;
            int idx;
            int count = 0;
            while (iss >> idx) {
                if (count < 3) {
                    face[count] = idx - 1;  // OBJ indices start at 1, not 0
                }
                count++;
            }
            if (count == 3) {
                faces.push_back(face);
            }
        }
    }

    file.close();
    return true;
}

void drawTriangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    glm::mat4 rotation = glm::rotate(glm::mat4(0.9f), rotationAngle, glm::vec3(0.0f, 0.8f, 0.2f));
    glm::vec4 v1_rotated = rotation * glm::vec4(v1, 1.0f);
    glm::vec4 v2_rotated = rotation * glm::vec4(v2, 1.0f);
    glm::vec4 v3_rotated = rotation * glm::vec4(v3, 1.0f);

    float scale = -0.05;
    glm::vec3 translation(0, 0, 0);
    glm::vec3 v1_transformed = glm::vec3(v1_rotated) * scale + translation;
    glm::vec3 v2_transformed = glm::vec3(v2_rotated) * scale + translation;
    glm::vec3 v3_transformed = glm::vec3(v3_rotated) * scale + translation;

    int x1 = (v1_transformed.x + 1) * SCREEN_WIDTH / 2;
    int y1 = (v1_transformed.y + 1) * SCREEN_HEIGHT / 2;
    int x2 = (v2_transformed.x + 1) * SCREEN_WIDTH / 2;
    int y2 = (v2_transformed.y + 1) * SCREEN_HEIGHT / 2;
    int x3 = (v3_transformed.x + 1) * SCREEN_WIDTH / 2;
    int y3 = (v3_transformed.y + 1) * SCREEN_HEIGHT / 2;

    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

void render() {
    for (const auto& face : faces) {
        drawTriangle(vertices[face.x], vertices[face.y], vertices[face.z]);
    }
}

int main(int argc, char* argv[]) {
    init();

    if (!loadOBJ("/Users/josue/Library/CloudStorage/OneDrive-Personal/Documents/UVG/SEXTO SEMESTRE/GRAFICAS/GRAPHICS/spaceship.obj")) {
        return -1;
    }
    
    std::cout << "Loaded " << vertices.size() << " vertices and " << faces.size() << " faces." << std::endl;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        rotationAngle += 0.01f;
        render();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
