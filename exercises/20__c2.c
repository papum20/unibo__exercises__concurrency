list<semaphore> semaphores;
semaphore mutex = semaphore(1);
int sum = 0;


void sumstop(int v) {
	mutex.P();
	sum += v;
	semaphore sem = semaphore(0);
	semaphores.push(sem);
	mutex.V();
	sem.P();
}


int sumgo() {
	mutex.P();
	while(!semaphores.empty()) {
		semaphore sem = semaphores.pop();
		sem.V();
	}
	int tmp = sum;
	sum = 0;
	mutex.V();
	return tmp;
}
