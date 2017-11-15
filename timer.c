#include "timer.h"

/*
//The following are suggestions for functions and data structures you will likely need.
typedef void (*handler_t)(void); //toggles led

//structure holding virtual timer info
//this may vary depending on your implementation
typedef struct Timer {
    handler_t	handler;//function pointer (called after timer period)
    uint32_t	time;//time remaining for this counter
    uint32_t	period;//period
    uint32_t	mode;//continuous or one shot timer
    struct Timer*  next;//points to next timer
} Timer_t;
*/
struct Timer *timer = NULL;

void swap(struct Timer *a, struct Timer *b)
{
    handler_t t_handler = a->handler;
    uint32_t t_period = a->period;
    uint32_t t_time = a->time;
    uint32_t t_mode = a->mode;

    a->handler = b->handler;
    a->period = b->period;
    a->time = b->time;
    a->mode = b->mode;

    b->handler = t_handler;
    b->period = t_period;
    b->time = t_time;
    b->mode = t_mode;
}

void Sort(Timer_t *head) {
	Timer_t *start = head;
	Timer_t *traverse;
	Timer_t *min;

	while(start->next && start)
	{
		min = start;
		traverse = start->next;

		while(traverse)
		{
			/* Find minimum element from array */
			if( min->time > traverse->time )
			{
				min = traverse;
			}

			traverse = traverse->next;
		}
		swap(start,min);			// Put minimum element on starting location
		start = start->next;
	}
}

//look in mss_timer.h for details
void start_hardware_timer(uint32_t period){

		MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
		MSS_TIM1_load_immediate(period);
		MSS_TIM1_start();
		MSS_TIM1_enable_irq();
}

//void led0() {code to toggle LED}

//put new timer in list maintaining order least time remaining to most
void insert_timer(Timer_t * newtimer){
	if (!timer)
		timer = newtimer;

	struct Timer *t = timer;

	while (t->next != NULL)
		t = t->next;

	t->next = newtimer;
	newtimer->next = NULL;


	Sort(timer);
};

//add a continuous (periodic) timer to linked list.
void startTimerContinuous(handler_t handler, uint32_t period){
	struct Timer *t;
	t = malloc(sizeof(struct Timer));
	t->handler = handler;
	t->period = period;
	t->mode = 0;
	t->time = period;
	t->next = NULL;
	if (timer == NULL)
		start_hardware_timer(period);
	insert_timer(t);
};
//example
//startTimerContinuous(&led0, 50000000);

//add a one shot timer to the linked list.

void startTimerOneshot(handler_t handler, uint32_t period){
	struct Timer *t;
	t = malloc(sizeof(struct Timer));
	t->handler = handler;
	t->period = period;
	t->mode = 1;
	t->time = period;
	t->next = NULL;
	if (timer == NULL)
			start_hardware_timer(period);
	insert_timer(t);
};

//update down count with elapsed time, call fnc if timer zero, update continuous timers
//with new down count
void update_timers(uint32_t sub_time){
	//timer->handler();

	struct Timer *t = timer;
	while (t) {
		t->time = t->time - sub_time;

		t = t->next;
	}
};


void deleteNode(struct Timer **head_ref, int position)
{
   // If linked list is empty
   if (*head_ref == NULL)
      return;

   // Store head node
   struct Timer* temp = *head_ref;

    // If head needs to be removed
    if (position == 0)
    {
        *head_ref = temp->next;   // Change head
        free(temp);               // free old head
        return;
    }

    int i;
    // Find previous node of the node to be deleted
    for (i = 0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    // If position is more than number of ndoes
    if (temp == NULL || temp->next == NULL)
         return;

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    struct Timer *next = temp->next->next;

    // Unlink the node from linked list
    free(temp->next);  // Free memory

    temp->next = next;  // Unlink the deleted node from list
}

void Timer1_IRQHandler( void ){
	uint32_t sub_time = timer->time;
	update_timers(sub_time);

	struct Timer *t = timer;


	int count = 0;
	while (t != NULL) {
		if (t->mode == 0 && t->time == 0) {//is continuous
			t->handler();
			t->time = t->period;
			t = t->next;
			count++;
			//startTimerContinuous(timer->handler, timer->period);
		}
		else if (t->time == 0){
			t->handler();
			t = t->next;
			deleteNode(&timer, count);

		}
		else
			t = t->next;

	}


	//delete timer->next


	//sglib_timer_t_delete(&timer, timer->next);

	//update all other timers

	//sglib_Timer_t_sort(&timer);
	Sort(timer);
	//print_Timer();

	MSS_TIM1_clear_irq();
	MSS_TIM1_load_immediate(timer->time);
	MSS_TIM1_start();
}
