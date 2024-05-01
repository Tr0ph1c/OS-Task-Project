#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw3.h"

#include "GUI.cpp"

#define WIDTH 1000
#define HEIGHT 800

GLFWwindow* window;

bool* running_ptr;

void error_callback (int error, const char* description) {
    fprintf(stderr, "stderr Error: %s\n", description);
}

int Init (bool* running) {
	running_ptr = running;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {std::cout << "GLFW init failed"; return 0;}
	const GLFWvidmode * videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Group/User Manager GUI", NULL, NULL);
	glfwSetWindowPos(window,
                 (videoMode->width - WIDTH) / 2,
                 (videoMode->height - HEIGHT) / 2); 
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

void RenderWindow () {
	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({WIDTH, HEIGHT});
		
	RenderGUI(running_ptr);

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
