#include "svc.h"
#include "util.h"

void* svc_init(void) {
    help* helper = malloc(sizeof(help));
    strcpy(helper->head, "master");
    helper->branch_array = NULL;
    helper->num_branch = 0;

    return helper;
}

void cleanup(void* helper) {
    // TODO: Implement
    help* h = helper;
	for(int i = 0; i < h->num_branch; i++) {
		if(h->branch_array[i].temp_track) {
			free(h->branch_array[i].temp_track);
		}
		if(h->branch_array[i].comit_array) {
			for(int j = 0; j < h->branch_array[i].num_comit; j++) {
				if(h->branch_array[i].comit_array[j].add_file) {
					free(h->branch_array[i].comit_array[j].add_file);
				}
				if(h->branch_array[i].comit_array[j].rm_file) {
					free(h->branch_array[i].comit_array[j].rm_file);
				}
				if(h->branch_array[i].comit_array[j].modi_file) {
					free(h->branch_array[i].comit_array[j].modi_file);
				}
				if(h->branch_array[i].comit_array[j].tracked_files) {
					free(h->branch_array[i].comit_array[j].tracked_files);
				}
			}
		}
		free(h->branch_array[i].comit_array);
	}
	free(h->branch_array);
	free(h);
}

int hash_file(void* helper, char* file_path) {
    // TODO: Implement
    if(!(file_path)) {
        //path is NULL
        return -1;
    }
    FILE* file;
    int hash = 0;
    if ((file = fopen(file_path, "r"))) {
        int strl = strlen(file_path);
        //hash of file_name
        for(int i = 0; i < strl; i++) {
    		hash = (hash + file_path[i]) % 1000;
    	}
        //hash of file content
        while(1) {
            int c = fgetc(file);
    	    if(feof(file)) {
    	       break;
    	   	}
    		hash = (hash + c) % 2000000000;
        }
    } else {
        //invalid path
        return -2;
    }
    fclose(file);
    return hash;
}

char* svc_commit(void* helper, char* message) {
    // TODO: Implement
    if(!(message)) {
        return NULL;
    }
	help* h = helper;
	if(!(h->branch_array)) {
		return NULL;
	}

    int i = find_index_branch(h, &i, h->head);
	if(h->branch_array[i].comit_array) {
		//check if modifications
		//0 means no change
		int check = check_commit_changes(helper, h->branch_array[i]);

		if(check == 0) {
			return NULL;
		} else {
			init_commit(&h->branch_array[i], message);
			for(int j = 0; j < h->branch_array[i].n_temp_track; j++) {
				int match = 0;
				int index = 0;
				for(int k = 0; k < h->branch_array[i]
					.comit_array[h->branch_array[i].num_comit-2]
					.num_tracked; k++) {

						if(strcmp(h->branch_array[i].temp_track[j].file_name,
							h->branch_array[i]
							.comit_array[h->branch_array[i].num_comit-2]
							.tracked_files[k].file_name) == 0) {
								match = 1;
								//if file_name is same, check content
								FILE* f;
								if((f = fopen(h->branch_array[i].temp_track[j].
									file_name, "r"))) {

									int pre_hash = h->branch_array[i]
										.temp_track[j].current_hash;

									int cur_hash = hash_file(helper,
										h->branch_array[i].temp_track[j]
										.file_name);

									if(pre_hash != cur_hash) {
										//modifi exists
										h->branch_array[i]
										.comit_array[h->branch_array[i]
										.num_comit-1].num_modi += 1;

										int num = h->branch_array[i]
										.comit_array[h->branch_array[i]
										.num_comit-1].num_modi;

										if(num == 1) {

                                            h->branch_array[i]
    										.comit_array[h->branch_array[i]
    										.num_comit-1].modi_file =
                                            malloc(sizeof(file)*(num));

											memcpy(&h->branch_array[i]
    										.comit_array[h->branch_array[i]
    										.num_comit-1].modi_file[num-1],
											&h->branch_array[i]
											.temp_track[j], sizeof(file));

										} else {
                                            h->branch_array[i]
    										.comit_array[h->branch_array[i]
    										.num_comit-1].modi_file =
                                            realloc(h->branch_array[i]
    										.comit_array[h->branch_array[i]
    										.num_comit-1].modi_file,
                                            sizeof(file)*(num));

											memcpy(&h->branch_array[i]
    										.comit_array[h->branch_array[i]
    										.num_comit-1].modi_file[num-1],
											&h->branch_array[i].temp_track[j],
                                            sizeof(file));
										}

									}

								} else {
									//can not open, delete locally
									h->branch_array[i]
									.comit_array[h->branch_array[i]
									.num_comit-1].num_rm += 1;

									int num = h->branch_array[i]
									.comit_array[h->branch_array[i]
									.num_comit-1].num_rm;


									if(num == 1) {
                                        h->branch_array[i]
    									.comit_array[h->branch_array[i]
    									.num_comit-1].rm_file =
                                        malloc(sizeof(file)*(num));

										memcpy(&h->branch_array[i]
    									.comit_array[h->branch_array[i]
    									.num_comit-1].rm_file[num-1],
											&h->branch_array[i]
											.temp_track[j], sizeof(file));
									} else {
                                        h->branch_array[i]
    									.comit_array[h->branch_array[i]
    									.num_comit-1].rm_file =
                                        malloc(sizeof(file)*(num));

										memcpy(&h->branch_array[i]
    									.comit_array[h->branch_array[i]
    									.num_comit-1].rm_file[num-1],
											&h->branch_array[i]
											.temp_track[j], sizeof(file));
									}
								}
							} else {
								index = j;
							}
					}
				// same file name not found
				if(match == 0) {
					h->branch_array[i]
					.comit_array[h->branch_array[i]
					.num_comit-1].num_add += 1;

					int num = h->branch_array[i]
					.comit_array[h->branch_array[i]
					.num_comit-1].num_add;

					if(num == 1) {
                        h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].add_file = malloc(sizeof(file)*(num));

						memcpy(&h->branch_array[i]
    					    .comit_array[h->branch_array[i]
    					    .num_comit-1].add_file[num-1],
							&h->branch_array[i]
							.temp_track[j], sizeof(file));
					} else {
                        h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].add_file = realloc(h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].add_file, sizeof(file)*(num));

						memcpy(&h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].add_file[num-1],
							&h->branch_array[i].temp_track[index],
							sizeof(file));
					}
				}

			}
		}

		for(int k = 0; k < h->branch_array[i].comit_array[h->branch_array[i]
			.num_comit-2].num_tracked; k++) {
				int match = 0;
				int index = 0;
				for(int j = 0; j < h->branch_array[i].n_temp_track; j++) {
					if(strcmp(h->branch_array[i].temp_track[j].file_name,
						h->branch_array[i]
						.comit_array[h->branch_array[i].num_comit-2]
						.tracked_files[k].file_name) == 0) {
							match = 1;
						} else {
							index = j;
						}
				}
				// same file name not found
				if(match == 0) {
					h->branch_array[i]
					.comit_array[h->branch_array[i]
					.num_comit-1].num_rm += 1;

					int num = h->branch_array[i]
					.comit_array[h->branch_array[i]
					.num_comit-1].num_rm;

					if(num == 1) {
                        h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].rm_file = malloc(sizeof(file)*num);
						memcpy(&h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].rm_file[num-1],
							&h->branch_array[i].temp_track[index],
							sizeof(file));
					} else {
                        h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].rm_file = realloc(h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].rm_file, sizeof(file)*(num));

						memcpy(&h->branch_array[i]
    					.comit_array[h->branch_array[i]
    					.num_comit-1].rm_file[num-1],
							&h->branch_array[i].temp_track[index],
							sizeof(file));
					}
				}
			}

		char commit_id[ID_LEN];
		int index = h->branch_array[i].num_comit;
        get_commit_id(h->branch_array[i].comit_array[index-1],
			commit_id, message);
        strcpy(h->branch_array[i].comit_array[index-1].commit_id, commit_id);

		// sacve copy of file;
        if(h->branch_array[i].comit_array) {

			for(int j = 0; j < h->branch_array[i]
				.comit_array[0].num_tracked; j++) {

					save_file(commit_id, h->branch_array[i]
						.comit_array[0].tracked_files[j].file_name);
			}
		}


	} else {
		initial_comit_array(&h->branch_array[i], message);

        char commit_id[ID_LEN];
        get_commit_id(h->branch_array[i].comit_array[0], commit_id, message);
        strcpy(h->branch_array[i].comit_array[0].commit_id, commit_id);
        // sacve copy of file;

		if(h->branch_array[i].comit_array) {

			for(int j = 0; j < h->branch_array[i]
				.comit_array[0].num_tracked; j++) {

					save_file(commit_id, h->branch_array[i]
						.comit_array[0].tracked_files[j].file_name);
			}
		}

	}

    return h->branch_array[i].comit_array[0].commit_id;
}

void* get_commit(void* helper, char* commit_id) {
    // TODO: Implement
	if(commit_id == NULL) {
		return NULL;
	}
	help* h = helper;
	if(h->branch_array) {
		for(int i = 0; i < h->num_branch; i++) {
			if(h->branch_array[i].comit_array) {
				for(int j = 0; j < h->branch_array[i].num_comit; j++) {
					if(strcmp(h->branch_array[i].comit_array[j].commit_id,
						commit_id) == 0) {
							return &h->branch_array[i].comit_array[j];
						}
				}
			}
		}
	}

	return NULL;
}

char** get_prev_commits(void* helper, void* commit, int* n_prev) {
	if(n_prev == NULL) {
		return NULL;
	}
	if(commit == NULL) {
		*n_prev = 0;
		return NULL;
	}
	help* h = helper;
	char* co = commit;

	char** pre_co = malloc(sizeof(char*));
	*n_prev = 0;

	int match = 0;
	if(h->branch_array) {
		for(int i = 0; i < h->num_branch; i++) {
			if(h->branch_array[i].comit_array) {
				int j;
				for(j = 0; j < h->branch_array[i].num_comit; j++) {
					if(strcmp(h->branch_array[i].comit_array[j].commit_id,
						co) == 0) {
							if(j == 0) {
								*n_prev = 0;
								free(pre_co);
								return NULL;
							}
							match = 1;
							break;
						}
				}
				if(match == 1) {
					for(int k = 0; k < j; k++) {
						*n_prev += 1;
						pre_co = realloc(pre_co, sizeof(char*)*(*n_prev));
						pre_co[(*n_prev)-1] =
							h->branch_array[i].comit_array[j].commit_id;
					}
				}

			}
		}
	} else {
		*n_prev = 0;
		free(pre_co);
		return NULL;
	}
	if(match == 0) {
		*n_prev = 0;
		free(pre_co);
		return NULL;
	}
    return pre_co;
}

void print_commit(void* helper, char* commit_id) {
    // TODO: Implement
    help* h = helper;
	if(!(commit_id)) {
		puts("Invalid commit id");
		return;
	}
	if(!(h->num_branch)) {
		puts("Invalid commit id");
		return;
	}
	int match = 0;
    for(int i = 0; i < h->num_branch; i++) {
		if(h->branch_array[i].comit_array) {
			for(int j = 0; j < h->branch_array[i].num_comit; j++) {
				if(strcmp(h->branch_array[i].comit_array[j].commit_id,
					commit_id) == 0) {
						match = 1;
						printf("%s ", commit_id);
						printf("[%s]: ", h->branch_array[i].branch_name);
						printf("%s\n", h->branch_array[i]
							.comit_array[j].message);

						for(int k = 0;k < h->branch_array[i]
							.comit_array[j].num_add; k++) {
								printf("    + %s\n", h->branch_array[i]
									.comit_array[j].add_file[k].file_name);
							}

						for(int k = 0; k < h->branch_array[i]
							.comit_array[j].num_rm; k++) {
								printf("    + %s\n", h->branch_array[i]
									.comit_array[j].rm_file[k].file_name);
							}

						for(int k = 0; k < h->branch_array[i]
							.comit_array[j].num_modi; k++) {
								printf("    + %s\n", h->branch_array[i]
								.comit_array[j].tracked_files[k].file_name);
							}

						puts("");
						printf("    Tracked files (%d):\n", h->branch_array[i]
							.comit_array[j].num_tracked);
						for(int k = 0; k < h->branch_array[i]
							.comit_array[j].num_tracked; k++) {
								printf("    [");
								printf("%10d] %s\n", h->branch_array[i]
								.comit_array[j].tracked_files[k].current_hash,
								h->branch_array[i].comit_array[j]
								.tracked_files[k].file_name);
							}
					}
			}
		}
    }

	if(match == 0) {
		puts("Invalid commit id");
		return;
	}

}

int svc_branch(void* helper, char* branch_name) {
	//check if valid
	if(branch_name == NULL || (check_valid(branch_name) == 0)) {
		return -1;
	}
	help* h = helper;
	if(h->branch_array) {
		//check if name exists
		for(int i = 0; i < h->num_branch; i++) {
			if(strcmp(h->branch_array[i].branch_name, branch_name) == 0) {
				return -2;
			}
			int check = check_commit_changes(helper, h->branch_array[i]);
			if(check != 0) {
				return -3;
			}
		}
	}

	// create branch
    int index_cur_bran = find_index_branch(h, &index_cur_bran, h->head);
	h->num_branch += 1;
	h->branch_array = realloc(h->branch_array, sizeof(branch)*(h->num_branch));
	strcpy(h->branch_array[h->num_branch-1].branch_name, branch_name);

    h->branch_array[h->num_branch-1].comit_array = NULL;
	h->branch_array[h->num_branch-1].num_comit = 0;
	h->branch_array[h->num_branch-1].temp_track = NULL;
	h->branch_array[h->num_branch-1].n_temp_track = 0;

    return 0;
}

int svc_checkout(void* helper, char* branch_name) {
    // TODO: Implement
    if(branch_name == NULL) {
        return -1;
    }

    help* h = helper;
    if(h->branch_array) {
        for(int i = 0; i < h->num_branch; i++) {
            int check = check_commit_changes(helper, h->branch_array[i]);
            if(check != 0) {
                return -2;
            }
        }
        int match = 0;
        for(int i = 0; i < h->num_branch; i++) {
            if(strcmp(h->branch_array[i].branch_name, branch_name) == 0) {
                match = 1;
                strcpy(h->head, branch_name);
            }
        }
        if(match == 0) {
            return -1;
        }
    }

    return 0;
}

char** list_branches(void* helper, int* n_branches) {
    // TODO: Implement
	if(n_branches == NULL) {
		return NULL;
	}
	help* h = helper;
	char** list_branches = malloc(sizeof(char*));
	*n_branches = 0;
	if(h->branch_array) {
		for(int i = 0; i < h->num_branch; i++) {
			*n_branches += 1;
			list_branches = realloc(list_branches,
				sizeof(char*)*(*n_branches));
			list_branches[(*n_branches)-1] = h->branch_array[i].branch_name;
			printf("%s\n", list_branches[(*n_branches)-1]);
		}
	} else {
		*n_branches = 1;
		printf("%s\n", h->head);
		list_branches[0] = h->head;
		return list_branches;
	}
    return list_branches;
}

int svc_add(void* helper, char* file_name) {
    // TODO: Implement
    if(!(file_name)) {
        return -1;
    }
    FILE* fi;
    int hash = 0;
    help* h = helper;
    if((fi = fopen(file_name, "r"))) {
        if(!(h->branch_array)) {
            //if branch is null
			h->num_branch = 1;
            h->branch_array = malloc(sizeof(branch)*(h->num_branch));
            strcpy(h->branch_array[0].branch_name, h->head);

            h->branch_array[0].comit_array = NULL;
            h->branch_array[0].num_comit = 0;

            h->branch_array[0].temp_track = malloc(sizeof(file));
            h->branch_array[0].n_temp_track = 1;

            strcpy(h->branch_array[0].temp_track[0].file_name,
                file_name);

            h->branch_array[0].temp_track[0]
                .current_hash = hash_file(helper, file_name);
            hash = hash_file(helper, file_name);
        } else {
            //branch is not null
            int i = find_index_branch(h, &i, h->head);

            //cmpr file_name with temp tracked file
            for(size_t j = 0; j < h->branch_array[i].n_temp_track; j++) {
                   if(strcmp(file_name, h->branch_array[i].temp_track[j]
                       .file_name) == 0) {
                            return -2;
                    }
           }
            //no match files found
            h->branch_array[i].n_temp_track += 1;
            int n_tr = h->branch_array[i].n_temp_track;

            h->branch_array[i].temp_track = realloc(
                h->branch_array[i].temp_track, sizeof(file)*n_tr);

            strcpy(h->branch_array[i].temp_track[n_tr-1].file_name,
                file_name);

            h->branch_array[i].temp_track[n_tr-1].current_hash =
                hash_file(helper, file_name);

            hash = hash_file(helper, file_name);
        }
    } else {
        return -3;
    }
    fclose(fi);
    return hash;
}

int svc_rm(void* helper, char* file_name) {
    // TODO: Implement
    if(!(file_name)) {
        return -1;
    }
    help* h = helper;
    int i = find_index_branch(h, &i, h->head);
    for(size_t j = 0; j < h->branch_array[i].n_temp_track; j++) {
        //find fileand remove
        if(strcmp(file_name,
            h->branch_array[i].temp_track[j].file_name) == 0) {
                int hash =
                    h->branch_array[i].temp_track[j].current_hash;
                for(size_t k=j; k<h->branch_array[i].n_temp_track-1; k++) {
                        h->branch_array[i].temp_track[k] =
                        h->branch_array[i].temp_track[k+1];
                }
                //decrese num_tracked_files
                h->branch_array[i].n_temp_track -= 1;
                h->branch_array[i].temp_track =
                    realloc(h->branch_array[i].temp_track,
                        sizeof(file)*h->branch_array[i].n_temp_track);

                return hash;
        }
    }
    return -2;
}

int svc_reset(void* helper, char* commit_id) {
    // TODO: Implement
    if(commit_id == NULL) {
        return -1;
    }

    help* h = helper;
	if(h->branch_array) {
		for(int i = 0; i < h->num_branch; i++) {
			if(h->branch_array[i].comit_array) {
				for(int j = 0; j < h->branch_array[i].num_comit; j++) {
					if(strcmp(h->branch_array[i].comit_array[j].commit_id,
						commit_id) == 0) {
                            // reset files
                            char* fold = concat(commit_id, "/");
                            for(int i = 0; i < h->branch_array[i]
                                .comit_array[j].num_tracked; i++) {

                                    char* path = concat(fold, basename(
                                        h->branch_array[i].comit_array[j]
                                        .tracked_files[i].file_name));
                                    write_file(path, h->branch_array[i]
                                        .comit_array[j].tracked_files[i]
                                        .file_name);

                                    free(path);
                            }
                            free(fold);
							return 0;
						}
				}
			}
		}
	}

	return -2;
}

char* svc_merge(void* helper, char* branch_name,
    struct resolution* resolutions, int n_resolutions) {
    // TODO: Implement
    if(branch_name == NULL) {
        puts("Invalid branch name");
        return NULL;
    }
    help* h = helper;
    if(h->branch_array) {
        int match = 0;
		for(int i = 0; i < h->num_branch; i++) {
            if(strcmp(branch_name, h->branch_array[i].branch_name) == 0) {
                match = 1;
                if(strcmp(branch_name, h->head) == 0) {
                    puts("Cannot merge a branch with itself");
                    return NULL;
                }
                int check = check_commit_changes(helper, h->branch_array[i]);
                if(check == 0) {
                    puts("Changes must be committed");
                    return NULL;
                }
            }
        }
        if(match == 0) {
            puts("Branch not found");
            return NULL;
        }
    }

    return NULL;
}
