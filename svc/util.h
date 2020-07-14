#ifndef util_h
#define util_h

char* concat(const char* s1, const char* s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char* result = malloc(len1 + len2 + 1);

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}

void write_file(char* file, char* target_file) {
    FILE* fptr1;
    FILE* fptr2;
    // Open one file for reading
    fptr1 = fopen(file, "r");
    if (fptr1 == NULL) {
        exit(0);
    }
    // Open another file for writing
    fptr2 = fopen(target_file, "w+");
    if (fptr2 == NULL) {
        exit(0);
    }
    // Read contents from file
    char c = fgetc(fptr1);
    while (c != EOF) {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
}

void save_file(char* commit_id, char* file_name) {
	//check is directory exists
	char* base_name = basename(file_name);

	DIR* dir = opendir(commit_id);
	if (dir) {
		/* Directory exists. */

		char* change = concat(commit_id, "/");

		char* make_path = concat(change, base_name);

		char touch[] = "touch ";
		char* touch_file = concat(touch, make_path);
		system(touch_file);

		write_file(file_name, make_path);

		free(change);
	    free(make_path);
	    free(touch_file);

		closedir(dir);
	} else if (ENOENT == errno) {
		/* Directory does not exist. */
		char make[10];
	    strcpy(make, "mkdir ");

		char* make_folder = concat(make, commit_id);
		system(make_folder);

		char* change = concat(commit_id, "/");
		char* make_path = concat(change, base_name);

		char touch[] = "touch ";
		char* touch_file = concat(touch, make_path);
		system(touch_file);

		write_file(file_name, make_path);

		free(make_folder);
	    free(change);
	    free(make_path);
	    free(touch_file);

	} else {
		/* opendir() failed for some other reason. */
		printf("%s\n", "other failed reasons");
	}

}

void init_commit(branch* branch, char* message) {
	branch->num_comit += 1;
	int num = branch->num_comit;
	branch->comit_array = realloc(branch->
		comit_array, sizeof(commit)*branch->num_comit);

	strcpy(branch->comit_array[num-1].message, message);

	branch->comit_array[num-1].add_file = NULL;
	branch->comit_array[num-1].num_add = 0;

	branch->comit_array[num-1].rm_file = NULL;
	branch->comit_array[num-1].num_rm = 0;

	branch->comit_array[num-1].modi_file = NULL;
	branch->comit_array[num-1].num_modi = 0;

	branch->comit_array[num-1].tracked_files = NULL;
	branch->comit_array[num-1].num_tracked = 0;
}

void initial_comit_array(branch* branch, char* message) {

	branch->comit_array = malloc(sizeof(commit));
	branch->comit_array[0].rm_file = NULL;
	branch->comit_array[0].modi_file = NULL;
	branch->comit_array[0].num_rm = 0;
	branch->comit_array[0].num_modi = 0;
	branch->comit_array[0].num_add = branch->n_temp_track;
	branch->comit_array[0].num_tracked = branch->n_temp_track;

	branch->num_comit += 1;

	strcpy(branch->comit_array[0].message, message);

	branch->comit_array[0].add_file =
		malloc(sizeof(file)*branch->comit_array[0].num_add);

	memcpy(branch->comit_array[0].add_file, branch->temp_track,
		sizeof(file)*branch->comit_array[0].num_add);

	branch->comit_array[0].tracked_files =
		malloc(sizeof(file)*branch->comit_array[0].num_tracked);

	memcpy(branch->comit_array[0].tracked_files, branch->temp_track,
		sizeof(file)*branch->comit_array[0].num_tracked);

}

int check_commit_changes(void* helper, branch branch){
	if(branch.comit_array) {
		//if num_temp == num_track
		if(branch.n_temp_track != branch
			.comit_array[branch.num_comit-1].num_tracked) {
				return 1;
		}
		//check if there are different file, if len is the same
		for(int j = 0; j < branch.n_temp_track; j++) {
			int match = 0;
			for(int k = 0; k < branch.comit_array[branch.num_comit-1]
				.num_tracked; k++) {
					// if different name found
					if(strcmp(branch.temp_track[j].file_name,branch
						.comit_array[branch.num_comit-1].tracked_files[k]
						.file_name) == 0) {
							match = 1;
							break;
					}

			}
			if(match == 0) {
				return 4;
			}
		}

		// check if modi exists
		FILE* f;
		for(int j = 0; j < branch.n_temp_track; j++) {
			if((f = fopen(branch.temp_track[j].
				file_name, "r"))) {

				int pre_hash = branch
					.temp_track[j].current_hash;

				int cur_hash = hash_file(helper,
					branch.temp_track[j].file_name);

				if(pre_hash != cur_hash) {
					return 2;
				}

			} else {
				return 3;
			}
		}
	}
	return 0;
}

int check_valid(char* branch_name) {
	int strl = strlen(branch_name);
	for(int i = 0; i < strl; i++) {

		if ((branch_name[i] >= 'a' && branch_name[i] <= 'z') ||
			(branch_name[i] >= 'A' && branch_name[i] <= 'Z') ||
			(branch_name[i] >= '0' && branch_name[i] <= '9') ||
			(branch_name[i] == '_') || (branch_name[i] == '-') ||
			(branch_name[i] == '/')) {
				//do nothing
	    } else {
			return 0;
		}
	}
	return 1;
}


int cmpr(const void* a, const void* b) {
    return strcasecmp(*(const char**)a, *(const char**)b);
}

int find_index_branch(help* h, int* i, char* branch_name) {
	for(*i = 0; *i < h->num_branch; (*i)++) {
		if(strcmp(branch_name, h->branch_array[*(i)].branch_name) == 0) {
			break;
		}
	}

	return *i;
}

void get_commit_id(commit c, char* commit_id, char* message) {
    int id = 0;

    int strl = strlen(message);
    for(int i = 0; i < strl; i++) {
        id = (message[i] + id) % 1000;
    }
    int num_file = c.num_add + c.num_rm + c.num_modi;
    file* f = malloc(sizeof(file)*num_file);
	if(c.num_add == 0 && c.num_rm != 0 && c.num_modi != 0) {
		memcpy(f, c.rm_file, sizeof(file)*c.num_rm);
		memcpy(f+c.num_rm, c.modi_file, sizeof(file)*c.num_modi);
	}else if(c.num_add != 0 && c.num_rm == 0 && c.num_modi != 0) {
		memcpy(f, c.add_file, sizeof(file)*c.num_add);
		memcpy(f+c.num_add, c.modi_file, sizeof(file)*c.num_modi);
	}else if(c.num_add != 0 && c.num_rm != 0 && c.num_modi == 0) {
		memcpy(f, c.add_file, sizeof(file)*c.num_add);
		memcpy(f+c.num_add, c.rm_file, sizeof(file)*c.num_rm);
	}else if(c.num_add == 0 && c.num_rm == 0 && c.num_modi != 0) {
		memcpy(f, c.modi_file, sizeof(file)*c.num_modi);
	}else if(c.num_add != 0 && c.num_rm == 0 && c.num_modi == 0) {
		memcpy(f, c.add_file, sizeof(file)*c.num_add);
	}else if(c.num_add == 0 && c.num_rm != 0 && c.num_modi == 0) {
		memcpy(f, c.rm_file, sizeof(file)*c.num_rm);
	}else {
	    memcpy(f, c.add_file, sizeof(file)*c.num_add);
	    memcpy(f+c.num_add, c.rm_file, sizeof(file)*c.num_rm);
	    memcpy(f+c.num_add+c.num_rm, c.modi_file, sizeof(file)*c.num_modi);
	}

    char** file_name = malloc(sizeof(char*)*num_file);

    for(int i = 0; i < num_file; i++) {
        file_name[i] = f[i].file_name;
    }

    qsort(file_name, num_file, sizeof(const char*), cmpr);

    for(int i = 0; i < num_file; i++) {
        for(int j = 0; j < c.num_add; j++) {
            if(strcmp(file_name[i], c.add_file[j].file_name) == 0) {
                id += 376591;
                break;
            }
        }
        for(int j = 0; j < c.num_rm; j++) {
            if(strcmp(file_name[i], c.rm_file[j].file_name) == 0) {
                id += 85973;
                break;
            }
        }
        for(int j = 0; j < c.num_modi; j++) {
            if(strcmp(file_name[i], c.modi_file[j].file_name) == 0) {
                id += 9573681;
                break;
            }
        }

        int strl = strlen(file_name[i]);
        for(int j = 0; j < strl; j++) {
            id = (id * ((unsigned char)file_name[i][j] % 37)) % 15485863 + 1;
        }
    }
    sprintf(commit_id, "%06x", id);
	free(file_name);
    free(f);
}

#endif
