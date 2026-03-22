cmake_minimum_required(VERSION 3.10.0)
project(nara VERSION 0.0.1 LANGUAGES C CXX)


add_subdirectory(submodules/hidapi)
include_directories(submodules/hidapi/hidapi)

add_library(${PROJECT_NAME}
	naraapi_hl.cpp
	naraapi_ll.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE hidapi::hidapi)