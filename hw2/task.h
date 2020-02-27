/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID : 150180001
Date: 13.11.19 */
/*
PLEASE DO NOT CHANGE THIS FILE 
*/

#define NAME_LENGTH 2

struct Task{
	char *name;
	int day;
	int time;
	int priority;

	Task *previous;
	Task *next;
	Task *counterpart;
};
