#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* a read-write lock */
pthread_rwlock_t  lock = PTHREAD_RWLOCK_INITIALIZER;

/* a linked list node */
struct node_t {
    int data;
    struct node_t* next;
};


/* add an element at front of list */
void list_insert(struct node_t** head, int data) {
    struct node_t* new = malloc(sizeof(struct node_t));
    new->data = data;
    new->next = *head;
    *head = new;
}

/* remove the first instance of an element from a list */
void list_remove(struct node_t** head, int data) {
    struct node_t* previous = NULL;
    struct node_t* current = *head;
    while (current) {
        if (current->data == data) {
            if (previous) {
                previous->next = current->next;
            } else {
                *head = current->next;
            }
            free(current);
            return;
        }
        current = current->next;
    }
}

/* print a linked list to the screen */
void list_print(struct node_t* head) {
    struct node_t* current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

/* our list is global */
struct node_t* head = NULL;

/* thread function which inserts and removes elements */
void* writer(void* idp) {
    int seed = *((int*) idp);

    //write lock:
    pthread_rwlock_wrlock(&lock);
    printf("writer thread #%d locks to access the list\n", seed);
    list_insert(&head, rand_r(&seed) % 100);
    list_remove(&head, rand_r(&seed) % 100);
    list_remove(&head, rand_r(&seed) % 100);
    list_remove(&head, rand_r(&seed) % 100);
    pthread_rwlock_unlock(&lock);
    printf("writer thread #%d un-locks to access the list\n", seed);

    pthread_exit(NULL);
}

/* thread function which tests if elements are in the list */
void* reader(void* idp) {
    int id = *((int*) idp);

    pthread_rwlock_rdlock(&lock);
    printf("reader thread #%d lockes to read the list data\n", id);
    list_print(head);
    pthread_rwlock_unlock(&lock);
    printf("reader thread #%d un-lockes to read the list data\n", id);

    pthread_exit(NULL);
}

int main () {
    /* set the number of threads */
    const int num_threads = 10;
    /* an array of threads */
    pthread_t threads[2 * num_threads];
    pthread_rwlock_init(&lock, NULL);

    /* spawn all writer threads */
    int seeds[num_threads];
    for (int i = 0; i < num_threads; i++) {
        seeds[i] = i;
        pthread_create(&threads[i], NULL, writer, &seeds[i]);
    }

    /* spawn all reader threads */
    for (int i = num_threads; i < 2 * num_threads; i++) {
        pthread_create(&threads[i], NULL, reader, (void*)&i);
    }

    /* join all threads collecting answer */
    for (int i = 0; i < 2 * num_threads; i++) {
        pthread_join(threads[i], (void**) NULL);
    }

    pthread_exit(0);
}

