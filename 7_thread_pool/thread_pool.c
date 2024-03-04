#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h> /* for sleep */
#include "thread_pool.h"
#include <sys/types.h>

#define QUEUE_SIZE 10 // not used in this code
#define NUMBER_OF_THREADS 3

#define TRUE 1

/* forward declarations */
void execute(void (*somefunction)(void *p), void *p);

struct Task
{
    void (*function)(void *p);  //the task
    void *data; 

    struct Task *next;
};

struct TaskQueue{
    struct Task *head;  //insert new Task
    struct Task *tail;  //pop Task
};

struct Threadpool
{
    struct TaskQueue* tasks;
    pthread_mutex_t mutex;  //for locking the Queue
    sem_t consumer_semaphore;
    sem_t producer_semaphore;
    int should_stop;
    pthread_t threads[NUMBER_OF_THREADS];
};


int is_empty(struct TaskQueue* q){
    return q->head==NULL;
}

// insert a task into the queue
void add_task(struct Threadpool* pool, struct Task* t) 
{
    struct TaskQueue* q = pool->tasks;
    if (is_empty(q)) { // pool is currently empty
        q->head = t;
        q->tail = t;
    } else {
        q->tail->next = t;
        q->tail = t;
    }
}

// remove a task from the queue
struct Task* pop_task(struct Threadpool* pool) 
{
    struct Task *worktodo;
    struct TaskQueue* q = pool->tasks;

    worktodo = q->head;
    if (worktodo == NULL) { // pop_task was required in an empty thread
        return NULL;
    }
    if (worktodo->next == NULL) { // dequeue made pool empty
        q->head = NULL;
        q->tail = NULL;
    } else {
        q->head = worktodo->next;
    }
    return worktodo;
}

/**
 * The function that each thread executes
 */
void* worker(void *task_param)
{
    printf("-->thread is start working...\n");
    struct Threadpool* pool = (struct Threadpool*)task_param;
    struct Task *task;
    while (TRUE) {  //work all the time for taking tasks from the queue
        if (pool->should_stop) {
            printf("should stop = %d, exiting the loop...\n", pool->should_stop);
            break; // stop working
        }
        pthread_mutex_lock(&(pool->mutex));   //lock since we are accessing the queue
        if (pool->tasks->head != NULL) {
            sem_wait(&(pool->consumer_semaphore)); //wait for new task to be inserted into the pool
            task = pop_task(pool); //get the next task from the thread pool
            printf("task popped and is executing by thread %lu...\n", pthread_self());
            sleep(1);
            sem_post(&pool->producer_semaphore); //signal that there is a place for a new task
            execute(task->function, task->data);
        }
        pthread_mutex_unlock(&pool->mutex); //no work to do, unlock to let others insert tasks
    }
    printf("thread finished working...\n");
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(struct Threadpool* pool, void (*somefunction)(void *p), void *p)
{
    printf("--->creating a new task...\n");
    pthread_mutex_lock(&pool->mutex);

    //create a new Task
    struct Task *worktodo = (struct Task*) malloc(sizeof(struct Task));
    worktodo->function = somefunction;
    worktodo->data = p;
    worktodo->next = NULL;
    //add the task to the thread pool
    add_task(pool, worktodo);
    
    sem_post(&pool->consumer_semaphore); //signal that one task has been added
    
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}

// initialize the thread pool
struct Threadpool* pool_init()
{
    printf("--->starting pool initialization...\n");
    struct Threadpool* pool = (struct Threadpool*) (malloc(sizeof(struct Threadpool)));
    pool->tasks = (struct TaskQueue*)malloc(sizeof(struct TaskQueue));
    pool->tasks->head = NULL;
    pool->tasks->tail = NULL;
    pool->should_stop = 0;
    pthread_mutex_init(&(pool->mutex), NULL);

    sem_init(&(pool->consumer_semaphore), 0, 0);
    sem_init(&(pool->producer_semaphore), 0, QUEUE_SIZE);
    //create all threads
    for (int i = 0; i < NUMBER_OF_THREADS; ++i) {
        pthread_create(&(pool->threads[i]), NULL, worker, (void *)(pool));
    }
    printf("done initializaing pool...\n");
    return pool;
}

// shutdown the thread pool
void pool_shutdown(struct Threadpool* pool)
{
    printf("--->pool is shutting down...\n");
    pool->should_stop = 1;

    //join all threads
    for(int i=0; i<NUMBER_OF_THREADS; ++i){
        pthread_join(pool->threads[i],NULL);
    }
}

