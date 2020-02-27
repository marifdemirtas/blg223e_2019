/* @Author
Student Name: Mehmet Arif Demirtaş
Student ID: 150180001
Date:16.12.19 */

#include <iostream> //cin, cout
#include <cstdio> // file ops
#include <cstdlib> // exit
#include <cstring> //strcpy

#define MAX_MESSAGE_SIZE 50

using namespace std;

#ifndef _STACK //STACK DEFINITIONS
#define _STACK

struct stack_node{
	int data;
	stack_node* next;
};

struct stack{
	stack_node* head;

	void create();
	void clear();
	void push(int data);
	int pop();
	bool is_empty();
};

#endif

#ifndef _NET //NETWORK DEFINITIONS
#define _NET
struct message{ //MESSAGE NODE
	char content[MAX_MESSAGE_SIZE + 1];
	int dest;
};

struct mh_node{ //MOBILE HOST NODE
    int id;
    int parent_id;

    void create(int n_id, int n_parent_id);
};

struct bs_node{ //BASE STATION NODE
    int id;
    int parent_id;

    int bs_max;             //Current max capacity of bs_node children, will increase by a factor of 2 when full
    int bs_size;            //Number of bs_node children connected
    bs_node *bs_list;       //Will be dynamic list of pointers to the children base stations
    
    int mh_max;             //Current max capacity of mh_node children, will increase by a factor of 2 when full
    int mh_size;            //Number of mh_node children connected
    mh_node *mh_list;       //Will be dynamic list of pointers to the children mobile host

    void create(int n_id, int n_parent_id);
    void resize_bs();
    void resize_mh();
    bool search(int s_id, stack s, char* message);
    bool add_bs(bs_node* child);
    bool add_mh(mh_node* child);
};

struct network{
    FILE* network_file;
    bs_node* root;
    stack search_stack;
    
    void create();
    void read_file(char* filename);
    void search(int dest, char* message);
    void clear();
};

#endif

//STACK METHODS
void stack::create(){
	head = NULL;
}

void stack::clear(){
	while(!is_empty())
		pop();
}

void stack::push(int data){
	stack_node *newnode = new stack_node;
	newnode->data = data;
	newnode->next = head;
	head = newnode;
}

int stack::pop(){
	if(!is_empty()){
		stack_node* temp = head;
		int data = temp->data;
		head = head->next;
		delete temp;
		return data;
	}
	return -1; //STACK RETURNS -1 WHEN EMPTY
}

bool stack::is_empty(){
	return (head == NULL);
}

//MOBILE HOST METHODS
void mh_node::create(int n_id, int n_parent_id){
    id = n_id;
    parent_id = n_parent_id;
}

//BASE STATION METHODS
void bs_node::create(int n_id, int n_parent_id){
    id = n_id;
    parent_id = n_parent_id;
    bs_max = 1;
    bs_size = 0;
    bs_list = new bs_node[bs_max];
    mh_max = 1;
    mh_size = 0;
    mh_list = new mh_node[mh_max];
}

void bs_node::resize_bs(){
    bs_max *= 2;
    bs_node *temp_list = new bs_node[bs_max];
    for(int i = 0; i < bs_max/2; i++){
        temp_list[i] = bs_list[i];
    }
    bs_node *todelete = bs_list;
    bs_list = temp_list;
    delete[] todelete;
}

void bs_node::resize_mh(){
    mh_max *= 2;
    mh_node *temp_list = new mh_node[mh_max];
    for(int i = 0; i < mh_max/2; i++){
        temp_list[i] = mh_list[i];
    }
    mh_node *todelete = mh_list;
    mh_list = temp_list;
    delete[] todelete;
}

bool bs_node::search(int s_id, stack s, char* message){ //Preorder traversal function for s_id > 0, also empties the tree if s_id == -1.
    s.push(id); //Push the current node's id into the stack to record the right way to MH
	bool found = false;
    bool deletion = (s_id == -1) ? true : false; //sets deletion to true if the function is ran to empty tree
    if(!deletion){
   	 	if(id == 0){ //Prints only the first time.
	        cout << "Traversing:";
	    }
        cout << id << " ";
    }
    for(int i = 0; i < mh_size; i++){
        if(mh_list[i].id == s_id){
            found = true;
            stack temp;
            temp.create();
            cout << endl << "Message:" << message << " To:";
            while(!s.is_empty()) {
             	temp.push(s.pop());
            }
            int i = 0;
            while(!temp.is_empty()){
            	cout << temp.pop() << " ";
            	i++;
            }
            cout << "mh_" << s_id << endl;
        }
    }
    if(deletion){
        delete[] mh_list;
    }
    if(!found){
	    for(int i = 0; i < bs_size; i++){
	        found = bs_list[i].search(s_id, s, message);    //RECURSIVE CALL
	        if(found){
	        	break;
            }
	    }
	}
    if(!found && id == 0){ //All nodes have been visited
    	s.pop();
    	if(id == 0 && s_id > 0)
	        cout << endl << "Can not be reached the mobile host mh_" << s_id << " at the moment" << endl;
    }
    if (deletion){
    	delete[] bs_list;
    }
    return found;
}

bool bs_node::add_bs(bs_node* child){
    bool found = false;
    if(child->parent_id == id){
        if(bs_size >= bs_max){
            resize_bs();
        }
        bs_list[bs_size++] = *child;
        found = true;
    }
    if(!found){
        for(int i = 0; i < bs_size; i++){
            if(bs_list[i].add_bs(child))
                break;
        }
    }
    return found;
}

bool bs_node::add_mh(mh_node* child){
    bool found = false;

    if(child->parent_id == id){
        if(mh_size >= mh_max){
            resize_mh();
        }
        mh_list[mh_size++] = *child;
        found = true;
    }
    if(!found){
        for(int i = 0; i < bs_size; i++){
            if(bs_list[i].add_mh(child))
                break;
        }
    }
    return found;
}

//NETWORK METHODS FOR ABSTRACTION
void network::create(){
    root = new bs_node;
    root->create(0, 0);
    search_stack.create();
}

void network::read_file(char* filename){
    network_file = fopen(filename, "r");
	if(network_file == NULL){
		cerr << "Network file cannot be opened." << endl;
		exit(1);
	}

    char line[32], type[4];
    int read_id, read_parent_id;

    while(!feof(network_file)){
        fgets(line, 32, network_file);
        sscanf(line, "%s %d %d", type, &read_id, &read_parent_id);
        if(type[0] ==  'B'){
            bs_node* newnode = new bs_node;
            newnode->create(read_id, read_parent_id);
            root->add_bs(newnode);
        } else if(type[0] == 'M') {
            mh_node* newnode = new mh_node;
            newnode->create(read_id, read_parent_id);
            root->add_mh(newnode);
        }
    }
    fclose(network_file);
}

void network::search(int dest, char* message){
    root->search(dest, search_stack, message);
    search_stack.clear();
}

void network::clear(){
	char c[2] = "0"; //Placeholder data
	root->search(-1, search_stack, c);
    delete root;
    root = NULL;
    search_stack.clear();
}

message** read_messages(const char* messages_str, message* arr[], int* max){

	FILE* message_file = NULL;
	message_file = fopen(messages_str, "r");
	if(message_file == NULL){
		cerr << "Message file cannot be opened." << endl;
		exit(1);
	}

	char line[MAX_MESSAGE_SIZE + 10], mymessage[MAX_MESSAGE_SIZE + 1], destc[10];
    int dest, i, k, j = 0;

    while(!feof(message_file)){
      
        i = 0;
        k = 0;
        fgets(line, MAX_MESSAGE_SIZE + 10, message_file);

        //SHOULD BE UNCOMMENTED IF THE FILE ENDS WITH A NEWLINE
        /*
       	if(feof(message_file)){
            break;
        }
		*/

        while(line[i] != '>'){
        	mymessage[i] = line[i];
        	i++;
        };

        mymessage[i++] = '\0';

        while(line[i] != '\n' && line[i] != '\0'){
        	destc[k] = line[i];
        	k++;
        	i++;
        };

        destc[k] = '\0';

        sscanf(destc, "%d", &dest);

        message* new_message = new message;
        strcpy(new_message->content, mymessage);
        new_message->dest = dest;
        arr[j++] = new_message;
        if(j == *max){
		    *max *= 2;
		    message** temp_list = new message*[*max];

		    for(int i = 0; i < *max/2; i++){
		        temp_list[i] = arr[i];
		    }

		    message** todelete = arr;
		    arr = temp_list;
		    delete[] todelete;
        }
    }
    *max = j;
    fclose(message_file);

    return arr;
}

int main(int argc, char* argv[])
{
    int messages_max = 2;
 
     if(argc < 3){
    	cerr << "Not enough input files are given, aborting..." << endl;
    	return 1;
    }
    char* network_str = argv[1];
    char* messages_str = argv[2];
    
    network my_network;
    my_network.create();
    my_network.read_file(network_str);

    message** arr = new message*[messages_max];
    arr = read_messages(messages_str, arr, &messages_max);

  	for(int i = 0; i < messages_max; i++){
  		my_network.search(arr[i]->dest, arr[i]->content);
  		delete arr[i];
  	}

  	delete[] arr;

  	my_network.clear();
}

