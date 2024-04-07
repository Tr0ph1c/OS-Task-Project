#pragma once

typedef struct group_type group;
typedef struct user_type user;

group CreateGroup(size_t capacity);
void DestroyGroup(group g);