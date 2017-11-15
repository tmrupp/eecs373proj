#include "mss_timer.h"
//#include "sglib.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

//The following are suggestions for functions and data structures you will likely need.

//declare function pointer type
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

//struct Timer *timer = NULL;

void swap(struct Timer *a, struct Timer *b);

void Sort(Timer_t *head);

//look in mss_timer.h for details
void start_hardware_timer(uint32_t period);

//void led0() {code to toggle LED}

//put new timer in list maintaining order least time remaining to most
void insert_timer(Timer_t * newtimer);

//add a continuous (periodic) timer to linked list.
void startTimerContinuous(handler_t handler, uint32_t period);
//example
//startTimerContinuous(&led0, 50000000);

//add a one shot timer to the linked list.

void startTimerOneshot(handler_t handler, uint32_t period);

//update down count with elapsed time, call fnc if timer zero, update continuous timers
//with new down count
void update_timers(uint32_t sub_time);


void deleteNode(struct Timer **head_ref, int position);

void Timer1_IRQHandler( void );
