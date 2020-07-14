#include "JXserver.h"

#define SERVER_BACKLOG (2000)
int main(int argc, char** argv) {
	// check arguments
	if(argc != 2) {
		return 0;
	}
	//=================read dictionary=========================
	FILE* fil = fopen("compression.dict", "rb");
	uint8_t* dict = malloc(8848);
	fread(dict, 8848, 1, fil);
	fclose(fil);

	uint8_t* v_array = malloc(8192);
	int* pos = malloc((257) * sizeof(int));
	init(dict, &v_array, &pos);

	struct dict* new = malloc(sizeof(struct dict) * 256);
	for (int i = 0; i < 256; i++) {
		new[i].v_len = pos[i+1] - pos[i];
		new[i].k = i;

		int dict_count = 0;
		int j = pos[i];
		while(j < pos[i+1]) {
			set_bit(new[i].v, dict_count++, get_bit(v_array, j));
			j++;
		}
	}
	free(dict);
	free(pos);
	free(v_array);
	//=============================================

	//===========initialize linked list===========
	struct list* q = malloc(sizeof(struct list));
	pthread_mutex_init(&q->lock, NULL);
	// initialize a node
    q->head = malloc(sizeof(struct node));
	q->head->next = NULL;
	// initialize data of node
	q->head->data = malloc(sizeof(struct request));
	struct request* d = q->head->data;
	d->id = 0;
	d->start = 0;
	d->len = 0;
	d->filename = NULL;
	//=============================================

	//==============initialize socket================
	int ser = socket(AF_INET, SOCK_STREAM, 0);
	if(ser < 0) {
		return 0;
	}
	//read address and port and directory from config
	FILE* f = fopen(argv[1], "rb");
	uint32_t addr;
	fread(&addr, 4, 1, f);
    uint16_t port;
    fread(&port, 2, 1, f);

	struct stat direct_stat;
	stat(argv[1], &direct_stat);
	int direct_len = direct_stat.st_size - 6;
	char* direct = malloc((direct_len + 1) * sizeof(char));
	fread(direct, sizeof(char), direct_len, f);
	direct[direct_len] = '\0';
    fclose(f);

	struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = addr;
    address.sin_port = port;

	if(bind(ser, (struct sockaddr*) &address, sizeof(struct sockaddr_in))) {
		return 0;
	}

	listen(ser, SERVER_BACKLOG);

	int add_len = sizeof(struct sockaddr);
	//=======================================
	while(1) {
		int client = accept(ser, (struct sockaddr*)&address,
			(socklen_t*)&add_len);

		if(client < 0) {
			return 0;
		}
		// initialize the connect data for thread
		struct connect* con = malloc(sizeof(struct connect));
		con->client = client;
		con->direct = direct;
		con->dict = new;
		con->list = q;

		pthread_t threa;
		pthread_create(&threa, NULL, thread_func, con);
	}
	return 0;
}
