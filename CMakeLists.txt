cmake_minimum_required(VERSION 3.26.4)
project(SR VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/opt/homebrew/Cellar/glm/0.9.9.8/include)
include_directories(${SDL2_INCLUDE_DIRS})

# Configurar SDL2 si lo estás utilizando
find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
