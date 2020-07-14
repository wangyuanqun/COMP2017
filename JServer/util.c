#include "util.h"

void set_bit(uint8_t* buf, int index, int value) {
    if(value == 1) {
		buf[index / 8] |= 1 << (8 - 1 - index % 8);
	} else {
		buf[index / 8] &= ~(1 << (8 - 1 - index % 8));
	}
}

uint8_t get_bit(uint8_t* array, int index){
	return array[index / 8] >> (8 - 1 - index % 8) & 1;
}

void init(uint8_t* dict, uint8_t** v_array, int** pos) {

	int num_256 = 0;
	int idx_seg = 0;

	int cursor = 0;
	while(cursor < 8848) {
		(*pos)[num_256] = idx_seg;

		// get length
		uint8_t len = 0x00;
		for (int i = 0; i < 8; i++) {
			set_bit(&len, i, get_bit(dict, cursor));
			cursor++;
		}

		// get value
		int i = 0;
		while(i < len) {
			set_bit(*v_array, idx_seg, get_bit(dict, cursor));
			i++;
			cursor++;
			idx_seg++;
		}

		num_256++;
		if (num_256 == 256) {
			(*pos)[num_256] = idx_seg;
			break;
		}
	}
}

void compress(struct dict* dict, uint8_t* payload, uint64_t paylen,
	uint64_t* result_len, uint8_t** result) {

	int cursor = 0;
	int i = 0;
	// loop through payload to find match
	while(i < paylen) {
		int j = 0;
		while(j < 256) {// 256 is dict size
			// if match found copy dict to comperssed message
			if(payload[i] == dict[j].k) {
				int k = 0;
				while(k < dict[j].v_len) {
					set_bit((*result), cursor, get_bit(dict[j].v, k++));
					cursor++;
				}
				break;
			}
			j++;
		}
		i++;
	}
	// check if there needs 0 padding_cursor
	uint8_t padding;
	if ((cursor % 8) != 0) {
		padding = 8 - (cursor % 8);
	} else {
		padding = 0;
	}
	// set the compressed message length
	*result_len = ((cursor + padding) / 8) + 1;

	// set 0 in padding
	int padding_index = cursor;
	for (int i = 0; i < padding; i++) {
		set_bit((*result), padding_index++, 0);
	}

	// put number of padding into the buffer
	(*result)[(*result_len)-1] = padding;

}

void decompress(struct dict* dict, uint8_t* payload, uint64_t paylen,
	uint64_t* result_len, uint8_t** result) {
	// get the length of compressed content
	int cmprs_len = ((paylen - 1) * 8) - payload[paylen-1];
	// the index of cmprs where start to match with key in dictionary
	int index_cmprs = 0;
	for(int index = 0; index <= cmprs_len; index++) {
		//store the subset of cmprs content
		int len = 0;
		uint8_t* array = malloc(paylen - 1);

		int i = index_cmprs;
		while(i < index) {
			set_bit(array, len++, get_bit(payload, i++));
		}
		// search the dictionary to find the match between key and subset
		for (int i = 0; i < 256; i++) {
			if (dict[i].v_len == len) {
				int match = 0;
				for (int j = 0; j < len; j++) {
					if (get_bit(dict[i].v, j) == get_bit(array, j)) {
						match = 1;
					} else {
						match = 0;
						break;
					}
				}
				//match found
				if (match == 1) {
					(*result)[(*result_len)++] = dict[i].k;
					index_cmprs = index;
					break;
				}
			}
		}
		free(array);
		array = NULL;
	}
}

int file_exist(char* directory, char* file, uint64_t* file_size) {
	// open the target directory
	struct dirent* direct;
	DIR* dir = opendir(directory);
	// the direcotry is not exist
	if (dir) {
		// restore file name
		char** file_name = malloc(sizeof(char) * 100);
		int file_count = 0;
		// read from directory
		while ((direct = readdir(dir))) {
			if (direct->d_type == DT_REG) {
				file_name[file_count] = direct->d_name;
				file_count += 1;
			}
		}
		// check if file is found, and get the size of file
		int i = 0;
		while(i < file_count) {
			if (strcmp(file_name[i++], file) == 0) {
				// combine file and direct
				char* path = malloc(strlen(file) + strlen(directory) + 2);
				strcpy(path, directory);
				strcat(path, "/");
				strcat(path, file);

				// get the size of file
				struct stat file_stat;
				stat(path, &file_stat);
				*file_size = file_stat.st_size;

				closedir(dir);
				free(file_name);
				free(path);
				return 1;
			}
		}

		closedir(dir);
		free(file_name);
		return 0;
	} else {
		return 0;
	}
}

int request_cmp(struct request* cursor, struct request* current) {
	if (cursor->id == current->id && cursor->start == current->start &&
		cursor->len == current->len && strcmp(cursor->filename,
		current->filename) == 0) {
			return 1;
	}
	return 0;
}
