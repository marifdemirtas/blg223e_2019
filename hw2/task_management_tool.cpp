/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID : 150180001
Date: 13.11.19 */
/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot;
		Task *compeer;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL;
  usable_day = 0;
  usable_time = 0;
  return;
  
	//WRITTEN BY ME
}

//THIS FUNCTION IS CREATED BY ME
void WorkPlan::deleteCtr(Task* day){ //deletes counterpart tasks of a given day
  Task *temp = NULL;

  while(day->counterpart != NULL){
    temp = day->counterpart;
    day->counterpart = day->counterpart->counterpart;
    delete[] (temp->name);
    delete temp;
  }
}

//THIS FUNCTION IS CREATED BY ME
Task* WorkPlan::findDay(int day){ //returns the pointer to the day on the given day
  Task* traverse = head;

  while(traverse && traverse->day < day){
    traverse = traverse->next;
  }

  return traverse;
}

void WorkPlan::close()
{
  Task *temp = NULL;
  if(head == NULL){
    return;
  }
  while(head->next != head){ //more than one node in list
    deleteCtr(head);
    head->previous->next = head->next;
    head->next->previous = head->previous;
    temp = head;
    head = head->next;
    delete[] (temp->name);
    delete temp;
  }
  deleteCtr(head);
  delete[] (head->name);
  delete head;
  head=NULL;

	//WRITTEN BY ME
}

//THIS FUNCTION IS CREATED BY ME
void WorkPlan::addCtr(Task *traverse, Task *task){ //adds the task pointed by *task as a counterpart of *traverse
  Task* previous = NULL;
  
  if (traverse->time > task->time) {  //days first task is updated
    task->previous = traverse->previous;
    task->next = traverse->next;
    traverse->previous->next = task;
    traverse->next->previous = task;
    task->counterpart = traverse;
    traverse->next = NULL;
    traverse->previous = NULL;
    if (traverse == head) {
        head = task;
    }
    return;
  } else if (traverse->time == task->time) { //priorities are checked
    if (traverse->priority >= task->priority){ //task is delayed
      checkAvailableNextTimesFor(task);
      task->day = getUsableDay();
      task->time = getUsableTime();
      add(task);
      return;
    } else if (traverse->priority < task->priority){ //traverse is delayed
      checkAvailableNextTimesFor(traverse);
      char* name = new char[3];
      strcpy(name, traverse->name);
      Task* newtask = insert(traverse, name);
      remove(traverse);
      add(task);
      newtask->day = getUsableDay();
      newtask->time = getUsableTime();
      add(newtask);
      return;
    };
    return;
  }
  //rest is performed if task should become a counterpart of traverse
  while(traverse->time < task->time && traverse->counterpart != NULL){
    previous = traverse;
    traverse = traverse->counterpart;
  }
  
  if (traverse->time > task->time){
    task->counterpart = traverse;
    previous->counterpart = task;
  } else if (traverse->counterpart == NULL) {
    traverse->counterpart = task;
    task->counterpart = NULL;
  } else if (traverse->time == task->time){
      checkAvailableNextTimesFor(traverse);
    if(traverse->priority <= task->priority){
      task->time = getUsableTime();
      task->day = getUsableDay();
      add(task);
      return;
    } else {
      previous->counterpart = task;
      task->counterpart = traverse->counterpart;
      remove(traverse);
      traverse->time = usable_time;
      traverse->day = usable_day;
      add(traverse);
      return;
    }
  }
}

//THIS FUNCTION IS CREATED BY ME
Task* WorkPlan::insert(Task *task, char* name){ //creates a dynamic node to add to the list
  Task* newtask = new Task;
  newtask->name = name;
  newtask->day = task->day;
  newtask->time = task->time;
  newtask->priority = task->priority;
  newtask->next = NULL;
  newtask->previous = NULL;
  newtask->counterpart = NULL;
  return newtask;
}

void WorkPlan::add(Task *task)
{ 
  char* name = new char[3];
  strcpy(name, task->name);
  Task* newtask = insert(task, name);
  
  if(head == NULL){
    head = newtask;
    head->previous = head;
    head->next = head;
    head->counterpart = NULL;
  } else {
    Task* traverse = head;
    if(traverse->day == newtask->day){
      addCtr(traverse, newtask);
    } else if (traverse->day < newtask->day){
      traverse = traverse->next;
      while(traverse->day < newtask->day && head->day != traverse->day){
        traverse = traverse->next;                          
      }
      if (head->day == traverse->day){ //add to end
        head->previous->next = newtask;
        newtask->previous = head->previous;
        head->previous = newtask;
        newtask->next = head;
      } else if (traverse->day == newtask->day){
        addCtr(traverse, newtask);
      } else { // add before traverse
        newtask->previous = traverse->previous;
        newtask->next = traverse;
        newtask->counterpart = NULL;
        traverse->previous->next = newtask;
        traverse->previous = newtask;        
      }
    } else if (traverse->day > newtask->day){
        head->previous->next = newtask;
        newtask->previous = head->previous;
        head->previous = newtask;
        newtask->next = head;
        head = newtask;
      }
  }

	//WRITTEN BY ME
} 

Task * WorkPlan::getTask(int day, int time)
{
  Task* traverse = findDay(day);

  while(traverse && traverse->time < time){
    traverse = traverse->counterpart;
  }

  return traverse;

	//WRITTEN BY ME
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{
  usable_time = delayed->time;
  usable_day = delayed->day;

  Task* traverse = delayed;
  Task* searchafterday = findDay(delayed->day);
  Task* breakpoint = searchafterday;
  
  while(traverse->counterpart != NULL && usable_time < 16){
    usable_time++;
    if(traverse->counterpart->time > usable_time){
      return;
    }
    traverse = traverse->counterpart;
  }
  
  while(searchafterday->next != breakpoint){
    usable_time = 8;
    usable_day++;
    searchafterday = searchafterday->next;
    traverse = searchafterday;
    if(traverse->time > getUsableTime()){
      return;
    }
    while(traverse->counterpart != NULL && usable_time < 16){
      usable_time++;
      if(traverse->counterpart->time > getUsableTime()){
        return;
      }
      traverse = traverse->counterpart;
    }
  }
  usable_time = 8;
  usable_day = head->previous->day + 1;

  return;
  
  //WRITTEN BY ME
}

void WorkPlan::delayAllTasksOfDay(int day)
{
  Task* delayed = findDay(day);

 // Task* dn = delayed->next;
  //Task* dp = delayed->previous;
  Task* temp;
    
  while(delayed->counterpart != NULL){
    checkAvailableNextTimesFor(delayed);
    if(usable_day == day){
      delayed->time = getUsableTime();
      continue;
    }
    char* name = new char[3];
    strcpy(name, delayed->name);
    Task* newtask = insert(delayed, name);
    temp = delayed->counterpart;
    remove(delayed);
    delayed = temp;
    newtask->day = getUsableDay();
    newtask->time = getUsableTime();
    add(newtask);
  }
  checkAvailableNextTimesFor(delayed);
  char* name = new char[3];
  strcpy(name, delayed->name);
  Task* newtask = insert(delayed, name);
  remove(delayed);
  newtask->day = getUsableDay();
  newtask->time = getUsableTime();
  add(newtask);


	//WRITTEN BY ME
}

void WorkPlan::remove(Task *target)
{
  Task* dayhead = findDay(target->day);
  if(target->counterpart == NULL && dayhead == target){
    if (head == target){
      head = target->next;
    }
    target->next->previous = target->previous;
    target->previous->next = target->next;
  }else if(target->counterpart != NULL){
    if(dayhead == target){
      dayhead->previous->next = dayhead->counterpart;
      dayhead->next->previous = dayhead->counterpart;
      dayhead->counterpart->next = dayhead->next;
      dayhead->counterpart->previous = dayhead->previous; 
      if (head == target){
        head = dayhead->counterpart;
      }
      delete[] (target->name);
      delete target;
      return;
    }
    while (dayhead->counterpart != target){
      dayhead = dayhead->counterpart;
    }
    dayhead->counterpart = target->counterpart;
  } else {
    while(dayhead->counterpart != target){
      dayhead = dayhead->counterpart;
    }
    dayhead->counterpart = NULL;
  }
  delete[] (target->name);
  delete target;
  return;

	//WRITTEN BY ME
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot;
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
