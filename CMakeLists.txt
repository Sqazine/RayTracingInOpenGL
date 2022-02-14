cmake_minimum_required(VERSION 3.10)

set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")

message("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
message("VCPKG_TARGET_TRIPLET=${VCPKG_TARGET_TRIPLET}")

project(RayTracingInOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

FILE(GLOB ROOT_DIR "*.h" "*.cpp")
FILE(GLOB GL_DIR "GL/*.h" "GL/*.cpp")

source_group("" FILES ${ROOT_DIR})
source_group("GL" FILES  ${GL_DIR})

find_package(glad CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(RayTracingInOpenGL ${ROOT_DIR}  ${GL_DIR})

target_link_libraries(RayTracingInOpenGL PRIVATE glad::glad SDL2::SDL2 imgui::imgui)

target_compile_definitions(RayTracingInOpenGL PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/Shaders/")
