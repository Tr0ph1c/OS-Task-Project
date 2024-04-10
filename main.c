#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "header.h"

const char* GROUPS_DIRECTORY = "./groups/";

int main () {
	printf("Welcome to User Manager System.");

	LoadAllGroups();
	
	return 0;
}

// Loads all groups from disk to memory in current session
void LoadAllGroups() {
	return;
}