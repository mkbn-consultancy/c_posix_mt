class ThreadWrapper{
	typedef void*(*thread_init_func_t)(void*);
public:
	ThreadWrapper(int id) : _workerID(id), _bIsWorking(false){}
	void initThread(thread_init_func_t func, IJobQueue* pJobs);
private:
	int		_workerID;
	bool		_bIsWorking;
	pthread_t	_theThread;
	IJobQuque* 	_jobsPool;
};

#include "workerthread.h"

void ThreadWrapper::initThread(thread_init_func_t func, IJobQueue* pJobs){
	//create the thread and make him start pooling the jobs queue
	pthread_create(&_theThread, NULL, func, (void*)pJobs);
}


/////////////////////////////////////

class IJobQueue{
public:
	virtual IJob* loadJob() = 0;
};


class TreadPool : public IJobQueue{
public:
	ThreadPool(int numThreads);

	void addJob(IJob* job);
	virtual IJob* loadJob();
//	std::list<IJobs*> getDoneJobs();
protected:
	void initialize();
	void scheduler();
	void jobWrapper();
	//the pooling method that each thread should run
	static void* threadExecute(void*);
private:
	int				_poolSize;
	std::vector<WorkerThread>	_threads;
	std::queue<IJOb*>	_waitingJobs;

	//mutex & semaphores
	static pthread_mutex_t	_lockForJobQueue;
	static sem_t _semForJobInsert;
	static sem_t _semForJobExtract;
};

#include "threadpool.h"

//init static members
static pthread_mutex_t ThreadPool::_lockForJobQueue = PTHREAD_MUTEX_INITIALIZER;
static sem_t ThreadPool::_semForJobInsert;
static sem_t ThreadPool::_semForJobExtract;

ThreadPool::ThreadPool(int numThreads)  : _poolSize(numThreads) {
	//init the semaphores:
	sem_init(&_semForJobInsert, 0, 0);
	sem_init(&_semForJobExtract, 0, _poolSize);
}

void ThreadPool::initialize(){
	//initialize the threads with the working function.
	std::vector<WorkerThread>::iterator itr = _threads.begin();
	for(; itr!=_threads.end(); ++itr){
		itr->initThread(ThreadPool::threadExecute, this);
	}
}

void ThreadPool::addJob(IJob* job){
	//wait for space in the Q to insert a new job
	sem_wait(&_senForJobInsert);

	//lock the Q
	pthread_mutex_lock(&_lockForJobQueue);
	_waitingJobs.push(job);

	//update the workers that there is a new job in the queue
	sem_post(&_semForJobExtract);
	pthread_mutex_unlock(&_lockForJobQueue);
}

void* ThreadPool::threadExecute(void* param){
	//cast the param to get "this" in order to access the jobs pool
	ThreadPool* pTP = (ThreadPool*)param;
	//wait for new jobs in the queue
	sem_wait(&_semForJobExtract);
	//lock to extract a job from the queue
	pthread_mutex_lock(&_lockForJobQueue);
	IJob* pJob = pTP->loadJob();
	pthread_mutex_unlock(&_lockForJobQueue);
	pJob->execute();
	sem_post(&_semForJobInsert); //there's a room for another job to insert the Q.
}

/////////////////////////////////////

int main(){
	ThreadPool thrdpool(5);

	std::vector<IJob*> tasks;
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));
	tasks.push_back(JobFactory::get(" "));

	//run the tasks with the thread pool
	int taskNum = tasks.size();
	for(int i=0; i<taskNum; ++i){
		thrdpool.addJob(tasks[i]);
	}

	while(true){
	}
}

