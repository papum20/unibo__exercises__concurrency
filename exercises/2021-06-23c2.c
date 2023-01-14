#define PRIORITY_MAX

stack<semaphore> stacks[PRIORITY_MAX + 1];
semaphore mutex = semaphore(1);

void PLP(int prio) {
	mutex.P();
	if(value <= 0) {
		semaphore sem = semaphore(0);
		stacks[prio].push(sem);
		mutex.V();
		sem.P();
	} else {
		value--;
		mutex.V();
	}
}

void PLV() {
	mutex.P();
	for(int i = 0; i <= PRIORITY_MAX; i++) {
		if(!stacks[i].empty()) {
			sem = stacks[i].pop();
			sem.V();
			return;
		}
	}
	value++;
	mutex.V();
}
