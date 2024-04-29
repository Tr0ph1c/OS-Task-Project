#include <string>

#include "imgui.h"
#include "structs.cpp"
#include "manager.c"

#define SPACE 150
#define MAX_INP 50
#define FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings

using namespace std;
using namespace ImGui;

enum wind {MANAGER, GROUPMAN, USERMAN};

//  GUI VAIRABLES
wind wind_state = MANAGER;

string selected_group = "";

char user_name[MAX_INP] = {0};
char group_name[MAX_INP] = {0};
//  END GUI VARIABLES
//===========================//
// FUNCTIONALITY VARIABLES
Group groups[100] = {0};
size_t groups_size = 0;
User users[100] = {0};
size_t users_size = 0;
// END FUNCTIONALITY VARIABLES

void LoadAllGroups() {
	// Init vars
	groups_size = 0;
	users_size = 0;

	FILE* fout;
	char line[50];
	
	//  LOAD GROUPS
	
	fout = (FILE*)popen("getent group | cut -d: -f1", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout); ++i, ++groups_size) {
		strcpy(groups[i].groupname, line);
		groups[i].numberofusersingroup = 0;
	}
	
	pclose(fout);
	
	//  LOAD USERS INSIDE OF GROUPS
	
	fout = (FILE*)popen("getent group | cut -d: -f4", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout); ++i, ++groups_size) {
		/*char* usertok;
		usertok = strtok(line, ",");
		for (int j = 0; usertok; ++j) {
			usertok = strtok(line, ",");
			strcpy(groups[i].usersingroup[j], usertok);
			groups[i].numberofusersingroup += 1;
		}*/
	}
	
	pclose(fout);
	
	//  LOAD USERS
	
	fout = (FILE*)popen("getent passwd | cut -d: -f1", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout); ++i, ++users_size) {
		strcpy(users[i].username, line);
	}
	
	pclose(fout);
}

void RenderGUI(bool* running) {
	switch (wind_state) {
	case MANAGER:
	{
	Begin("Manager", running, FLAGS);
	
	if (Button("User Manager")) wind_state = USERMAN;
	SameLine();
	if (Button("Group Manager")) wind_state = GROUPMAN;
	
	Separator();
	
	BeginChild("##Groups", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y));
	Text("All Groups");
	for (size_t i = 0; i < groups_size; ++i) {
		Text(groups[i].groupname);
		SameLine(SPACE);
		PushID(i);
		if (Button("select")) selected_group = groups[i].groupname;
		PopID();
	}
	EndChild();
	
	SameLine();
	
	BeginChild("##UsersInGroup", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y));
	Text("Users In Selected Group");
	for (int i = 0; i < 10; ++i) {
		Text("%s user %d", selected_group.c_str(), i);
		SameLine(SPACE);
		PushID(i);
		if (Button("remove")) std::cout << "clicked";
		PopID();
	}
	EndChild();
	
	End();
	break;
	}
	case GROUPMAN:
	{
	Begin("Manage Groups", running, FLAGS);
	
	if(Button("Back")) wind_state = MANAGER;
	
	Separator();
	
	BeginChild("##Groups", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y / 2.0f));
	Text("All groups on system");
	for (size_t i = 0; i < groups_size; ++i) {
		Text(groups[i].groupname);
		SameLine(SPACE);
		PushID(i);
		if (Button("delete")) std::cout << "clicked";
		PopID();
	}
	EndChild();
	
	Text("Create new group");
	PushItemWidth(300);
	InputText("group name", group_name, MAX_INP);
	PopItemWidth();
	SameLine();
	if (Button("Create Group")) cout << "added user";
	
	End();
	break;
	}
	case USERMAN:
	{
	Begin("Manage Users", running, FLAGS);
	
	if(Button("Back")) wind_state = MANAGER;
	
	Separator();
	
	BeginChild("##AllUsers", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y / 2.0f));
	Text("All users on system");
	for (size_t i = 0; i < users_size; ++i) {
		Text(users[i].username);
		SameLine(SPACE);
		PushID(i);
		if (Button("delete")) std::cout << "clicked";
		PopID();
	}
	EndChild();
	
	Text("Create new user");
	InputText("username", user_name, MAX_INP);
	SameLine();
	if (Button("Create User")) printf("\n%s", user_name);
	
	End();
	break;
	}
	}
}
