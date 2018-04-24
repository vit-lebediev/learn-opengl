cmake_minimum_required(VERSION 3.11.1)

set (CMAKE_CXX_STANDARD 11)

project(learn_opengl)

# Include GLFW
add_subdirectory("lib/glfw")
include_directories("lib/glfw/include")

find_library(GLEW GLEW)
if (NOT GLEW)
    message(FATAL_ERROR "GLEW Not found")
endif()

find_library(COCOA Cocoa)
if (NOT COCOA)
    message(FATAL_ERROR "Cocoa Not found")
endif()

find_library(OPEN_GL OpenGL)
if (NOT OPEN_GL)
    message(FATAL_ERROR "OpenGL Not found")
endif()

find_library(IO_KIT IOKit)
if (NOT IO_KIT)
    message(FATAL_ERROR "IOKit Not found")
endif()

find_library(CORE_VIDEO CoreVideo)
if (NOT CORE_VIDEO)
    message(FATAL_ERROR "CoreVideo Not found")
endif()

# Bring ONLY the headers 
# include_directories(include)

# Can manually add the sources using the set command as follows:
# set(SOURCES src/mainapp.cpp src/Student.cpp)

# However, the file(GLOB...) allows for wildcard additions:
# Possible option: GLOB_RECURSE
file(GLOB SOURCES "src/*.cpp")
add_executable (triangle ${SOURCES})

target_link_libraries(triangle glfw ${GLFW_LIBRARIES} ${GLEW} ${COCOA} ${OPEN_GL} ${IO_KIT} ${CORE_VIDEO})

# find_library(GLFW glfw3)
# if (NOT GLWF)
#     message(FATAL_ERROR "GLFW Not found")
# endif()
