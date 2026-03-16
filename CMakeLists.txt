cmake_minimum_required(VERSION 3.16.0)
project(sayonara VERSION 0.1.0)



set(CMAKE_CXX_STANDARD 20)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Dependencies
# Raylib (automatically downloaded)
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

# rlImGui (submodule)
include_directories(submodules/rlImGui)

# hidapi (unsure)
add_subdirectory(submodules/hidapi)
include_directories(submodules/hidapi)


# DearImGui (submodule)
include_directories(submodules/imgui)
include_directories(submodules/imgui/backends)



# Build and link
add_executable(${PROJECT_NAME} 
	# Application
  src/main.cpp
  src/utils.cpp

  # Nara API
  src/api/naraapi_hl.cpp
  src/api/naraapi_ll.cpp

  # External Libraries
  submodules/imgui/imgui.cpp
  submodules/imgui/imgui_demo.cpp
  submodules/imgui/imgui_draw.cpp
  submodules/imgui/imgui_tables.cpp
  submodules/imgui/imgui_widgets.cpp
  submodules/rlImGui/rlImGui.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE hidapi::hidapi)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)


# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
