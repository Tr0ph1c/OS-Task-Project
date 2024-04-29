typedef struct s_user {
	char username[50];
} User;

typedef struct s_group {
	char groupname[50];
	char usersingroup[50][50];
	size_t numberofusersingroup;
} Group;
