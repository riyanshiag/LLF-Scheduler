#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

struct process {
	int id;
	double arrival_time;
	double burst_time;
	double remaining_time;
	double deadline;
	double period;
};

void LLF_schedule(struct process proc[], int n)
{
	double time = 0.0;

	// Find process with maximum laxity (lowest priority)
	double max_laxity = 0;
	int max_laxity_proc = -1;
	for (int i = 0; i < n; i++) {
		//printf("Process %d: arrival_time = %.1f, burst_time = %.1f, deadline = %.1f, period = %.1f\n", proc[i].id, proc[i].arrival_time, proc[i].burst_time, proc[i].deadline, proc[i].period);
		double laxity = proc[i].deadline - time - proc[i].burst_time;
		if (laxity > max_laxity) {
			max_laxity = laxity;
			max_laxity_proc = i;
		}
	}

	int last_exec_process = max_laxity_proc;
	while (true) {
		// Find process with minimum laxity (highest priority)
		double min_laxity = proc[max_laxity_proc].deadline - time - proc[max_laxity_proc].remaining_time;
		int exec_process = max_laxity_proc;
		for (int i = 0; i < n; i++) {
			if (proc[i].arrival_time > time)
				continue;
			double laxity = proc[i].deadline - time - proc[i].remaining_time;
			if (laxity < min_laxity) {
				min_laxity = laxity;
				exec_process = i;
			}
		}

		// Execute the last executed process if it is different from the currently chosen process and has the same laxity (to avoid context switch)
		if ((exec_process != last_exec_process) && proc[last_exec_process].arrival_time < time && (fabs(proc[last_exec_process].deadline - proc[last_exec_process].remaining_time - proc[exec_process].deadline + proc[exec_process].remaining_time) < 0.01))
			exec_process = last_exec_process;

		// Execute the process with minimum laxity
		proc[exec_process].remaining_time -= 0.1;
		printf("time: %.1fs to %.1fs, process: %d, remaining time: %.1f\n", time, time + 0.1, proc[exec_process].id, fabs(proc[exec_process].remaining_time));
		time += 0.1;						// Granularity of 0.1s
		last_exec_process = exec_process;	// Update the last executed process

		// Check if the currently executing job has completed execution
		// Break if the job with maximum laxity has finished execution, otherwise add a new job of the process
		if (fabs(proc[exec_process].remaining_time) <= 0.01) {
			if (exec_process == max_laxity_proc)
				break;
			proc[exec_process].arrival_time += proc[exec_process].period;
			proc[exec_process].remaining_time = proc[exec_process].burst_time;
			proc[exec_process].deadline += proc[exec_process].period;
			printf("Next job of task %d joining at time = %.1fs\n", proc[exec_process].id, proc[exec_process].arrival_time);
		}
		sleep(1);							// To slow down the output
	}
}

int main()
{
	FILE *file;

	file = fopen("input.txt", "r");

	if (file == NULL) {
		printf("Error opening file.\n");
		return 1;
	}

	int a;
	fscanf(file, "%d", &a);

	struct process procs[a];

	int n = 0;
	while (true) {
		int count = fscanf(file, "%d %lf %lf", &procs[n].id, &procs[n].period, &procs[n].burst_time);
		if (count == EOF || count != 3) {
			if (feof(file)) {
				printf("End of file reached.\n");
				break;
			}
			if (ferror(file)) {
				printf("Error reading process %d.\n", n + 1);
				clearerr(file);
				continue;
			}
			printf("Invalid process format on line %d.\n", n + 1);
			return 1;
		}

		procs[n].arrival_time = 0;
		procs[n].deadline = procs[n].period;
		procs[n].remaining_time = procs[n].burst_time;

		n++;
		if (n == a)
			break;
	}

	fclose(file);

	// int n = sizeof(procs) / sizeof(procs[0]);
	printf("%d tasks registered.\n", n);

	LLF_schedule(procs, n);

	return 0;
}
