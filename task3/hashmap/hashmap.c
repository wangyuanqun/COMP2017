#include "hashmap.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define CAPCITY (3600000)
struct hash_map {
    size_t (*hash)(void*);
	int (*cmp)(void*, void*);
	void (*kd)(void*);
	void (*vd)(void*);
    struct node** li;
	int cap;
};

struct node {
    void* k;
    void* v;
    struct node* next;
    pthread_mutex_t l;
};

struct hash_map* hash_map_new(size_t (*hash)(void*), int (*cmp)(void*,void*),
    void (*key_destruct)(void*), void (*value_destruct)(void*)) {
        if(!hash || !cmp || !key_destruct || !value_destruct) {
            return NULL;
        }
        struct hash_map* h = malloc(sizeof(struct hash_map));
        h->li = malloc(sizeof(struct node)*CAPCITY);
        h->cap = CAPCITY;
        h->hash = hash;
        h->cmp = cmp;
        h->kd = key_destruct;
        h->vd = value_destruct;
        for (int i = 0; i < h->cap; i++){
    		h->li[i] = NULL;
    	}
        return h;
}

void redo_entry(struct hash_map* map, struct node* temp, void* k, void* v) {
    map->kd(temp->k);
    map->vd(temp->v);
    temp->v = v;
    temp->k = k;
}
void hash_map_put_entry_move(struct hash_map* map, void* k, void* v) {
    if(!map || !k || !v) {
        return;
    }
    pthread_mutex_lock(&lock);
    int index = (map->hash(k))%(map->cap);
    if(map->li[index]) {
        struct node* temp = map->li[index];
        if(map->cmp(temp->k, k) == 1) {
            redo_entry(map, temp, k, v);
            pthread_mutex_unlock(&lock);
            return;
        } else {
            while(temp->next) {
                temp = temp->next;
                if(map->cmp(temp->k,k) == 1) {
                    redo_entry(map, temp, k, v);
                    pthread_mutex_unlock(&lock);
                    return;
                }
            }
            temp->next = malloc(sizeof(struct node));
            temp->next->v = v;
            temp->next->k = k;
            temp->next->next = NULL;
        }
        pthread_mutex_unlock(&lock);
    } else {
        map->li[index] = malloc(sizeof(struct node));
        map->li[index]->k = k;
        map->li[index]->v = v;
        map->li[index]->next = NULL;
    }
    pthread_mutex_unlock(&lock);
}

void* hash_map_get_value_ref(struct hash_map* map, void* k) {
    if(!map || !k) {
        return NULL;
    }
    int index = (map->hash(k))%(map->cap);
    if(map->li[index]) {
        if(map->cmp(map->li[index], k) == 1) {
            return map->li[index]->v;
        } else {
            struct node* temp = map->li[index];
            while(temp) {
                if(map->cmp(temp->k,k) == 1) {
                    return temp->v;
                }
                temp = temp->next;
            }
        }
    }
	return NULL;
}

void hash_map_remove_entry(struct hash_map* map, void* k) {
    if(!map || !k) {
        return;
    }
    int index = (map->hash(k))%(map->cap);
    if(map->li[index]) {
        pthread_mutex_lock(&lock);
        if(map->cmp(map->li[index]->k, k) == 1) {
            struct node* temp = map->li[index];
            map->li[index] = temp->next;
    		map->kd(temp->k);
    		map->vd(temp->v);
    		free(temp);
            pthread_mutex_unlock(&lock);
    		return;
        } else {
            struct node* temp = map->li[index];
            while(temp) {
                if(map->cmp(temp->k,k) == 1) {
                    struct node* t = temp;
                    temp = t->next;
            		map->kd(t->k);
            		map->vd(t->v);
            		free(t);
                }
                temp = temp->next;
            }
            pthread_mutex_unlock(&lock);
        }
    } else {
        return;
    }
}

void hash_map_destroy(struct hash_map* map) {
    if(map) {
        pthread_mutex_lock(&lock);
        for(int i = 0; i < map->cap; i++) {
            if(map->li[i]) {
                struct node* temp = map->li[i];
                while (temp != NULL){
        			struct node* t = temp->next;
        			map->kd(temp->k);
        			map->vd(temp->v);
        			free(temp);
        			temp = t;
        		}
            }
        }
        free(map->li);
        free(map);
        pthread_mutex_unlock(&lock);
    }
}
