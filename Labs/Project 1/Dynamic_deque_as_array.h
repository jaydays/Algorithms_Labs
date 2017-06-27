#ifndef DYNAMIC_DEQUE_AS_ARRAY_H
#define DYNAMIC_DEQUE_AS_ARRAY_H

#include "ece250.h"
#include "Exception.h"
#include <iostream>

using namespace std;

class Dynamic_deque_as_array{
	public:
	    //variables
	    int* mainArray; //pointer to be used as an array
        int headIndex; //head index
        int tailIndex; //tail index
        int counter; //counter
        int arraySize; // size of the array

	    //constructor/destructor
		Dynamic_deque_as_array(int n =10);
		~Dynamic_deque_as_array();

        //Accessors
		int head() const;
		int tail() const;
		int size() const;
		bool empty() const;
		int capacity() const;

        //Mutators
		void enqueue_head( const int & );
		void enqueue_tail( const int & );
		void double_array();
		int dequeue_head();
		int dequeue_tail();
		void clear();
};


Dynamic_deque_as_array::Dynamic_deque_as_array(int n) {
    arraySize = n;
    if (arraySize<1) arraySize = 1;
    mainArray = new int[arraySize];
    counter = 0;
    headIndex = 0;
    tailIndex = 0;
}




Dynamic_deque_as_array::~Dynamic_deque_as_array() {
    delete[]mainArray;
}





int Dynamic_deque_as_array::size() const {
	return counter;
}


int Dynamic_deque_as_array::capacity() const {
	return arraySize;
}


bool Dynamic_deque_as_array::empty() const {
    if (counter == 0) return 1;
    return 0;
}


int Dynamic_deque_as_array::head() const {
    if (empty()) throw new underflow();
	return mainArray[headIndex];
}


int Dynamic_deque_as_array::tail() const {
	if (empty()) throw new underflow();
	return mainArray[tailIndex];
}



void Dynamic_deque_as_array::enqueue_head( const int &obj ) {
	if (counter == arraySize){
        double_array();
	}

	if (counter != 0){//if counter = 0, don't move head index
	    if (headIndex ==0) headIndex = arraySize; //guarentees headIndex is positive for modulus operator
        headIndex =(headIndex-1)%arraySize;
    }
    mainArray[headIndex] = obj;
	counter++;
	return;
}

void Dynamic_deque_as_array::enqueue_tail( const int &obj ) {
	if (counter == arraySize){
        double_array();
	}

	if (counter != 0){//if counter = 0, don't move tail index
        tailIndex =(tailIndex+1)%arraySize;
    }
    mainArray[tailIndex] = obj;
	counter++;
	return;
}


void Dynamic_deque_as_array::double_array(){
    int* tempArray = new int[arraySize*2];
    for (int ctr = 0; ctr<counter; ctr++){
        int tempPos = (ctr + headIndex)%arraySize;
        tempArray[ctr] = mainArray[tempPos];
    }
    delete[] mainArray;
    mainArray = tempArray;
    headIndex = 0;
    tailIndex = counter-1;
    arraySize = 2*arraySize;
    return;
}

int Dynamic_deque_as_array::dequeue_head() {
	if (empty()) throw new underflow();
	int oldHead = headIndex;
	if (counter>1) headIndex = (headIndex+1)%arraySize;//only move head if there is at least 1 entry
	counter--;
	return mainArray[oldHead];
}

int Dynamic_deque_as_array::dequeue_tail() {
	if (empty()) throw new underflow();
	int oldTail = tailIndex;
	if (counter>1){//only tail if counter is greater than 1
        if (tailIndex ==0) tailIndex = arraySize; //guarentees tailIndex is positive for modulus operator
        tailIndex = (tailIndex-1)%arraySize;
	}
	counter--;
	return mainArray[oldTail];
}


void Dynamic_deque_as_array::clear() {
    counter = 0;
    headIndex = 0;
    tailIndex = 0;
	return;
}

#endif
