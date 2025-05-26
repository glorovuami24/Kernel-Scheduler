#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "job.h"
#include "cpu.h"
#include "list.h"

// Global Stride list
static Node *stride_list = NULL;
static int global_job_id_stride = 0;

void enqueueJobStride(char *job_name, int priority, int burst, int start_time, int deadline) {
    Job *newJob = malloc(sizeof(Job));
    newJob->job_name = strdup(job_name);
    newJob->priority = priority; //as amount of time that job runs for
    newJob->burst = burst; //time needed  for completion
    newJob->start_time = start_time;
    newJob->deadline = 0; //as time counter
    newJob->job_id = global_job_id_stride++;
    
    Node *newNode = malloc(sizeof(Node));
    newNode->job = newJob;
    newNode->next = NULL;
    
    // Append to tail.
    if (stride_list == NULL) {
        stride_list = newNode;
    } else {
        Node *temp = stride_list;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}

void minStart(Node *min_time, int *current_time) {
    int min_start = -1;
    Node *temp = stride_list;
    while (temp != NULL) {
        if (min_start == -1 || temp->job->start_time < min_start) {
            min_start = temp->job->start_time;
            min_time = temp;
        }
        temp = temp->next;
    }
    if (*current_time < min_start) *current_time = min_start;
}

void minTimeJob(Node *min_time, int *current_time) {
    Node *temp = stride_list;
    while (temp != NULL) {
        if ((*current_time >= temp->job->start_time) &&
            (min_time->job->deadline > temp->job->deadline)) {
                min_time = temp;
            }
        temp = temp->next;
    }
    min_time->job->deadline += min_time->job->priority;
    *current_time += min_time->job->priority;
}

void startJob(Node *min_time, int *current_time, float *total_response_time) {
    if ((min_time->job->job_id & 0x100000) == 0) { //job started
        *total_response_time += (*current_time - min_time->job->start_time);
        min_time->job->job_id |= 0x100000;  // Mark as started
    }
}

void checkFinished(Node *min_time, int *current_time) {
    if (min_time->job->burst <= min_time->job->deadline) {
        *current_time -= min_time->job->deadline - min_time->job->burst; //reduce time from too completed job
        min_time->job->burst = min_time->job->deadline;
    }
}

void finishJob(Node *min_time, int *current_time, float *total_turnaround_time, float *total_waiting_time, int *count) {
    if (min_time->job->burst == min_time->job->deadline) {
        int turnaround_time = *current_time - min_time->job->start_time;
        int waiting_time = turnaround_time - min_time->job->burst;  
        *total_turnaround_time += turnaround_time;
        *total_waiting_time += waiting_time;
        *count++;
        if (min_time == stride_list) { 
            stride_list = stride_list->next;
        } else {
            Node *prev = stride_list;
            while (prev->next && prev->next != min_time)
                prev = prev->next;
        
            if (prev->next == min_time)
                prev->next = min_time->next; //remove finished job from list
        }
        free(min_time->job->job_name);
        free(min_time->job);
        free(min_time);
    }
}

void printJobs(int current_time) {
    Node *temp = stride_list;
    printf("time:%d\n",current_time);
    while (temp != NULL) {
        printf("%s:%d ",temp->job->job_name, temp->job->deadline);
        temp = temp->next;
    }
    printf("\n");
}

void printMetrics(int current_time, float total_turnaround_time, float total_waiting_time, float total_response_time, int count) {
    printf("\nStride Scheduling Metrics:\n");
    printf("Average waiting time = %.2f\n", total_waiting_time / count);
    printf("Average turnaround time = %.2f\n", total_turnaround_time / count);
    printf("Average response time = %.2f\n", total_response_time / count);
}

 void runSchedulerStride() {
    int current_time = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_response_time = 0;
    int count = 0;

    while (stride_list != NULL) { //untill all jobs are finished
        Node *min_time = stride_list;
        minStart(min_time, &current_time);
        minTimeJob(min_time, &current_time);
        startJob(min_time, &current_time, &total_response_time);
        checkFinished(min_time, &current_time);
        printJobs(current_time);
        finishJob(min_time, &current_time, &total_turnaround_time, &total_waiting_time, &count);
    }
    printMetrics(current_time, total_turnaround_time, total_waiting_time, total_response_time, count);
}

