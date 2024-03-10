## LLF Scheduling Algorithm

This program implements the Least Laxity First (LLF) scheduling algorithm for periodic tasks. It reads task information from a file and schedules the tasks based on their periods, burst times, and deadlines.

### Input Structure

The input file (`input.txt`) should have the following structure:

- `<number_of_tasks>`: An integer specifying the total number of tasks to be scheduled.
- `<task_id>`: An integer representing the unique identifier for each task.
- `<period>`: A double representing the period of the task (time between consecutive arrivals).
- `<burst_time>`: A double representing the execution time (burst time) of the task.

### Execution Commands

1. **Compile the Program:**
   ```bash
   gcc -o llf_scheduler llf_scheduler.c -lm
   
2. **Execution**
   ```bash
   ./llf_scheduler
   ```

### Output 

The program will output the scheduling details to the console, including :

- The start and end times of task execution
- Task IDs
- Remaining times
