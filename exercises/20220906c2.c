semaphore mutex(1)
queue<semaphore> processes = new list<semaphore>()
int sum = 0

void sumstop(int v):
	mutex.P()
	sum += v
	semaphore sem = new semaphore(0)
	processes.push(sem)
	mutex.V()
	sem.P()
	free(sem)

int sumgo(void):
	mutex.P()
	int res = sum
	while(!processes.isEmpty()):
		semaphore s = processes.pop()
		s.V()
	sum = 0
	mutex.V()
	return res
