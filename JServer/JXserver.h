#ifndef JXSERVER_H
#define JXSERVER_H

#include "util.h"

void* thread_func(void* arg);

// if errors occurs responsing with 0xf
void error_func(int client);

// echo for type 0x0
void echo_func(int client, struct connect* cl, uint8_t cmprs,
	uint8_t rqr_cmprs);

//directory list for type 0x2
void direct_list(int client, struct connect* cl, uint8_t cmprs,
	uint8_t rqr_cmprs);

// file size query for type 0x4
void file_size(int client, struct connect* cl, uint8_t cmprs,
	uint8_t rqr_cmprs);
	
// retrive file for type 0x6
void retrive_file(int client, struct connect* cl, uint8_t cmprs,
	uint8_t rqr_cmprs);

#endif
