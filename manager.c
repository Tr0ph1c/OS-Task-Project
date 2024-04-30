#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void toLowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isupper(str[i])) {
            str[i] = tolower(str[i]);
        }
    }
}

// Check if the group is exist or not
bool isGroupExist(char *groupName)
{
	FILE *file;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	file = fopen("/etc/group", "r");
	if (file == NULL)
	{
		perror("Failed to open /etc/group");
		return false;
	}

	while ((read = getline(&line, &len, file)) != -1)
	{
		char *token;
		token = strtok(line, ":");
		if (token != NULL && strcmp(token, groupName) == 0)
		{
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
bool addGroup(char *groupName)
{
	if (!isGroupExist(groupName))
	{
		char command[265] = "sudo groupadd ";
		strcat(command, groupName);
		int status = system(command);
		if (status == -1)
		{
			perror("Faild to add group\n");
			return false;
		}
		else
		{
			printf("Group is added\n");
			return true;
		}
	}
	else
	{
		printf("This group's name is already exist\n");
		return false;
	}
}

// Delete group
void deleteGroup(char *groupName)
{
	if (isGroupExist(groupName))
	{
		char command[265] = "sudo groupdel ";
		strcat(command, groupName);
		int status = system(command);
		if (status == -1)
		{
			perror("Faild to add group\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Group is deleted\n");
		}
	}
	else
	{
		printf("There is NO group with this name\n");
	}
}

void addUser(char *username, char *groupname) {
	char command[200] = "sudo usermod -aG ";
	strcat(command, groupname);
	strcat(command, " ");
	strcat(command, username);

	system(command);
	printf("User Added\n");
}

void deleteUser(char *username, char *groupname) {
	char command[200] = "sudo gpasswd -d ";
	strcat(command, username);
	strcat(command, " ");
	strcat(command, groupname);

	system(command);
	printf("User Removed\n");
}

// Add new user / delete existing user

void addNewUser(char *username, char *comment, char *expiry) {
    char command[50];
    sprintf(command, "sudo useradd -m -c \"%s\" -e \"%s\" %s", comment, expiry, username);
    system(command);
}

void remUser(char *username) {
    char command[50]; 
    sprintf(command, "sudo deluser %s", username);
    system(command);
}

// Change user info

void changeUsername (char *user, char *newName) {
    char command[100];
    sprintf(command, "sudo usermod -l %s %s", newName, user);
    system(command);
}

void changeUserPassword (char *user, char *newPass) {
    char command[100];
    sprintf(command, "echo \"%s:%s\" | sudo chpasswd", user, newPass);
    system(command);
}

void changeUserTip (char* username, char* newTip) {
	char command[100];
	sprintf(command, "sudo usermod -c '%s' %s", newTip, username);
	system(command);
}

void changeUserExpiry (char* username, char* newExp) {
	char command[100];
	sprintf(command, "sudo chage -E %s %s", newExp, username);
	system(command);
}
