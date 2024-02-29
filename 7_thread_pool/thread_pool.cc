#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10 // not used in this code
#define NUMBER_OF_THREADS 3

#define TRUE 1
#define DEBUG 1 // 0 - no debug message, 1 - main points, 2 - all

#define LIFE 2 // amount of seconds threads will live

// struct Task worktodo;
// struct Threadpool* pool;
// time_t begin;

// pthread_t bee; // the worker bee

int i = 0; // task_id debugging
int j = 0; // thread_id debugging


struct Task
{
    void (*function)(void *p);  //the task
    void *data; 

    struct Task *next;
    int task_id; //debug purpose
};

struct TaskQueue{
    struct Task *head;  //insert new Task
    struct Task *tail;  //pop Task
};

bool is_empty(TaskQueue* q){
    return q->head==NULL;
}

struct Threadpool
{
    TaskQueue* tasks;
    pthread_mutex_t mutex;  //for locking the Queue
    sem_t semaphore;
    int timespan;
};


// insert a task into the queue
int add_task(ThreadPool* pool, struct Task* t) 
{
    struct TaskQueue* q = pool->tasks;
    if (q->is_empty()) { // pool is currently empty
        if (DEBUG > 1) printf("Pool is currently empty...\n");
        q->head = t;
        q->tail = t;
    } else {
        if (DEBUG > 1) printf("Pool isn't currently empty...\n");
        q->tail->next = t;
        q->tail = t;
    }
    if (DEBUG > 1) printf("Task with id = %d added...\n", t->task_id);
    sem_post(&pool->semaphore);
    return 0;
}

// remove a task from the queue
struct Task* pop_task(ThreadPool* pool) 
{
    struct Task *worktodo;
    struct TaskQueue* q = pool->tasks;

    worktodo = q->head;
    if (worktodo == NULL) { // dequeue was required in an empty thread
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

// the worker thread in the thread pool
void* worker(void *param)
{
    int thread_id = (int) param;
    if (DEBUG){ 
        printf("Start work of thread %u...\n", thread_id);
    }
    struct Task *task;
    int num_of_works = 0;
    time_t now;
    while (TRUE) {
        now = time(NULL);
        if (now - begin > pool->timespan) break; // stop condition

        sem_wait(&pool->semaphore); //wait for new task to be inserted into the pool
      
        pthread_mutex_lock(&pool->mutex);

        // if (DEBUG && pool->head){ 
        //     printf(".locking in thread %u...\n", thread_id);
        // }
        if (pool->tasks->head == NULL) {
            pthread_mutex_unlock(&pool->mutex); //no work to do
        } else {
            task = pop_task(&pool);
            ++num_of_works;
            // if (DEBUG > 1){ 
            //     printf("Thread %u got job of adding %u + %u...\n", thread_id, *((int*) task->data), *((int*) task->data + 1));
            // }
            pthread_mutex_unlock(&pool->mutex);
            // if (DEBUG){
            //     printf(".unlocking in thread %u...\n", thread_id);
            // }
            execute(task->function, task->data);
        }
        sem_post(&pool->semaphore);
    }
    // if (DEBUG) printf("Closing thread %u, that performed %u works...\n",thread_id, num_of_works);
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
int pool_submit(void (*somefunction)(void *p), void *p)
{
    pthread_mutex_lock(&pool->mutex);
    if (DEBUG > 1) printf("New task in the pool... Locking\n");
    struct task *worktodo = (struct task*) malloc(sizeof(struct task));
    worktodo->function = somefunction;
    worktodo->data = p;
    worktodo->task_id = i;
    worktodo->next = NULL;
    ++i;
    if (DEBUG > 1) {
        printf("work.a = %u ... ", *((int*) worktodo->data));
        printf("work.b = %u\n", *((int*) worktodo->data + 1));
        //printf(">>> *((int*) worktodo.data) == worktodo.b = %u\n", *((int*) (worktodo.data + sizeof(int))));
    }
    enqueue(worktodo);
    if (DEBUG > 1) printf("Enqueued. Unlocking execution...\n");
    pthread_mutex_unlock(&pool->mutex);
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    pool = (struct threadpool*) malloc(sizeof(struct threadpool));
    pool->head = NULL;
    pool->tail = NULL;
    pool->timespan = LIFE; //seconds
    pthread_mutex_init(&pool->mutex, NULL);
    sem_init(&pool->semaphore, 0, NUMBER_OF_THREADS);
    begin = time(NULL);
    for (int i = 0; i < NUMBER_OF_THREADS; ++i, ++j) {
        pthread_create(&bee, NULL, worker, (void *) j);
    }
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee,NULL);
    if (DEBUG) printf("End of execution :)\n");
}