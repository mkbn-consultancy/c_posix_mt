#include <pthread.h>
#include <semaphore.h>

// function prototypes
void execute(void (*somefunction)(void *p), void *p);
int pool_submit(void (*somefunction)(void *p), void *p);
void *worker(void *param);
void pool_init(void);
void pool_shutdown(void);

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

struct Threadpool
{
    TaskQueue tasks;
    pthread_mutex_t mutex;  //for locking the Queue
    sem_t semaphore;
    int timespan;
};