#include <stdio.h>
#include <stdlib.h>

struct user_type {
	char name[64];
    char password[64];
    int age;
};

struct group_type {
    size_t size;
    size_t capacity;
	struct user_type* users;
};

struct group_type* CreateGroup(size_t capacity) {
	struct group_type* g = malloc(sizeof(struct group_type));

	g->size = 0;
	g->capacity = capacity;
	g->users = malloc(sizeof(struct user_type) * capacity);

    return g;
}

void DestroyGroup(struct group_type* g) {
    free(g->users);
    free(g);
} 