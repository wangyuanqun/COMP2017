#ifndef svc_h
#define svc_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define ID_LEN (7)
#define BRANCH_NAME (51)
#define FILE_PATH (261)
#define LEN_MESS (61)

typedef struct file {
    char file_name[FILE_PATH];

    int prev_hash;
    int current_hash;
} file;

typedef struct commit {
    char commit_id[ID_LEN];
    char message[LEN_MESS];

    file* add_file;
    int num_add;

    file* rm_file;
    int num_rm;

    file* modi_file;
    int num_modi;

    file* tracked_files;
    int num_tracked;

} commit;

typedef struct branch {
    char branch_name[BRANCH_NAME];
    commit* comit_array;
    int num_comit;

    file* temp_track;
    int n_temp_track;
} branch;

typedef struct help {
    char head[BRANCH_NAME];
    branch* branch_array;
    int num_branch;
} help;

typedef struct resolution {
    // NOTE: DO NOT MODIFY THIS STRUCT
    char *file_name;
    char *resolved_file;
} resolution;


void *svc_init(void);

void cleanup(void *helper);

int hash_file(void *helper, char *file_path);

char *svc_commit(void *helper, char *message);

void *get_commit(void *helper, char *commit_id);

char **get_prev_commits(void *helper, void *commit, int *n_prev);

void print_commit(void *helper, char *commit_id);

int svc_branch(void *helper, char *branch_name);

int svc_checkout(void *helper, char *branch_name);

char **list_branches(void *helper, int *n_branches);

int svc_add(void *helper, char *file_name);

int svc_rm(void *helper, char *file_name);

int svc_reset(void *helper, char *commit_id);

char *svc_merge(void *helper, char *branch_name, resolution *resolutions, int n_resolutions);

#endif
