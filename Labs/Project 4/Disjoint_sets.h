
#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#ifndef nullptr
#define nullptr 0
#endif


#include <limits>
#include "Exception.h"

using namespace std;

struct ll_entry; // represents each element of the linked list
struct set_info; // keeps track of the information (pointers to head and tail and the size of the set) of each set
//could we delete the above two lines?


struct ll_entry{ //element of linked list
	int content;
	set_info* ptr_to_info; // ptr to the info entry of the corresponding set
	ll_entry* next;
	
};

struct set_info { 
	ll_entry* head;
	ll_entry* tail;
	int size;//size = weight?
};

class Disjoint_set {

private:
	ll_entry** nodes;
	set_info** sets;
	int set_counter;
	int initial_num_sets;
public:
	Disjoint_set(int);
	~Disjoint_set();
	int find_set(int) const;
	int num_sets() const;
	void union_sets(int, int);
};

Disjoint_set::Disjoint_set(int n) : nodes(new ll_entry*[n]), 
sets (new set_info*[n]), 
set_counter(n),
initial_num_sets(n) {
	for (int c = 0; c < set_counter; c++) {
		//declare new set and node element
		nodes[c] = new ll_entry;
		sets[c] = new set_info;
		
		//initialize sets
		sets[c]->size = 1;
		sets[c]->head = nodes[c];
		sets[c]->tail = nodes[c];

		//initialize nodes
		nodes[c]->ptr_to_info = sets[c];
		nodes[c]->next = NULL;
		nodes[c]->content = c;
	}
}

Disjoint_set::~Disjoint_set() {
	for (int c = 0; c < initial_num_sets; c++) {
		//delete the individual elements
		delete nodes[c];
		delete sets[c];
	}
	//delete the arrays for the elements
	delete[] nodes;
	delete[] sets;
}
int Disjoint_set::find_set(int item) const{
	return nodes[item]->ptr_to_info->head->content;
}

int Disjoint_set::num_sets() const {
	return set_counter;
}

void Disjoint_set::union_sets(int node_index1, int node_index2) {
	if (node_index1 == node_index2) return;
	//make sure two nodes aren't already in same set
	if (find_set(node_index1) == find_set(node_index2)) return;
	
	//get info for both nodes
	set_info* si1 = nodes[node_index1]->ptr_to_info;
	set_info* si2 = nodes[node_index2]->ptr_to_info;

	// ni1: the index of the larger set, ni2: the index of the smaller index
	int ni1 = si1->size >= si2->size ? node_index1 : node_index2; //reads if size1>=size2 then ni1 takes node_index1
	int ni2 = si1->size < si2->size ? node_index1 : node_index2; //reads if size1<size2 then ni2 takes node_index1

	//get info for larger and smaller nodes
	set_info* large_info = nodes[ni1]->ptr_to_info;
	set_info* small_info = nodes[ni2]->ptr_to_info;
	
	//temporary node pointer, points to tail element in larger set
	ll_entry* temp = large_info->tail;
	//chage tail pointer in larger set from null to head of smaller set
	temp->next = small_info->head;
	//temp points to head of smaller set
	temp = small_info->head;

	while (temp->next!=NULL) {
		//point each node of smaller set to info of largerset
		temp->ptr_to_info = large_info;
		//go to next node
		temp = temp->next;
	}

	//the last node will always be missed, this code is to endure it points to larger info
	temp->ptr_to_info = large_info; 
	//set tail of larger info to last element
	large_info->tail = temp;
	//point last element to null
	temp->next = NULL;
	//increase size of set and decrement set counter
	large_info->size += small_info->size;
	set_counter--;
}
#endif
