/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID: 150180001
Date: 15.10.2019 */

#ifndef STOCK_H
#define STOCK_H

struct node {
  int size;
  int quant;
  node *next;
};

struct stock {
  node *head;
  void create();
  void add_stock(int);
  void sell(int);
  void current_stock();
  void clear();
  void read_input(char*);
};

#endif
