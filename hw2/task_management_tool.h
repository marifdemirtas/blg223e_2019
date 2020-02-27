/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID : 150180001
Date: 13.11.19 */
/*
PLEASE DO NOT REMOVE ANY FUNCTION OR VARIABLE IN WORKPLAN STRUCT, IF YOU NEED YOU CAN ADD NEW FUNCTIONS 
*/


#ifndef TASK_MANAGEMENT_TOOL
#define TASK_MANAGEMENT_TOOL

#include <stdio.h>

#define strnicmp strncasecmp

#include "task.h"

struct WorkPlan{
	void create();
	void close();
	void add(Task *task);
	void checkAvailableNextTimesFor(Task *delayed);
	int  getUsableDay();
	int getUsableTime();
	void remove(Task *target);
	void display(bool verbose, bool testing);
	void delayAllTasksOfDay(int day);
	Task * getTask(int day, int time);
	bool checkCycledList();

//MY FUNCTIONS, ADDED FOR SIMPLICITY, BREAKS LONGER FUNCTIONS INTO SMALLER ONES  
  void deleteCtr(Task *day); //DELETES ALL COUNTERPART TASKS OF A GIVEN DAY
  Task* findDay(int day); //RETURNS A POINTER TO THE FIRST TASK OF THE GIVEN DAY
  void addCtr(Task *traverse, Task *task); //ADDS A COUNTERPART TASK TO THE GIVEN DAY
  Task* insert(Task* task, char* name); //RETURNS A POINTER TO A DYNAMIC ALLOCATED NODE WHICH CONTAINS THE INFORMATION OF TASK TO BE ADDED 
	
	Task *head;
	int usable_day;
	int usable_time;
};
#endif 

