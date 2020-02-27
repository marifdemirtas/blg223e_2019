/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID: 150180001
Date: 15.10.2019 */
#include <iostream> // for cout
#include <cstdio>   // for file ops
#include <cstdlib>  // for memory ops
#include "stock.h"  // for stock data structures 

using namespace std;

// FOLLOWING ARE THE METHODS DEFINED ON LIST STRUCT

void stock::create(){ //Creates a new, empty stock list
  head = NULL;
}

void stock::add_stock(int size){ //Adds a shoe with the given size to the stock
  node *newnode = new node;
  node *traverse, *prev;
  newnode->size = size; //newnode is being initialized with the data given
  newnode->next = NULL;
  newnode->quant = 1;
  
  if(head == NULL){ //There are no shoes in the stock, this one is the head
    head = newnode;
  } else if (head->size > newnode->size){ //The given size shoe comes before the head, so it becomes new head
    newnode->next = head;
    head = newnode;
  } else if (head->size == newnode->size){ //The given size shoe is same with head, head's quantity is increased by one
    head->quant++;
    delete newnode; //newnode is not necessary after increasing the quantity
  } else { //The given size shoe comes after head 
    traverse = head;
    while(true){
      prev = traverse;
      traverse = traverse->next;
      if (traverse){ //Works while the stock list is not over
        if(newnode->size > traverse->size){ //The given size shoe comes after the current element in the list
          continue;
        } else if (newnode->size == traverse->size){ //The given size shoe is on the list, quantity is increased by one
          traverse->quant++;
          delete newnode; //newnode is not necessary after increasing the quantity
          break;
        }
      }
      newnode->next = traverse; //The given size shoe is appended at the current position
      prev->next = newnode;
      break;
    }
  }
}

void stock::sell(int size){ //Processes an order of a shoe with the given size
  node *searchnode = new node;
  node *traverse, *prev;
  searchnode->size = (-1) * size; //searchnode is the shoe to be sold
  searchnode->next = NULL;

  if(head == NULL){ //If there are no entries in the stock, this prints NO_STOCK
    cout << "NO_STOCK" << endl;
  } else if (searchnode->size == head->size && head->quant == 1){ //If the shoe we're looking for is the head of the stock and there is only one shoe of that size, this deletes that shoe
    traverse = head;
    head = head->next;
    delete traverse;
  } else { //This goes through the stock for all other scenerios
    traverse = head;
    prev = traverse;

    while(true){
      if (searchnode->size == traverse->size){ //searchnode is found
        if(traverse->quant == 1){              //The last shoe of the size being searched for is being deleted
          prev->next = traverse->next;
          delete traverse;
          break;
        } else {                               //There are multiple shoes same size, quantity is reduced by one
          traverse->quant--;
          break;
        }
      }
      if (traverse->next == NULL){ //searchnode is not found but we're at the end of the stock list
        cout << "NO_STOCK" << endl;
        break;
      }
      prev = traverse;
      traverse = traverse->next;
      }
  }
  delete searchnode; //Search has been done, node is no longer necessary
}

void stock::current_stock() { //Uses a temp node to go through the list and print it on the standard output stream
   node *traverse = head;
   while (traverse != NULL) { 
      cout << traverse->size << ":" << traverse->quant << endl;
      traverse = traverse->next;
   }
}

void stock::clear(){ //Clears the list by deleting all the nodes to prevent memory errors
  node *traverse;
  while(head!=NULL){
    traverse=head->next;
    delete head;
    head = traverse;
  }
}

void stock::read_input(char* inputname){ //This method is not on the example stock struct template. It is for reading an input file and processing the requests made.
  FILE *inputfile; 
 
  if (!(inputfile = fopen(inputname, "r"))){ //Checks if input file can be opened properly in read-only mode.
    cerr << "No input found." << endl;
    exit(1);
  }
  
  fseek(inputfile, 0, SEEK_SET); //Sets the file pointer to the beginning of the input file
  while(true){
    int command;
    fscanf(inputfile, "%d", &command); //Reads the next integer on the input file to the command variable
    
    if(feof(inputfile)){ //Breaks the loop when the file pointer is at the end
        break; 
    }

    if (command == 0) {   //Calls a method of stock object based on the command from the file
      current_stock();
    } else if (command > 0){
      add_stock(command);
    } else if (command < 0) {
      sell(command);
    } else {
      cerr << "Invalid input" << endl;
    }
  };

  fclose(inputfile); //Closes the input file properly
}

int main(int argc, char* argv[])
{
  stock shoe_stock;
  shoe_stock.create();
  shoe_stock.read_input(argv[1]);
  shoe_stock.clear();
}
