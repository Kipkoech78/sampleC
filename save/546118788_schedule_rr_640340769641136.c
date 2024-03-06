

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"

struct node *list;
int pid = 1;

// add a task to the list 
void add(char *name, int priority, int burst) {
	Task *newTask;
	newTask = malloc(sizeof(Task));
	newTask->name = name;
	newTask->tid = pid;
	pid++;
	newTask->priority = priority;
	newTask->burst = burst;
	newTask->preEmpt = 0;
	insert_tail(&list, newTask);
}

void schedule() {
	struct node *temp = list;
	double totalTurn = 0.0;
	double totalWait = 0.0;
	double totalResp = 0.0;
	int totalTime = 0;


	while (more_run(&temp)) {
		//iterate 
		while (temp != NULL) {
			if (temp->task->burst > 0 && temp->task->burst <= QUANTUM) {
				totalTime += QUANTUM;
				run(temp->task, QUANTUM);
				temp->task->burst = 0;
				temp->task->endTime = totalTime;
				totalTurn += totalTime;
			}
			else if (temp->task->burst > 0) {
				temp->task->preEmpt = temp->task->preEmpt + 1;
				totalTime += QUANTUM;
				run(temp->task, QUANTUM);
				temp->task->burst = temp->task->burst - QUANTUM;
			}
			temp = temp->next;
		}
		
		temp = list;
	}

	for (int i = 0; i < pid - 1; i++)
		totalResp += (QUANTUM * i);

	temp = list;

	while (temp != NULL) {
		totalWait += (temp->task->endTime - QUANTUM)
						- (temp->task->preEmpt * QUANTUM);
		temp = temp->next;
	}
	printf("Round Robin: The Average Turnaround time, Average wait time, and Average Response time are below respectively:")
	printf( avgTurn / (pid - 1));
	printf( avgWait / (pid - 1));
	printf( avgWait / (pid - 1));

}


bool more_run(struct node **head) {
	bool more = false;
	struct node *temp = *head;

	while (temp != NULL) {
		if (temp->task->burst > 0)
			more = true;
		temp = temp->next;
	}
	return more;
}
