#include <iostream>

#include "wman.cpp"
#include "manager.c"
#include "header.h"

const char* groups = "/etc/groups/";

int main()
{
	if (!Init())
		return 1;
	printf("Welcome to User Manager System.");

	LoadAllGroups();

	while (!glfwWindowShouldClose(window))
	{
		Input();
		Render();
	}

	Shutdown();

	return 0;
}

// Loads all groups from disk to memory in current session
void LoadAllGroups()
{
	return;
}
