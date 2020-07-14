#include "JXserver.h"

void error_func(int client) {
	uint8_t e_header = 0xf << 4;
	uint64_t e_paylen = 0x0;
	write(client, &e_header, 1);
	write(client, &e_paylen, 8);
	close(client);
}

void echo_func(int client, struct connect* cl,
	uint8_t cmprs, uint8_t rqr_cmprs) {
	// check if receive correctly
	uint64_t origin_paylen;
	if(recv(client, &origin_paylen, 8, 0) <= 0) {
		return;
	}
	// check if paylen is valid
	uint64_t paylen = bswap_64(origin_paylen);
	if(paylen < 0) {
		error_func(client);
		return;
	}
	// check if receive correctly
	uint8_t* payload = malloc(sizeof(uint8_t)*paylen);
	if(recv(client, payload, paylen, 0) <= 0) {
		return;
	}
	// if the recived message is compressed
	if(cmprs == 0) {
		// if the responed message is not required compressed
		if(rqr_cmprs == 0) {
			uint8_t response = (ECHO << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);
			write(client, &origin_paylen, 8);
			write(client, payload, paylen);
			free(payload);

			// if the responed message is required compressed
		} else {
			// prepare for the results to send
			uint64_t result_len = 0;
			uint8_t* result = malloc(100);
			compress(cl->dict, payload, paylen, &result_len, &result);
			cmprs = 1;
			rqr_cmprs = 0;
			paylen = htobe64(result_len);

			//send response
			uint8_t response = (ECHO << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);
			write(client, &paylen, 8);
			write(client, result, result_len);
			free(payload);
			free(result);
		}
		// if the recived message is not compressed
	} else {
		// if the responed message is not required compressed
		if(rqr_cmprs == 0) {
			uint8_t response = (ECHO << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);
			write(client, &origin_paylen, 8);
			write(client, payload, paylen);
			free(payload);
		} else {
			// if the responed message is required compressed
			rqr_cmprs = 0;

			uint8_t response = (ECHO << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);
			write(client, &origin_paylen, 8);
			write(client, payload, paylen);
			free(payload);
		}
	}
}

void direct_list(int client, struct connect* cl,
	uint8_t cmprs, uint8_t rqr_cmprs) {
	// open directory
	struct dirent* direct;
	DIR* dir = opendir(cl->direct);

	// if directory exists
	if (dir) {
		// get length of payload
		uint64_t origin_paylen;
		if(recv(client, &origin_paylen, 8, 0) <= 0) {
			return;
		}
		//error check if paylen is valid
		uint64_t paylen = bswap_64(origin_paylen);
		if(paylen < 0) {
			error_func(client);
			return;
		}

		// check if paylen is 0
		if (paylen == 0) {
			uint8_t* file_name = malloc(100);
			int file_count = 0;
			// read from directory
			while ((direct = readdir(dir))) {
				if (direct->d_type == DT_REG) {
					int i = 0;
					while (i < strlen(direct->d_name)) {
						file_name[file_count + i] = direct->d_name[i];
						i++;
					}
					file_count += strlen(direct->d_name)+1;
					file_name[file_count-1] = 0x00;
				}
			}
			closedir(dir);
			// if no file in this directory, set 0x00
			if (file_count == 0) {
				file_name[file_count] = 0x00;
				file_count++;
			}

			// if not require compress
			if(rqr_cmprs == 0) {
				paylen = htobe64(file_count);

				uint8_t response = (DIRECT_LIST << 4)|(cmprs << 3)|(rqr_cmprs << 2);
				write(client, &response, 1);
				write(client, &paylen, 8);
				write(client, file_name, file_count);
				free(file_name);
			} else {
				// compress required
				uint64_t result_len = 0;
				uint8_t* result = malloc(100);
				compress(cl->dict, file_name, file_count,&result_len,&result);
				cmprs = 1;
				rqr_cmprs = 0;
				paylen = htobe64(result_len);
				//send response
				uint8_t response = (DIRECT_LIST << 4)|(cmprs << 3)|(rqr_cmprs << 2);
				write(client, &response, 1);
				write(client, &paylen, 8);
				write(client, result, result_len);
				free(file_name);
				free(result);
			}

		} else {
			error_func(client);
			return;
		}
	} else {
		error_func(client);
		return;
	}
}

void file_size(int client, struct connect* cl,
	uint8_t cmprs, uint8_t rqr_cmprs) {
	// get length of payload
	uint64_t origin_paylen;
	if(recv(client, &origin_paylen, 8, 0) <= 0) {
		return;
	}
	//error check
	uint64_t paylen = bswap_64(origin_paylen);
	if(paylen <= 0) {
		error_func(client);
		return;
	}
	//get payload
	uint8_t* payload = malloc(sizeof(uint8_t) * paylen);
	if (recv(client, payload, paylen, 0) <= 0) {
		return;
	}

	char* file = (char*)payload;
	uint64_t file_size;
	if (file_exist(cl->direct, file, &file_size) == 1) {
		if(rqr_cmprs == 0) {
			// if not required compress, send back content
			rqr_cmprs = 0;
			cmprs = 0;
			uint8_t response = (FILE_SIZE << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);

			uint64_t pay = htobe64(8);
			write(client, &pay, 8);

			uint64_t resp_pay = htobe64(file_size);
			write(client, &resp_pay, 8);

			free(payload);
		} else {
			// convert size to big endian
			uint64_t converted_size = htobe64(file_size);
			free(payload);
			payload = (uint8_t*)&converted_size;
			// compress file with dictionary
			uint64_t result_len = 0;
			uint8_t* result = malloc(sizeof(uint8_t)*100);
			compress(cl->dict, payload, 8, &result_len, &result);

			cmprs = 1;
			rqr_cmprs = 0;
			paylen = htobe64(result_len);
			// send back to client with response message
			uint8_t response = (FILE_SIZE << 4) | (cmprs << 3) | (rqr_cmprs << 2);
			write(client, &response, 1);
			write(client, &paylen, 8);
			write(client, result, result_len);
			free(result);
		}
	} else {
		// if file not exists, error occurs
		error_func(client);
		return;
	}
}
void retrive_file(int client, struct connect* cl,
	uint8_t cmprs, uint8_t rqr_cmprs) {
	// check if receive correctly
	uint64_t origin_paylen;
	if(recv(client, &origin_paylen, 8, 0) <= 0) {
		return;
	}
	// check if paylen is valid
	uint64_t paylen = bswap_64(origin_paylen);
	if(paylen <= 0) {
		error_func(client);
		return;
	}
	// check if receive correctly
	uint8_t* payload = malloc(sizeof(uint8_t)*paylen);
	if(recv(client, payload, paylen, 0) <= 0) {
		free(payload);
		return;
	}

	// the payload is compressed, then decompress it
	if(cmprs == 1) {
		uint64_t result_len = 0;
		uint8_t* result = malloc(100);
		decompress(cl->dict, payload, paylen, &result_len, &result);
		free(payload);
		payload = NULL;
		paylen = result_len;
		payload = result;
	}

	// get the retrive file propertities
	uint32_t id = 0;
	uint64_t start = 0;
	uint64_t len = 0;
	// get session_id, start offset and length from payload
	memcpy(&id, payload, 4);
	memcpy(&start, &payload[4], 8);
	memcpy(&len, &payload[12], 8);
	id = htobe32(id);
	start = htobe64(start);
	len = htobe64(len);
	// get the file name
	int file_len = paylen - 20;
	char* filename = malloc(file_len * sizeof(char));
	memcpy(filename, &payload[20], file_len);

	// store the content to the structure
	struct request* current = malloc(sizeof(struct request));
	current->id = id;
	current->start = start;
	current->len = len;
	current->filename = filename;

	pthread_mutex_lock(&cl->list->lock);
	// check if the same request recived
	int match = 0;
	struct node* cursor = cl->list->head;
	while (cursor->next) {
		if (request_cmp(cursor->data, current)) {
			match = 1;
			break;
		}
		cursor = cursor->next;
	}

	// loop to the last one
	if (request_cmp(cursor->data, current)) {
		match = 1;
	}

	// if the same request received, make the response
	if(match) {
		pthread_mutex_unlock(&cl->list->lock);
		uint8_t header = RETRIVE_FILE << 4 | 0 << 3 | 0 << 2;
		write(client, &header, 1);
		uint64_t ret_len = 0;
		write(client, &ret_len, 8);
		free(current);
		free(payload);
		free(filename);
		return;
	} else {
		// if it is a different request then add to new node
		cursor->next = malloc(sizeof(struct node));
		cursor->next->next = NULL;
		cursor->next->data = malloc(sizeof(struct request));
		cursor->next->data->id = id;
		cursor->next->data->start = start;
		cursor->next->data->len = len;
		cursor->next->data->filename = filename;
		pthread_mutex_unlock(&cl->list->lock);
	}

	// check is the file exists
	uint64_t file_size = 0;
	if (file_exist(cl->direct, filename, &file_size) == 0) {
		error_func(client);
		free(current);
		free(payload);
		free(filename);
		return;
	}

	// check if request message length is valid
	int diff = file_size - start;
	if (len > diff){
		error_func(client);
		free(current);
		free(payload);
		free(filename);
		return;
	}

	// create the path to the file with given directory
	int path_len = file_len + strlen(cl->direct) + 2;
	char* path = malloc(path_len*sizeof(char));
	strcpy(path, cl->direct);
	strcat(path, "/");
	strcat(path, filename);
	//get the requested file content
	FILE* f = fopen(path, "r");
	fseek(f, start, SEEK_SET);
	uint8_t* content = malloc(len);
	fread(content, 1, len, f);
	fclose(f);

	// buffer is session id stuff combined with content
	uint8_t* buffer = malloc(len + 20);
	memcpy(buffer, payload, 20);
	memcpy(&buffer[20], content, len);
	free(content);

	if(rqr_cmprs == 1) {
		// set compress to 1 to indicate that the content is compressed
		cmprs = 1;
		rqr_cmprs = 0;
		uint8_t response = (RETRIVE_FILE << 4) | (cmprs << 3) | (rqr_cmprs << 2);
		write(client, &response, 1);
		// compress the content
		uint64_t result_len = 0;
		uint8_t* result = malloc(5000);
		compress(cl->dict, buffer, len+20, &result_len, &result);
		// send back the content with corresponding length
		uint64_t ret_len = htobe64(result_len);
		write(client, &ret_len, 8);
		write(client, result, result_len);
		free(path);
		free(current);
		free(payload);
		free(buffer);
		free(filename);
	} else {
		// not require compress, set compress to 0
		cmprs = 0;
		rqr_cmprs = 0;
		uint8_t response = (RETRIVE_FILE << 4) | (cmprs << 3) | (rqr_cmprs << 2);
		write(client, &response, 1);
		paylen = htobe64(len + 20);
		write(client, &paylen, 8);
		// write content back to client
		write(client, buffer, len+20);
		free(path);
		free(current);
		free(payload);
		free(buffer);
		free(filename);
	}
}

void* thread_func(void* arg) {
	struct connect* cl = arg;
	int client = cl->client;

	while(1) {
		uint8_t header;
		if(recv(client, &header, 1, 0) <= 0) {
			break;
		}
		// initialising type, compress and required compress
		uint8_t type = header >> 4;
		uint8_t cmprs = header >> 3;
		cmprs = cmprs << 7;
		cmprs = cmprs >> 7;
        uint8_t rqr_cmprs = header >> 2;
		rqr_cmprs = rqr_cmprs << 7;
		rqr_cmprs = rqr_cmprs >> 7;

		// type 0x0 is echo
		if(type == 0x0) {
			echo_func(client, cl, cmprs, rqr_cmprs);
			// type 0x2 is Directory listing
		} else if(type == 0x2) {
			direct_list(client, cl, cmprs, rqr_cmprs);
			// type 0x4 is File size query
		} else if(type == 0x4) {
			file_size(client, cl, cmprs, rqr_cmprs);
			// type 0x6 is Retrive file
		} else if(type == 0x6) {
			retrive_file(client, cl, cmprs, rqr_cmprs);
			//type 0x8 is Shut down
		} else if(type == 0x8) {
			close(client);
			shutdown(client, SHUT_WR);
			free(cl);
			exit(0);
			// anything else is an error
		} else {
			error_func(client);
			break;
		}
	}
	free(cl);
    return NULL;
}
