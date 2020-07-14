#include "rc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//-----------------------------
// #define RC_INVALID_REF (0xFFFFFFFFFFFFFFFF)
// #define RC_INIT_SZ (8)
// #define RC_GROWTH_RT (2)
// #define RC_REF(type,p) (((type*)p->ptr))
// #define RC_DEREF(type,p) (*RC_REF(type,p))
//
// struct rc_entry {
//     size_t count;
//     size_t* dep_list;
//     size_t n_deps;
//     size_t dep_capacity;
// };
//
// struct strong_ref {
//     void* ptr;
//     struct rc_entry entry;
// };
//
// struct weak_ref {
//     size_t entry_id;
// };
//------------------------------------

struct object {
	struct strong_ref* strong_ref;
	struct weak_ref weak_ref;
};

struct object* head;
int size = 0;



struct strong_ref* rc_alloc(void* ptr, size_t n, struct strong_ref* dep) {
	if(ptr == NULL) {
		//null n null
		if(dep == NULL) {
			struct strong_ref* po = malloc(sizeof(struct strong_ref));
			po->ptr = malloc(n);
			po->entry.count=1;
			po->entry.n_deps=0;
			po->entry.dep_capacity = 32;
			po->entry.dep_list = malloc(sizeof(size_t)*po->entry.dep_capacity);
			if(head == NULL) {
				size+=1;
				head = malloc(sizeof(struct object));
				head[0].strong_ref = po;
				head[0].weak_ref.entry_id = 0;
				return po;
			} else {
				size+=1;
				head = realloc(head, sizeof(struct object)*size);
				struct object ne;
				ne.strong_ref = po;
				ne.weak_ref.entry_id = size-1;
                head[size-1] = ne;
				return po;
			}
		// null n dep
		} else if (dep != NULL) {

			struct strong_ref* po = malloc(sizeof(struct strong_ref));
			po->ptr = malloc(n);
			po->entry.count=1;
			po->entry.n_deps=0;
			po->entry.dep_capacity = 32;
			po->entry.dep_list = malloc(sizeof(size_t)*po->entry.dep_capacity);

			size+=1;
			head = realloc(head, sizeof(struct object)*size);

			struct object next;
            next.strong_ref = po;
			next.weak_ref.entry_id = size-1;
			head[size-1] = next;

            for(int i = 0; i < size; i++) {
                if(dep->ptr == head[i].strong_ref->ptr) {
                    head[i].strong_ref->entry.n_deps+=1;
                    if(head[i].strong_ref->entry.n_deps == head[i].strong_ref->entry.dep_capacity) {
                        head[i].strong_ref->entry.dep_capacity*= 2;
                        head[i].strong_ref->entry.dep_list = realloc(head[i].strong_ref->entry.dep_list, sizeof(size_t)*head[i].strong_ref->entry.dep_capacity);
                    }
                    head[i].strong_ref->entry.dep_list[head[i].strong_ref->entry.n_deps-1] = size-1;
                }
            }
            return po;
		}
	} else if (ptr != NULL) {
		// ptr n null
        if(dep == NULL) {
            for(int i = 0; i < size; i++) {
                if(head[i].strong_ref->ptr == ptr) {
                    head[i].strong_ref->entry.count+=1;
                    return head[i].strong_ref;
                }
            }
        //ptr n dep
        } else {
            struct strong_ref* po;
            for(int i = 0; i < size; i++) {

                if(head[i].strong_ref->ptr == ptr) {
                    po = head[i].strong_ref;
                    head[i].strong_ref->entry.count += 1;
                }

                if(head[i].strong_ref->ptr == dep->ptr) {
                    head[i].strong_ref->entry.n_deps+=1;
                    if(head[i].strong_ref->entry.n_deps == head[i].strong_ref->entry.dep_capacity) {
                        head[i].strong_ref->entry.dep_capacity*= 2;
                        head[i].strong_ref->entry.dep_list = realloc(head[i].strong_ref->entry.dep_list, sizeof(size_t)*head[i].strong_ref->entry.dep_capacity);
                    }
                    head[i].strong_ref->entry.dep_list[head[i].strong_ref->entry.n_deps-1] = size-1;
                }

            }
            return po;
		}
	}
	return NULL;
}

struct weak_ref rc_downgrade(struct strong_ref* ref) {
	struct weak_ref r = { 0xFFFFFFFFFFFFFFFF };

    if(ref == NULL) {
        return r;
    }
	for(int i = 0; i < size; i++) {
        if(ref->ptr == head[i].strong_ref->ptr) {
            head[i].strong_ref->entry.count-=1;
            if(head[i].strong_ref->entry.count > 0) {
                r = head[i].weak_ref;
            } else {
                for(size_t j = 0; j < head[i].strong_ref->entry.n_deps; j++) {
                    head[head[i].strong_ref->entry.dep_list[j]].strong_ref->entry.count = 0;
                }
            }
        }
    }

	return r;
}

struct strong_ref* rc_upgrade(struct weak_ref ref) {
	if(ref.entry_id == RC_INVALID_REF || head == NULL) {
		return NULL;
	}

    if(head[ref.entry_id].strong_ref->entry.count > 0) {
        for(size_t i = 0; i < head[ref.entry_id].strong_ref->entry.n_deps; i++) {
            head[head[ref.entry_id].strong_ref->entry.dep_list[i]].strong_ref->entry.count+=1;
        }
        head[ref.entry_id].strong_ref->entry.count += 1;

        return head[ref.entry_id].strong_ref;
    }


	return NULL;
}

void rc_cleanup() {
    for(int i = 0; i < size; i++) {
        free(head[i].strong_ref->ptr);
        if(head[i].strong_ref->entry.dep_list) {
            free(head[i].strong_ref->entry.dep_list);
        }
        free(head[i].strong_ref);
    }
    free(head);
    head = NULL;
}

// struct obj {
// 	int p;
// 	struct obj* k;
// };
//
// int rc_example_2() {
//     struct strong_ref* m = rc_alloc(NULL, sizeof(struct obj), NULL); // obj m = new obj()
// 	struct strong_ref* n = rc_alloc(NULL, sizeof(struct obj), NULL); // obj n = new obj()
// 	struct strong_ref* kk = ((struct obj*) m->ptr)->k;
//     kk = rc_alloc(n->ptr, sizeof(struct obj), m); // obj m.k = n
//     printf("%zu ", m->entry.count); // 1
//     printf("%zu\n", n->entry.count); // 2
//
//     struct weak_ref k = rc_downgrade(m);
//     printf("%zu ", m->entry.count); // 0
//     printf("%zu ", kk->entry.count); // 1
//     printf("%zu\n", n->entry.count); // 1
//     return 0;
// }
//
//
// int main(int argc, char** argv) {
// 	rc_example_2();
//
// 	return 0;
// }
