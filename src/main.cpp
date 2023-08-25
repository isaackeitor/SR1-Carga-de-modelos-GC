#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <SDL.h>
#include <ctime>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct Face {
    std::vector<int> vertexIndices;
};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 680;

Color currentColor = {0, 0, 0, 255};
Color clearColor = {255, 255, 255, 255};

std::vector<glm::vec3> vertices;
std::vector<Face> faces;
std::vector<glm::vec3> modelVertices;
float rotationAngle = 0.0f;

void drawTriangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SR 1 - Carga de modelos", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void clear() {
    SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    SDL_RenderClear(renderer);
}

void render() {
    for (size_t i = 0; i < modelVertices.size(); i += 3) {
        drawTriangle(modelVertices[i], modelVertices[i + 1], modelVertices[i + 2]);
    }
}

bool loadOBJ(const std::string &path, std::vector<glm::vec3> &out_vertices, std::vector<Face> &out_faces) {
    std::ifstream file(path);
    if (!file.is_open()) {
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
            out_vertices.push_back(vertex);
        } else if (type == "f") {
            Face face;
            std::string vertexIndexStr;
            while (iss >> vertexIndexStr) {
                std::istringstream vertexIndexStream(vertexIndexStr);
                int vertexIndex;
                vertexIndexStream >> vertexIndex;
                --vertexIndex;
                face.vertexIndices.push_back(vertexIndex);
            }
            out_faces.push_back(face);
        }
    }

    return true;
}

std::vector<glm::vec3> setupVertexArray(const std::vector<glm::vec3> &vertices, const std::vector<Face> &faces) {
    std::vector<glm::vec3> vertexArray;

    for (const auto &face : faces) {
        glm::vec3 firstVertex = vertices[face.vertexIndices[0]];
        for (size_t i = 2; i < face.vertexIndices.size(); i++) {
            vertexArray.push_back(firstVertex);
            vertexArray.push_back(vertices[face.vertexIndices[i - 1]]);
            vertexArray.push_back(vertices[face.vertexIndices[i]]);
        }
    }

    return vertexArray;
}

void drawTriangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 0.8f, 0.2f));
    glm::vec4 v1_rotated = rotation * glm::vec4(v1, 1.0f);
    glm::vec4 v2_rotated = rotation * glm::vec4(v2, 1.0f);
    glm::vec4 v3_rotated = rotation * glm::vec4(v3, 1.0f);

    float scale = 0.05;             
    glm::vec3 translation(sin(rotationAngle) * 0.5, 0, 0); 

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

int main(int argc, char **argv) {
    init();

    if (!loadOBJ("/Users/josue/Library/CloudStorage/OneDrive-Personal/Documents/UVG/SEXTO SEMESTRE/GRAFICAS/GRAPHICS/spaceship.obj", vertices, faces)) {
        std::cerr << "Failed to load model" << std::endl;
        return 1;
    }


    modelVertices = setupVertexArray(vertices, faces);

    bool running = true;
    clear();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();
        rotationAngle += 0.0005f;
        render();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
