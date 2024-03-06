

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *list;
int pid = 1;


void add(char *name, int priority, int burst) {
	Task *newTask;
	newTask = malloc(sizeof(Task));
	newTask->name = name;
	newTask->tid = pid;
	newTask->priority = priority;
	newTask->burst = burst;
	newTask->preEmpt = 0;
	pid++;

	insert_priority(&list, newTask);
}

void schedule() {
	struct node *temp = list;
	struct node *rr;
	struct node *itr;
	double totalTurn = 0.0;
	double totalWait = 0.0;
	double totalResp = 0.0;
	int samePri = 0;
	int totalTime = 0;

	
	while (temp != NULL) {
		itr = temp;
		insert_head(&rr, temp->task);
		while (itr->next != NULL
				&& itr->task->priority == itr->next->task->priority) {
			insert_head(&rr, itr->next->task);
			itr = itr->next;
			temp = itr;
			samePri++;
		}

		if (samePri > 0) {
			itr = rr;
		
			while (more_run(&itr)) {
				while (itr != NULL) {
					if (itr->task->burst > 0 && itr->task->burst <= QUANTUM) {
						totalTime += QUANTUM;
						run(itr->task, QUANTUM);
						itr->task->burst = 0;
						itr->task->endTime = totalTime;
						totalTurn += totalTime;
					} else if (itr->task->burst > 0) {
						itr->task->preEmpt = itr->task->preEmpt + 1;
						totalTime += QUANTUM;
						run(itr->task, QUANTUM);
						itr->task->burst = itr->task->burst - QUANTUM;
					}
					itr = itr->next;
				}
				itr = rr;
			}
	
			for (int i = 0; i <= samePri; i++)
				totalResp += (QUANTUM * i);
		
			itr = rr;
			while (itr != NULL) {
				totalWait += (itr->task->endTime - QUANTUM)
						- (itr->task->preEmpt * QUANTUM);
				itr = itr->next;
			}
		}

		else {
			totalWait += totalTime;
			totalResp += totalTime;
			run(temp->task, temp->task->burst);
			totalTime += temp->task->burst;
			totalTurn += totalTime;
		}
		
		rr = NULL;
		samePri = 0;
		temp = temp->next;
	}

	printf("Priority with Round Robin: The Average Turnaround time, Average wait time, and Average Response time are below respectively:")
	printf( avgTurn / (pid - 1));
	printf( avgWait / (pid - 1));
	printf( avgWait / (pid - 1));
}

//Round Robin implementation
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
