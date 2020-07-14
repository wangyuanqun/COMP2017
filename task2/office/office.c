#include "office.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_Q_SIZE 100


int size;
struct employee* enQueue(struct employee** queue, int* rear, struct employee* new_emp)
{
    size+=1;
    int j = *rear;
    queue[*rear] = new_emp;
    (*rear)++;
    return queue[j];
}

struct employee* deQueue(struct employee** queue, int* front)
{
   (*front)++;
   return queue[*front - 1];
}

struct employee* root;


 void office_employee_place(struct office* off, struct employee* supervisor,
   struct employee* emp) {

     if(off == NULL || emp == NULL) {
         return;
     }


     struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);

     if(supervisor == NULL) {
         if(off->department_head == NULL) {

             off->department_head = malloc(sizeof(struct employee));
             memcpy(off->department_head, emp, sizeof(struct employee));

             off->department_head->name = malloc(sizeof(char)*39);
             strcpy(off->department_head->name, emp->name);

             root = off->department_head;

         }else if(off->department_head != NULL) {

             int rear = 0;
             int front = 0;
             struct employee* temp_emp = off->department_head;


             while(temp_emp) {

                 if(temp_emp->subordinates) {
                     for(int i = 0; i < temp_emp->n_subordinates; i++) {
                         enQueue(queue, &rear, temp_emp->subordinates+i);
                     }
                 } else {
                     //set subordinates

                     temp_emp->n_subordinates+=1;
                     temp_emp->subordinates = realloc(temp_emp->subordinates,
                         sizeof(struct employee)*temp_emp->n_subordinates);

                     memcpy(&(temp_emp->subordinates[temp_emp->n_subordinates-1]),
                             emp, sizeof(struct employee));

                     (temp_emp->subordinates+temp_emp->n_subordinates-1)->name=
                     malloc(sizeof(char)*39);

                     strcpy(
                     (temp_emp->subordinates+temp_emp->n_subordinates-1)->name,
                     emp->name);

                     temp_emp->subordinates[temp_emp->n_subordinates-1].supervisor
                     = temp_emp;

                     break;
                 }

                 temp_emp = deQueue(queue, &front);
             }
         }
     } else if(supervisor != NULL) {

         int rear = 0;
         int front = 0;
         struct employee* temp_emp = off->department_head;

         while(temp_emp) {

             if(supervisor == temp_emp) {

                 temp_emp->n_subordinates++;
                 temp_emp->subordinates = realloc(temp_emp->subordinates,
                     sizeof(struct employee)*temp_emp->n_subordinates);

                 memcpy((temp_emp->subordinates+temp_emp->n_subordinates-1),
                         emp, sizeof(struct employee));

                 (temp_emp->subordinates+temp_emp->n_subordinates-1)->name =
                 malloc(sizeof(char)*39);

                 strcpy(
                 (temp_emp->subordinates+temp_emp->n_subordinates-1)->name,
                 emp->name);

                 temp_emp->subordinates[temp_emp->n_subordinates-1].supervisor
                 = temp_emp;

                 break;
             }
             if(temp_emp->subordinates) {
                 for(int i = 0; i < temp_emp->n_subordinates; i++) {
                     enQueue(queue, &rear, temp_emp->subordinates+i);
                 }
             }
             temp_emp = deQueue(queue, &front);
         }
     }
     // free queue
     free(queue);
 }


 void office_fire_employee(struct employee* employee) {
     if(employee == NULL) {
         return;
     }

     int front = 0;
     int rear = 0;
     struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);
     struct employee* temp = root;

     while(temp->subordinates) {
		 //check if fire_emp is in sub
		 for(int i = 0; i < temp->n_subordinates; i++) {

			 if(temp->subordinates+i == employee) {

				 if((temp->subordinates+i)-> subordinates) {
					 //remove emp who has subordinate


				 } else {
					 //remove emp who do not have subordinates
                     struct employee* fr = &temp->subordinates[i];
                     free(fr->name);
					 for(int j = i; j < temp->n_subordinates-1; j++) {
						 memmove(&temp->subordinates[j], &temp->subordinates[j+1], sizeof(struct employee));
					 }
					 temp->n_subordinates-=1;
					 temp->subordinates = realloc(temp->subordinates, sizeof(struct employee)*temp->n_subordinates);
				 }
                 free(queue);
				 return;
			 }


			 enQueue(queue, &rear, temp->subordinates+i);
		 }


         temp = deQueue(queue, &front);



	 }
     free(queue);

 }


struct employee* office_get_first_employee_with_name(struct office* office,
  const char* name) {
      if(office == NULL || name ==NULL) {
          return NULL;
      }
      int front = 0;
      int rear = 0;
      struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);
      struct employee* temp = office->department_head;

      while(temp) {

          if(strcmp(temp->name, name) == 0) {
              free(queue);
              return temp;
          }
          if(temp->subordinates) {
              for(int i = 0; i < temp->n_subordinates; i++) {
                  enQueue(queue, &rear, temp->subordinates+i);
              }
          }
          if(front == rear) {
              break;
          }
          temp = deQueue(queue, &front);

      }

    free(queue);
	return NULL;
}


struct employee* office_get_last_employee_with_name(struct office* office,
  const char* name) {
      if(office == NULL || name == NULL) {
          return NULL;
      }
      int front = 0;
      int rear = 0;
      struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);
      struct employee* temp = office->department_head;
      struct employee* ret = NULL;

      while(temp) {

          if(strcmp(temp->name, name) == 0) {
              ret = temp;
          }
          if(temp->subordinates) {
              for(int i = 0; i < temp->n_subordinates; i++) {
                  enQueue(queue, &rear, temp->subordinates+i);
              }
          }
          if(front == rear) {
              break;
          }
          temp = deQueue(queue, &front);

      }

    free(queue);
	return ret;
}


void office_get_employees_at_level(struct office* office, size_t level,
  struct employee** emplys, size_t* n_employees) {
      if(office == NULL) {
          return;
      }
      *n_employees = 0;
      *emplys = malloc(sizeof(struct employee));

      int front = 0;
      int rear = 0;
      struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);
      struct employee* temp = office->department_head;

      if(level == 0) {
          *n_employees += 1;
          (*emplys)[0] = *temp;
      } else {
          // up is upper, emplys is n_level start from 1;
          struct employee* up = temp;
          int n_up = 1;
          for(int i = 0; i <level; i++) {

              *n_employees = 0;
              *emplys = realloc(*emplys, sizeof(struct employee) * *n_employees);

              for(int j = 0; j < n_up; j++) {
                  if(up[j].subordinates) {
                      for(int l = 0; l < up[j].n_subordinates; l++) {
                          *n_employees += 1;
                          *emplys = realloc(*emplys, sizeof(struct employee) * *n_employees);
                          (*emplys)[*n_employees-1] = up[j].subordinates[l];
                      }
                  }
              }
              n_up = *n_employees;
              up = *emplys;
          }
      }
      free(queue);

}


void office_get_employees_by_name(struct office* office, const char* name,
  struct employee** emplys, size_t* n_employees) {
      if(office == NULL || name == NULL) {
          return;
      }
      *n_employees = 0;
      *emplys = malloc(sizeof(struct employee));
      int front = 0;
      int rear = 0;
      struct employee** queue = malloc(sizeof(struct employee*)*MAX_Q_SIZE);
      struct employee* temp = office->department_head;

      while(temp) {

          if(strcmp(temp->name, name) == 0) {
              *n_employees += 1;
              *emplys = realloc(*emplys, sizeof(struct employee) * *n_employees);
              (*emplys)[*n_employees - 1] = *temp;
          }
          if(temp->subordinates) {
              for(int i = 0; i < temp->n_subordinates; i++) {
                  enQueue(queue, &rear, temp->subordinates+i);
              }
          }
          if(front == rear) {
              break;
          }
          temp = deQueue(queue, &front);

      }
      free(queue);
}


void post(struct employee* emp, struct employee** emplys, size_t* n_employees) {
	if(!emp) {
		return;
	}
	for(int i = 0; i < emp->n_subordinates; i++) {
		post(&emp->subordinates[i], emplys, n_employees);
        *n_employees += 1;
        *emplys = realloc(*emplys, sizeof(struct employee) * *n_employees);
        (*emplys)[*n_employees-1] = emp->subordinates[i];
	}
}

void office_get_employees_postorder(struct office* off,
  struct employee** emplys,
  size_t* n_employees) {
      if(off == NULL) {
          return;
      }

      *emplys = malloc(sizeof(struct employee));
      *n_employees = 0;

	  post(off->department_head, emplys, n_employees);

      *n_employees += 1;
      *emplys = realloc(*emplys, sizeof(struct employee) * *n_employees);
      (*emplys)[*n_employees-1] = *off->department_head;
}



void office_disband(struct office* office) {
    free(office);
}

void office_promote_employee(struct employee* emp) {}
void office_demote_employee(struct employee* supervisor, struct employee* emp){
}




// int main() {
//     struct office* off = malloc(sizeof(struct office));
//     off->department_head = NULL;
//
//     struct employee emp1 = {
//         .name = "Employee1",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
//     struct employee emp2 = {
//         .name = "Employee2",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
//     struct employee emp3 = {
//         .name = "Employee3",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
//     struct employee emp4 = {
//         .name = "Employee4",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
// 	struct employee emp5 = {
//         .name = "Employee5",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
//     struct employee emp6 = {
//         .name = "Employee6",
//         .supervisor = NULL,
//         .subordinates = NULL,
//         .n_subordinates = 0
//     };
//
//     puts("-------------------------");
//     office_employee_place(off, NULL, &emp1);
//     office_employee_place(off, NULL, &emp2);
//     office_employee_place(off, NULL, &emp3);
//     office_employee_place(off, NULL, &emp4);
//     office_employee_place(off, NULL, &emp5);
//
//     struct employee* emplys = NULL;
//     size_t n_employees = 0;
//
//     office_get_employees_postorder(off, &emplys, &n_employees);
//
//     printf("%zu\n", n_employees);
//     printf("%s\n", emplys[4].name);
//
//
//
//
//   return 0;
// }
