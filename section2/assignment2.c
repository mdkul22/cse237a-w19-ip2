#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
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

	time = get_current_time_us();
	thread_button(sv);
	duration = get_current_time_us() - time;
	sv->duration[0] = duration;

	time = get_current_time_us();
	thread_threecolor(sv);
	duration = get_current_time_us() - time;
	sv->duration[1] = duration;

	time = get_current_time_us();
	thread_big(sv);
	duration = get_current_time_us() - time;
	sv->duration[2] = duration;

	time = get_current_time_us();
	thread_small(sv);
	duration = get_current_time_us() - time;
	sv->duration[3] = duration;

	time = get_current_time_us();
	thread_touch(sv);
	duration = get_current_time_us() - time;
	sv->duration[4] = duration;

	time = get_current_time_us();
	thread_rgbcolor(sv);
	duration = get_current_time_us() - time;
	sv->duration[5] = duration;

	time = get_current_time_us();
	thread_aled(sv);
	duration = get_current_time_us() - time;
	sv->duration[6] = duration;

	time = get_current_time_us();
	thread_buzzer(sv);
	duration = get_current_time_us() - time;
	sv->duration[7] = duration;
	float util = 0;
	for(int j=0; j<8; j++)
	{
	sv->tasks[j] = j;
	sv->prev_Alive[j] = 0;
	sv->prevTime[j] = 0;
	util += sv->duration[j]/workloadDeadlines[j];
	if(util > 1)
	{
	printDBG("Util beyond 100%");
	}
	}
	sv->oldUtil = util;
	sv->newUtil = 0;
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
	int temp =0;
	long long deadline = 100000000;
	while(1){
	for(int j=0; j < NUM_TASKS; j++)
		{
			// task finished previously
			if((aliveTasks[j] == 0) && (sv->prev_Alive[j]==1))
			{
				sv->realDeadline[j] = 0;
			}
			// task newly created
			else if((aliveTasks[j]== 1) && (sv->prev_Alive[j]==0))
			{
				sv->realDeadline[j] = workloadDeadlines[j];
				sv->prevTime[j] = get_scheduler_elapsed_time_us();
				sv->currExec[j] = sv->duration[j];
				sv->newUtil += (float)sv->currExec[j]/(float)sv->realDeadline[j];
				if(sv->realDeadline[j]<deadline){
					deadline = sv->realDeadline[j];
					prev_selection = j;
				}
			}
			// task is old
			else if((aliveTasks[j]== 1) && (sv->prev_Alive[j]==1))
			{
				sv->realDeadline[j] = sv->realDeadline[j] - (get_scheduler_elapsed_time_us() - sv->prevTime[j]);
				sv->prevTime[j] = get_scheduler_elapsed_time_us();
				sv->newUtil += (float)sv->curreExec[j]/(float)sv->realDeadline[j];
				if(sv->realDeadline[j]<deadline){
					deadline = sv->realDeadline[j];
					prev_selection = j;
				}
			}
			sv->prev_Alive[j] = aliveTasks[j];
		}
		if(prev_selection != -1)
		{
			break;
		}
	}
	printDBG("%.2f : New Util    %.2f  :  Old Util\t", sv->newUtil, sv->oldUtil);
	printDBG("T%d's Deadline: %lld and idle time: %lld\n", prev_selection, (sv->realDeadline[prev_selection]+get_scheduler_elapsed_time_us())/1000, idleTime);
	// The retun value can be specified like this:
	sv->currExec[prev_selection] -= 10000;
	if(sv->realDeadline[prev_selection]<10000)
	{
		sv->prev_Alive[prev_selection] = 0;
		sv->currExec[prev_selection] = 0;
	}
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	if(sv->newUtil>sv->oldUtil)
	{
		sel.freq = 1;
	}
	else
	{
		sel.freq = 0;
	}
	sv->oldUtil=sv->newUtil;
	sv->newUtil=0;
  return sel;
}
