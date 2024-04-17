#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw3.h"

#include "GUI.cpp"

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
    
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
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
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	RenderGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	glfwSwapBuffers(window);
}

void Shutdown () {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
   	glfwTerminate();
}
