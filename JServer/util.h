#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <byteswap.h>

#define ECHO (0x1)
#define DIRECT_LIST (0x3)
#define FILE_SIZE (0x5)
#define RETRIVE_FILE (0x7)


struct dict {
	uint8_t k;
	int v_len;
	uint8_t v[32];
};

struct connect {
	int client;
	struct dict* dict;
	char* direct;
	struct list* list;
};

struct list {
    struct node* head;
    pthread_mutex_t lock;
};

struct node {
	struct request* data;
	struct node* next;
};

struct request {
	uint32_t id;
	uint64_t start;
	uint64_t len;
	char* filename;
};

// set bit into the corresponding positions
void set_bit(uint8_t* buf, int index, int value);

//get bit from the corresponding positions
uint8_t get_bit(uint8_t* array, int index);

// extract values and len from dict
void init(uint8_t* dict, uint8_t** v_array, int** pos);

// compress original payload with dictionary
void compress(struct dict* dict, uint8_t* payload, uint64_t paylen,
	uint64_t* result_len, uint8_t** result);

// decompress original payload with dictionary
void decompress(struct dict* dict, uint8_t* payload, uint64_t paylen,
	uint64_t* result_len, uint8_t** result);

// check if file exists and get the size of file
int file_exist(char* file, char* directory, uint64_t* file_size);

// check if the same request exists
int request_cmp(struct request* cursor, struct request* current);

#endif
