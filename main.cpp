#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "wman.cpp"
#include "header.h"

const char* GROUPS_DIRECTORY = "./groups/";

int main () {
	if (!Init()) return 1;
	printf("Welcome to User Manager System.");

	LoadAllGroups();
	
	while (!glfwWindowShouldClose(window)) {
		Input();
		Render();
	}
	
	Shutdown();
	
	return 0;
}

// Loads all groups from disk to memory in current session
void LoadAllGroups() {
	return;
}



// Check if the group is exist or not
bool isGroupExist(char *groupName){
  FILE *file;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  
  file = fopen("/etc/group","r");
  if(file == NULL){
    perror("Failed to open /etc/group");
    return false;
  }
  
  while((read = getline(&line,&len,file)) != -1){
    char *token;
    token = strtok(line,":");
    if(token != NULL && strcmp(token , groupName) == 0){
      free(line);
      fclose(file);
      return true;
    }
  }
  free(line);
  fclose(file);
  return false;
}




// Add group
void addGroup(char *groupName){
  if(!isGroupExist(groupName)){
    char command[265] = "sudo groupadd ";
    strcat(command,groupName);
    int status = system(command);
    if(status == -1){
      perror("Faild to add group\n");
      exit(EXIT_FAILURE);
    }
    else{
      printf("Group is added\n");
    }
  }
  else{
    printf("This group's name is already exist\n");
  }
  

}




// Delete group
void deleteGroup(char *groupName){
  if(isGroupExist(groupName)){
    char command[265] = "sudo groupdel ";
    strcat(command,groupName);
    int status = system(command);
    if(status == -1){
      perror("Faild to add group\n");
      exit(EXIT_FAILURE);
    }
    else{
      printf("Group is deleted\n");
    }
  }
  else{
    printf("There is NO group with this name\n");
  }
}