class semaphore {
	int value;
	//int blocked = 0;
	binary_semaphore mutex(1);
	//binary_semaphore sem(0);
	queue<semaphore> q;

	void init(int initval) {
		value = initval;
		q = new queue();
	}

	void P() {
		mutex.P();
		if(value == 0) {
			//blocked++;
			//sem.P();
			semaphore s = new semaphore(0);
			q.push(s);
			mutex.V();
			s.P();
			free(s);
			blocked--;
		}
		value--;
		mutex.V();
	}

	void V() {
		mutex.P();
		value++;
		//if(blocked > 0) {
		if(q.size() > 0) {
			semaphore s = q.pop();
			s.V();	
		}
		else
			mutex.V();
	}

}
