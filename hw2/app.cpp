/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID : 150180001
Date: 13.11.19 */
/*
PLEASE DO NOT CHANGE THIS FILE 
*/

#include <iostream>	
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <ctype.h>
#include <bits/stdc++.h>
#include <chrono>
#include <thread> 

#define strnicmp strncasecmp 

#include "task_management_tool.h"

using namespace std;

typedef WorkPlan Datastructure;

Datastructure workplan;

void print_menu();
bool perform_operation(char);
void add_from_file(string filepath);
void display_schedule();
void remove_task();
void delay_a_task(); 
void delay_all_tasks_of_a_day();

string filepath="appointments";
bool testing=false;

int main(int argc, char* argv[]){
	if (argc>1)
	{
		if(strnicmp(argv[1],"f",1) == 0) 
		{
			filepath=argv[2];
			ifstream in(filepath);
			testing=true;
			if(!in.is_open())
			{
				cout<<"ERROR: File could not been open"<<endl;
				return 1;
			}
			else
				cout<<"You have changed default file path as '"<<argv[2]<<"'"<<endl;
		}
		else
		{
			cout << "Error: You have entered an invalid choice" << endl; 
			cout << "Please try again"<<endl;
		}
	}		
	workplan.create();	
	bool end = false; 
	char choice; 	
	while (!end) { 
		print_menu(); 
		cin >> choice;
		end = perform_operation(choice); 
		} 	
	workplan.close();
        this_thread::sleep_for(chrono::seconds(1));// to see the closing outputs
	system("clear");// make this line as comment if you are compiling on Windows
	//system("cls"); // make this line as comment if you are compiling on Linux or Mac
	return EXIT_SUCCESS;
}

void print_menu(){
	//system("clear");// make this line as comment if you are compiling on Windows
	//system("cls"); // make this line as comment if you are compiling on Linux or Mac
	cout << endl << endl;
	cout << "DR CELEBI APPOINTMENT APPLICATION" << endl;
	cout << "Choose an operation" << endl;
	cout << "A: Add From File" << endl;	
	cout << "D: Display Schedule" << endl;
	cout << "R: Remove Task" << endl;	
	cout << "G: Delay A Task" << endl;
	cout << "S: Delay All Tasks Of A Day Respecting Their Order and Priority" << endl;
	cout << "E: Exit" << endl;	
	cout << endl;
	cout << "Enter a choice {A,D,R,G,S,E}: ";
}

bool perform_operation(char choice){
	bool terminate=false;
	switch (choice) {
		case 'A': case 'a': 
			add_from_file(filepath);
			break; 
		case 'D': case 'd': 
			display_schedule();
			break;		 
		case 'R': case 'r': 
			remove_task();
			break;		
		case 'G': case 'g': 
			delay_a_task();
			break; 
		case 'S': case 's': 
			delay_all_tasks_of_a_day();
			break; 
		case 'E': case 'e': 
			cout << "Are you sure you want to exit the program? (Y/N):";
			cin >> choice;
			if(choice=='Y' || choice=='y')
				terminate=true; 
			break; 
		default: 
			cout << "Error: You have entered an invalid choice" << endl; 
			cout << "Please try again {A, D, R, G, S, E}  :" ;
			cin >> choice;
			terminate = perform_operation(choice);
			break; 
	}
	return terminate;
}

void add_from_file(string filepath)
{	
	ifstream in(filepath);
	if(!in.is_open())
		cout<<"ERROR: File could not been open"<<endl;
	Task task;
	while(in.peek() != EOF )
	{		
		string name, day, time, priority;
		getline(in,name,',');
		getline(in,day,',');
		getline(in,time,',');
		getline(in,priority,'\n');		

		char cstr[name.size() + 1];
		name.copy(cstr, name.size() + 1);
		cstr[name.size()] = '\0';
		task.name=cstr;
		task.day=stoi(day);
		task.time=stoi(time);
		task.priority=stoi(priority);	
		cout<<task.name<<" is read from file"<<endl;		
		workplan.add(&task);
	}
	in.close();
}

void remove_task(){
		int day=0,time=0;
	cout<<"Insert the day number and time of task that you want to remove"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}

	cout << "Time (Just enter hour without minutes) :";
	while (!(cin >> time))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nTime (Just enter hour without minutes) :";
	}
	//get the task that you want to remove
	Task *target=workplan.getTask(day, time);

	//remove task
	if (target!=NULL)
		workplan.remove(target);
}

void display_schedule()
{
	workplan.display(false,testing);
}

void delay_all_tasks_of_a_day()
{
	int day=0;
	cout<<"Insert the day number you want to delay"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}
	workplan.delayAllTasksOfDay(day);
}

void delay_a_task()
{
	int day=0,time=0;
	cout<<"Insert the day number and time of the task that you want to delay"<<endl;
	cout << "Day number :";
	while (!(cin >> day))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nDay number :";
	}

	cout << "Time (Just enter hour without minutes) :";
	while (!(cin >> time))
	{
	    cin.clear(); // clear the error flags
	    cin.ignore(INT_MAX, '\n'); // discard the row
	    cout << "Invalid input! Try again\nTime (Just enter hour without minutes) :";
	}
	
	if (day>0 and time>0)
	{ 
		//get the task that you want to suspend
		Task *oldone=workplan.getTask(day, time);
		Task *newone=new Task();
		if (oldone!=NULL)
		{
			newone->name=new char [strlen(oldone->name)];
			strcpy(newone->name,oldone->name);
			newone->priority=oldone->priority;
			workplan.checkAvailableNextTimesFor(oldone);
			cout<<workplan.getUsableDay()<<".day and "<<std::setw(2)<<workplan.getUsableTime()<<":00 is the first available day and hour for delaying the task "<<oldone->name<<" that is on "<<oldone->day<<".day at "<<std::setw(2)<< oldone->time <<":00"<<endl;
			newone->day=workplan.getUsableDay();
			newone->time=workplan.getUsableTime();
			workplan.remove(oldone);	
			workplan.add(newone);
		}		
		delete [] newone->name;
		delete newone;
			
	}
	else
		cout<<"Invalid input!"<<endl;
}
