
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *list;
int pid = 1;

// add a task to the list 
void add(char *name, int priority, int burst) {
	Task *newTask;
	newTask = malloc(sizeof(Task));
	newTask->name = name;
	newTask->tid = pid;
	newTask->priority = priority;
	newTask->burst = burst;
	pid++;

	insert_shortest(&list, newTask);
}

void schedule() {
	struct node *temp = list;
	double avgTurn = 0.0;
	double avgWait = 0.0;
	int end = 0;


	while (temp != NULL) {
		temp->task->startTime = end;
		avgWait += end;
		end = end + temp->task->burst;
		temp->task->endTime = end;
		avgTurn += end;
		run(temp->task, temp->task->burst);
		temp = temp->next;
	}

	printf("SJF: The Average Turnaround time, Average wait time, and Average Response time are below respectively:")
	printf( avgTurn / (pid - 1));
	printf( avgWait / (pid - 1));
	printf( avgWait / (pid - 1));

}
