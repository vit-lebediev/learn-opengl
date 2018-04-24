#include <GLFW/glfw3.h>

void main () {

    if (!glfwInit())
    {
        // Initialization failed
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window) 
    {
        // Window or OpenGL context creation failed
    }

    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        // Keep running
    }

    int widht, height;

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

    


