#ifndef SCHEDULER_H
#define SCHEDULER_H

// SJF scheduler with start time support (provided earlier)
void enqueueJobSJF(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerSJF();

// EDF scheduler (deadline scheduling) (provided earlier)
void enqueueJobEDF(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerEDF();

// First-Come First-Served (FCFS) scheduler
void enqueueJobFCFS(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerFCFS();

// Round Robin (RR) scheduler
void enqueueJobRR(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerRR();

// Priority scheduler (non-preemptive)
void enqueueJobPriority(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerPriority();

// Priority Round Robin scheduler
void enqueueJobPriorityRR(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerPriorityRR();

// Stride scheduler
void enqueueJobStride(char *job_name, int priority, int burst, int start_time, int deadline);
void runSchedulerStride();

#endif
