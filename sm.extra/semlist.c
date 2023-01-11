#include<semlist.h>

struct semlist {
	struct semlist *next;
	semaphore sem;
};

void semlist_enqueue(struct semlist **tp, semaphore sem) {
	struct semlist *new = malloc(sizeof(*new));
	if (new) {
		new->sem = sem;
		if (*tp) {
			struct semlist *last = *tp;
			new->next = last->next;
			last->next = new;
		} else 
			new->next = new;
		*tp = new;
	}
}

void semlist_push(struct semlist **tp, semaphore sem) {
	struct semlist *new = malloc(sizeof(*new));
	if (new) {
		new->sem = sem;
		if (*tp) {
			struct semlist *last = *tp;
			new->next = last->next;
			last->next = new;
		} else {
			new->next = new;
			*tp = new;
		}
	}
}

static semaphore semlist_get(struct semlist **tp) {
	if (*tp) {
		struct semlist *last = *tp;
		struct semlist *first = last->next;
		semaphore rval = first->sem;
		if (first == first->next)
			*tp = NULL;
		else
			last->next = first->next;
		free(first);
		return rval;
	} else
		return NULL;
}

semaphore semlist_dequeue(struct semlist **tp) {
	return semlist_get(tp);
}

semaphore semlist_pop(struct semlist **tp) {
	return semlist_get(tp);
}

