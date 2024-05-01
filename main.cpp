#include <iostream>
#include <thread>

#include "wman.cpp"
#include "header.h"

bool running = true;

int main()
{
	if (!Init(&running)) return 1;
	
	LoadAllGroups();

	while (!glfwWindowShouldClose(window) && running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
		Input();
		RenderWindow();
	}

	Shutdown();

	return 0;
}
