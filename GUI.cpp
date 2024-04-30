#include <string>

#include "imgui.h"
#include "structs.cpp"
#include "manager.c"

#define SPACE 150
#define GAP Dummy(ImVec2(0.0f, 20.0f));
#define MAX_INP 50
#define FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings

using namespace std;
using namespace ImGui;

enum wind {MANAGER, GROUPMAN, USERMAN};

//  GUI VAIRABLES
wind wind_state = MANAGER;

Group* selected_group;
User* selected_user;

char selected_user_tip[MAX_INP] = {0};
char selected_user_exp[11] = {0};

char user_name[MAX_INP] = {0};
char user_tip[MAX_INP] = {0};
char user_expiry[11] = {0};
char group_name[MAX_INP] = {0};
char ch_user_name[MAX_INP] = {0};
char ch_user_pword[MAX_INP] = {0};
char ch_user_tip[MAX_INP] = {0};
char ch_user_exp[11] = {0};
bool adding_to_group = false;
//  END GUI VARIABLES
//====================================//
// FUNCTIONALITY VARIABLES
Group groups[100] = {0};
size_t groups_size = 0;
User users[100] = {0};
size_t users_size = 0;
char new_line[] = "\n";
// END FUNCTIONALITY VARIABLES

void ClearBuffer(char* buffer, size_t max_size) {
	for (size_t i = 0; i < max_size; ++i) {
		buffer[i] = '\0';
	}
}

void FixBuffer(char* buffer, size_t max_size) {
	for (size_t i = 0; i < max_size; ++i) {
		if (buffer[i] == '\n') buffer[i] = '\0';
		if (buffer[i] == ' ') buffer[i] = '-';
	}
}

void LoadAllGroups() {
	// Init vars
	selected_group = nullptr;
	selected_user = nullptr;
	groups_size = 0;
	users_size = 0;
	ClearBuffer(selected_user_tip, MAX_INP);
	ClearBuffer(selected_user_exp, 11);
	ClearBuffer(user_name, MAX_INP);
	ClearBuffer(user_tip, MAX_INP);
	ClearBuffer(user_expiry, 11);
	ClearBuffer(group_name, MAX_INP);
	ClearBuffer(ch_user_name, MAX_INP);
	ClearBuffer(ch_user_pword, MAX_INP);
	ClearBuffer(ch_user_tip, MAX_INP);
	ClearBuffer(ch_user_exp, 11);
	adding_to_group = false;
	
	// Declare file and line
	FILE* fout;
	char line[50] = {0};
	
	//  LOAD GROUPS
	
	fout = (FILE*)popen("getent group | cut -d: -f1", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout) != NULL; ++i, ++groups_size) {
		FixBuffer(line, MAX_INP);
		strcpy(groups[i].groupname, line);
		groups[i].numberofusersingroup = 0;
	}
	
	pclose(fout);
	
	//  LOAD USERS INSIDE OF GROUPS
	
	fout = (FILE*)popen("getent group | cut -d: -f4", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout) != NULL; ++i) {
		FixBuffer(line, MAX_INP);
		if (!(*line)) continue;
		char* usertok;
		char* prevtok = new_line;
		for (int j = 0; ; ++j) {
			usertok = strtok(line, ",");
			if (strcmp(usertok, prevtok) == 0) break;
			strcpy(groups[i].usersingroup[j], usertok);
			groups[i].numberofusersingroup += 1;
			prevtok = usertok;
		}
	}
	
	pclose(fout);
	
	//  LOAD USERS
	
	fout = (FILE*)popen("getent passwd | cut -d: -f1", "r");
	
	for (int i = 0; fgets(line, sizeof(line), fout) != NULL; ++i, ++users_size) {
		FixBuffer(line, MAX_INP);
		strcpy(users[i].username, line);
	}
	
	pclose(fout);
}

void RenderGUI(bool* running) {
	switch (wind_state) {
	//
	//   ---------------------------  MANAGER MENU
	//
	case MANAGER:
	{
	Begin("Manager", running, FLAGS);
	
	if (Button("User Manager")) wind_state = USERMAN;
	SameLine();
	if (Button("Group Manager")) wind_state = GROUPMAN;
	
	Separator();
	
	BeginChild("##Groups", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y));
	Text("All Groups");
	
	GAP
	
	for (size_t i = 0; i < groups_size; ++i) {
		Text(groups[i].groupname);
		SameLine(SPACE);
		PushID(i);
		if (Button("select")) selected_group = &groups[i];
		PopID();
	}
	EndChild();
	
	SameLine();
	
	BeginChild("##UsersInGroup", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y));
	if (selected_group) {
		Text("Users In [%s]", selected_group->groupname);
		
		GAP
		
		for (size_t i = 0; selected_group && i < selected_group->numberofusersingroup; ++i) {
			Text("%s", selected_group->usersingroup[i]);
			SameLine(SPACE);
			PushID(i);
			if (Button("remove")) {
				deleteUser(selected_group->usersingroup[i], selected_group->groupname);
				LoadAllGroups();
			}
			PopID();
		}
	}
	EndChild();
	
	End();
	break;
	}
	//
	//   ---------------------------  GROUP MANAGER MENU
	//
	case GROUPMAN:
	{
	Begin("Manage Groups", running, FLAGS);
	
	if(Button("Back")) wind_state = MANAGER;
	
	Separator();
	
	BeginChild("##Groups", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y / 2.0f));
	Text("All groups on system");
	
	GAP
	
	for (size_t i = 0; i < groups_size; ++i) {
		Text(groups[i].groupname);
		SameLine(SPACE);
		PushID(i);
		if (Button("delete")) {
			deleteGroup(groups[i].groupname);
			LoadAllGroups();
		}
		PopID();
	}
	EndChild();
	
	Separator();
	
	GAP
	
	Text("Create new group");
	PushItemWidth(300);
	InputText("group name", group_name, MAX_INP);
	PopItemWidth();
	if (Button("Create Group")) {
		if (*group_name) {
			FixBuffer(group_name, MAX_INP);
			printf("%d", addGroup(group_name));
			LoadAllGroups();
		}
	}
	
	End();
	break;
	}
	//
	//   ---------------------------  USER MANAGER MENU
	//
	case USERMAN:
	{
	Begin("Manage Users", running, FLAGS);
	
	if(Button("Back")) wind_state = MANAGER;
	
	Separator();
	
	BeginChild("##AllUsers", ImVec2(GetContentRegionAvail().x / 2.0f, GetContentRegionAvail().y - 200));
	Text("All users on system");
	
	GAP
	
	for (size_t i = 0; i < users_size; ++i) {
		Text(users[i].username);
		SameLine(SPACE);
		PushID(i);
		if (Button("select")) {
			FILE* fpipe;
			char sysc[150] = {0};
			selected_user = &users[i];
			sprintf(sysc, "getent passwd %s | cut -d ':' -f 5", selected_user->username);
			fpipe = (FILE*)popen(sysc, "r"); fgets(selected_user_tip, MAX_INP, fpipe); pclose(fpipe);
			FixBuffer(selected_user_tip, MAX_INP);
			sprintf(sysc, "chage -i -l %s | grep 'Account expires' | cut -d ':' -f 2", selected_user->username);
			fpipe = (FILE*)popen(sysc, "r"); fgets(selected_user_exp, 11, fpipe); pclose(fpipe);
			FixBuffer(selected_user_exp, 11);
		}
		PopID();
	}
	EndChild();
	
	SameLine();
	
	BeginChild("##UserInfo", ImVec2(GetContentRegionAvail().x, GetContentRegionAvail().y - 200));
	if (selected_user) {
		Text("CURRENT USER NAME: [%s]", selected_user->username);
		Text("CURRENT USER TIP: [%s]", selected_user_tip);
		Text("CURRENT USER EXPIRY: [%s]", selected_user_exp);
		
		GAP
		
		Text("New username:");
		SameLine(SPACE);
		PushItemWidth(200);
		InputText("##ch_un", ch_user_name, MAX_INP);
		PopItemWidth();
		if (Button("Update Username")) {
			if (*ch_user_name) {
				FixBuffer(ch_user_name, MAX_INP);
				changeUsername(selected_user->username, ch_user_name);
				LoadAllGroups();
			}
		}
		
		GAP
		
		Text("New password:");
		SameLine(SPACE);
		PushItemWidth(200);
		InputText("##ch_upwd", ch_user_pword, MAX_INP);
		PopItemWidth();
		if (Button("Update Password")) {
			FixBuffer(ch_user_pword, MAX_INP);
			changeUserPassword(selected_user->username, ch_user_pword);
			LoadAllGroups();
		}
		
		GAP
		
		Text("New tip:");
		SameLine(SPACE);
		PushItemWidth(200);
		InputText("##ch_ut", ch_user_tip, MAX_INP);
		PopItemWidth();
		if (Button("Update Tip")) {
			FixBuffer(ch_user_tip, MAX_INP);
			changeUserTip(selected_user->username, ch_user_tip);
			LoadAllGroups();
		}
		
		GAP
		
		Text("New expiry:");
		SameLine(SPACE);
		PushItemWidth(200);
		InputText("YYYY-MM-DD", ch_user_exp, 11);
		PopItemWidth();
		if (Button("Update Expiry")) {
			FixBuffer(ch_user_exp, 11);
			changeUserExpiry(selected_user->username, ch_user_exp);
			LoadAllGroups();
		}
		
		GAP
		
		if (Button("Add To Group...")) {
			adding_to_group = true;
		}
		
		if (Button("Delete User")) {
			remUser(selected_user->username);
			LoadAllGroups();
		}
		
		BeginChild("##AddUserToGroup", ImVec2(GetContentRegionAvail().x, GetContentRegionAvail().y));
		if (adding_to_group) {
			Text("All groups on system");
		
			GAP
			
			for (size_t i = 0; i < groups_size; ++i) {
				Text(groups[i].groupname);
				SameLine(SPACE);
				PushID(i);
				if (Button("add here")) {
					addUser(selected_user->username, groups[i].groupname);
					LoadAllGroups();
				}
				PopID();
			}
		}
		EndChild();
	}	
	EndChild();
	
	Separator();
	
	GAP
	
	Text("Create new user");
	
	PushItemWidth(300);
	InputText("username", user_name, MAX_INP);
	PopItemWidth();
	
	PushItemWidth(300);
	InputText("user tip", user_tip, MAX_INP);
	PopItemWidth();
	
	Text("Expiry Date Format: YYYY-MM-DD");
	Text("Leave empty for no expiry date.");
	PushItemWidth(300);
	InputText("user expiry", user_expiry, MAX_INP);
	PopItemWidth();
	
	if (Button("Create User")) {
		if (*user_name) {
			FixBuffer(user_name, MAX_INP);
			addNewUser(user_name, user_tip, user_expiry);
			LoadAllGroups();
		}
	}
	
	End();
	break;
	}
	}
}
