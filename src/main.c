#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

#define MAX_LINE 256

/*
 * Input file format (CSV):
 *   job_name,priority,burst,start_time,deadline
 *
 * For jobs that do not use deadlines, set deadline to -1.
 * The scheduler type is specified as a command-line argument:
 *   "sjf"         => Shortest Job First scheduler (with start time support)
 *   "edf"         => Earliest Deadline First scheduler (deadline scheduling)
 *   "fcfs"        => First Come First Serve scheduler
 *   "rr"          => Round Robin scheduler
 *   "priority"    => Priority scheduler
 *   "priority_rr" => Priority Round Robin scheduler
 *   "stride"      => Stride Scheduling
 */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_file> <scheduler_type>\n", argv[0]);
        printf("Valid scheduler types: sjf, edf, fcfs, rr, priority, priority_rr\n");
        return 1;
    }
    
    char *filename = argv[1];
    char *scheduler_type = argv[2];
    
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening input file");
        return 1;
    }
    
    char line[MAX_LINE];
    
    // Read each line from the file.
    while (fgets(line, sizeof(line), fp)) {
        // Remove the newline character if present.
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;
        
        // Parse the CSV line: job_name,priority,burst,start_time,deadline
        char *token = strtok(line, ",");
        if (!token) continue;
        char *job_name = token;
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int priority = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int burst = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int start_time = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) continue;
        int deadline = atoi(token);
        
        // Enqueue the job into the chosen scheduler.
        if (strcmp(scheduler_type, "sjf") == 0) {
            enqueueJobSJF(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "edf") == 0) {
            enqueueJobEDF(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "fcfs") == 0) {
            enqueueJobFCFS(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "rr") == 0) {
            enqueueJobRR(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "priority") == 0) {
            enqueueJobPriority(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "priority_rr") == 0) {
            enqueueJobPriorityRR(job_name, priority, burst, start_time, deadline);
        } else if (strcmp(scheduler_type, "stride") == 0) {
            enqueueJobStride(job_name, priority, burst, start_time, deadline);
        } else {
            printf("Error: Unknown scheduler type: %s\n", scheduler_type);
            printf("Valid scheduler types: sjf, edf, fcfs, rr, priority, priority_rr\n");
            fclose(fp);
            return 1;
        }
    }
    
    fclose(fp);
    
    // Run the selected scheduler.
    if (strcmp(scheduler_type, "sjf") == 0) {
        runSchedulerSJF();
    } else if (strcmp(scheduler_type, "edf") == 0) {
        runSchedulerEDF();
    } else if (strcmp(scheduler_type, "fcfs") == 0) {
        runSchedulerFCFS();
    } else if (strcmp(scheduler_type, "rr") == 0) {
        runSchedulerRR();
    } else if (strcmp(scheduler_type, "priority") == 0) {
        runSchedulerPriority();
    } else if (strcmp(scheduler_type, "priority_rr") == 0) {
        runSchedulerPriorityRR();
    } else if (strcmp(scheduler_type, "stride") == 0) {
        runSchedulerStride();
    }

    return 0;
}
