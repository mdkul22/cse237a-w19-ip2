#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <stdio.h>

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h

// Assignment: You need to implement the following two functions.

// learn_workloads(SharedVariable* v):
// This function is called at the start part of the program before actual scheduling
// - Parameters
// sv: The variable which is shared for every function over all threads
void learn_workloads(SharedVariable* sv) {
	// TODO: Fill the body
	// This function is executed before the scheduling simulation.
	// You need to calculate the execution time of each thread here.
	long long time = 0;
	long long duration = 0;
	printf("Performing threads to check performance\n");

	printf("thread_button time in us\n");
	time = get_current_time_us();
	thread_button(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[0] = duration;

	printf("thread_threecolor time in us\n");
	time = get_current_time_us();
	thread_threecolor(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[1] = duration;

	printf("thread_big time in us\n");
	time = get_current_time_us();
	thread_big(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[2] = duration;

	printf("thread_small time in us\n");
	time = get_current_time_us();
	thread_small(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[3] = duration;

	printf("thread_touch time in us\n");
	time = get_current_time_us();
	thread_touch(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[4] = duration;

	printf("thread_rgbcolor time in us\n");
	time = get_current_time_us();
	thread_rgbcolor(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[5] = duration;

	printf("thread_aled time in us\n");
	time = get_current_time_us();
	thread_aled(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[6] = duration;

	printf("thread_buzzer time in us\n");
	time = get_current_time_us();
	thread_buzzer(sv);
	duration = get_current_time_us() - time;
	printf("Duration: %lld\n", duration);
	sv->duration[7] = duration;
	for(int j=0; j<8; j++)
	{
	sv->tasks[j] = j;
	}
	for(int j = 0; j < 8; j++){
		for(int p = j+1; p < 8; p++)
		{
			int temp;
			if(workloadDeadlines[j]>workloadDeadlines[p])
			{
				temp = sv->tasks[j];
				sv->tasks[j] = sv->tasks[p];
				sv->tasks[p] = temp;
			}
		}
		printf("%d\t", sv->tasks[j]);
	}
	printf("\n");
}

	// Thread functions for workloads:
	// thread_button, thread_threecolor, thread_big, thread_small,
	// thread_touch, thread_rgbcolor, thread_aled, thread_buzzer

	// Tip 1. You can call each workload function here like:
	// thread_button();

	// Tip 2. You can get the current time here like:
	// long long curTime = get_current_time_us();


// select_task(SharedVariable* sv, const int* aliveTasks):
// This function is called while runnning the actual scheduler
// - Parameters
// sv: The variable which is shared for every function over all threads
// aliveTasks: an array where each element indicates whether the corresponed task is alive(1) or not(0).
// idleTime: a time duration in microsecond. You can know how much time was waiting without any workload
//           (i.e., it's larger than 0 only when all threads are finished and not reache the next preiod.)
// - Return value
// TaskSelection structure which indicates the scheduled task and the CPU frequency
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
	// TODO: Fill the body
	// This function is executed inside of the scheduling simulation.
    // You need to implement an energy-efficient EDF (Earliest Deadline First) scheduler.

	// Tip 1. You may get the current time elapsed in the scheduler here like:
	//long long curTime = get_scheduler_elapsed_time_us();

	// Also, do not make any interruptable / IO tasks in this function.
	// You can use printfDBG instead of printf.

	// Sample scheduler: Round robin
	// It selects a next thread using aliveTasks.
	static int prev_selection = -1;
	int i = prev_selection + 1;
	while(1) {
		if (i == NUM_TASKS)
			i = 0;
		if(aliveTasks[sv->tasks[i]]==1)
		{
			prev_selection = sv->tasks[i];
			for(int j=0; j<i;j++)
			{
				if(aliveTasks[sv->tasks[j]]==1){
				prev_selection = sv->tasks[j];
				break;
				}
			}
			break;
		}
		++i;
	}
	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = 0; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)
  return sel;
}
