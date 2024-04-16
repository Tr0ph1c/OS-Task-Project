#include <iostream>
#include "glfw3.h"

#define WIDTH 1000
#define HEIGHT 800

GLFWwindow* window;

void error_callback (int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int Init () {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {std::cout << "GLFW init failed"; return 0;}
    window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);
    if (!window) {std::cout << "Window failed to create"; return 0;}
    glfwMakeContextCurrent(window);
    return 1;
}

void Input () {
	glfwPollEvents();
	
	// Initial input binding to test inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

void Render () {
	glClearColor(0.2f, 0.2f, 0.2f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //render

    glfwSwapBuffers(window);
}

void Shutdown () {
	glfwDestroyWindow(window);
    glfwTerminate();
}