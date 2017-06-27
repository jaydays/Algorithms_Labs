#ifndef LINEAR_HASH_TABLE_H
#define LINEAR_HASH_TABLE_H

/*****************************************
 * UW User ID:  j2dias
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "Exception.h"
#include "ece250.h"
#include <cmath>
#include <iostream>

using namespace std;

enum state {EMPTY, OCCUPIED, DELETED};
int ogCapacity ;

template<typename T>
class LinearHashTable {
	private:
		int counter;
		int power;
		int arraySize;
        	int mod;
		T *mainArray;
		state *indexState;


	public:
		LinearHashTable( int m = 5 );
		~LinearHashTable();
		int size() const;
		int capacity() const;
		double load_factor() const;
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		int hash_func(T const &);
		void insert( T const & );
		void double_array();
		bool remove( T const & );
		void half_array();
		void clear();
		void print() const;
};

template<typename T >
LinearHashTable<T >::LinearHashTable( int m ):
power(m),
arraySize( 1 << power ){
	mod = arraySize;
	ogCapacity = arraySize;
	mainArray = new T[arraySize];
	indexState = new state[arraySize];
	for (int ctr = 0; ctr < arraySize; ctr++) {
		indexState[ctr] = EMPTY;
		//mainArray[ctr] = 0;
	}
	counter = 0;
}

template<typename T >
LinearHashTable<T >::~LinearHashTable() {
	delete[] mainArray;
	delete[] indexState;
}

template<typename T >
int LinearHashTable<T >::size() const {
	return counter;
}

template<typename T >
int LinearHashTable<T >::capacity() const {
	return arraySize;
}

template<typename T >
int LinearHashTable<T >::hash_func(T const &obj) {
	int modInt = obj;
	if (modInt < 0) modInt = modInt*(mod - 1);

	int index = modInt%arraySize;
	return index;
}

template<typename T >
double LinearHashTable<T >::load_factor() const {
	double dblCounter = counter;
	double dblArraySize = arraySize;
	return (dblCounter / dblArraySize);
}

template<typename T >
bool LinearHashTable<T >::empty() const {
	return (counter == 0);
}


template<typename T >
bool LinearHashTable<T >::member( T const &obj ) const {
	//couldn't access hash_func
	int modInt = obj;
	if (modInt < 0) modInt = modInt*(mod - 1);
	int pos = modInt%arraySize;
	int ctr = 0;

	while (indexState[pos] == OCCUPIED || indexState[pos] == DELETED) {
		if (mainArray[pos] == obj&& indexState[pos] == OCCUPIED) return true;
		pos = (pos + 1) % arraySize;
		ctr++;
		if (ctr == arraySize+1) return false;
	}
	
    return false;
}

template<typename T >
T LinearHashTable<T >::bin( int n ) const {
	if (n<0||n>=arraySize||indexState[n] != OCCUPIED) return 0;
	return mainArray[n];
}

template<typename T >
void LinearHashTable<T >::insert( T const &obj ) {
	if (load_factor() > 0.75) double_array();
	int pos = hash_func(obj);
	while (indexState[pos] == OCCUPIED) {
		pos = (pos + 1) % arraySize;
	}
	mainArray[pos] = obj;
	indexState[pos] = OCCUPIED;
	counter++;
}

template<typename T >
void LinearHashTable<T >::double_array() {
	arraySize = 2 * arraySize;
	counter = 0;
	T tempObj;
	T* tempMainArray = new T[arraySize/2]; //new T[arraySize];
	state* tempIndexState = new state[arraySize/2]; //new state[arraySize];
	
	for (int ctr = 0; ctr < arraySize/2; ctr++) {
		tempMainArray[ctr] = mainArray[ctr];
		tempIndexState[ctr] = indexState[ctr];
	}

	delete[] mainArray;
	delete[] indexState;
	indexState = new state[arraySize];
	mainArray = new T[arraySize];
	for (int ctr = 0; ctr < arraySize; ctr++) {
		indexState[ctr] = EMPTY;
	}
	

	for (int ctr = 0; ctr<arraySize/2; ctr++) {
		
		if (tempIndexState[ctr] == OCCUPIED) {
			tempObj = tempMainArray[ctr];
			insert(tempObj);			
		}
	}
	delete[] tempMainArray;
	delete[] tempIndexState;
	return;
}

template<typename T >
bool LinearHashTable<T >::remove( T const &obj ) {
	int pos = hash_func(obj);
	int ctr = 0;
	while (indexState[pos] == OCCUPIED|| indexState[pos] == DELETED) {
		if (mainArray[pos] == obj&&indexState[pos] == OCCUPIED) {
			indexState[pos] = DELETED;
			counter--;
			if (load_factor() <0.25&&arraySize>ogCapacity) half_array();
			return true;
		}
		pos = (pos + 1) % arraySize;
		ctr++;
		if (ctr == arraySize+1) return false;
	}
	return false;
}

template<typename T >
void LinearHashTable<T >::half_array() {
	arraySize = arraySize/2;
	counter = 0;
	T tempObj;
	T* tempMainArray = new T[arraySize * 2]; //new T[arraySize];
	state* tempIndexState = new state[arraySize * 2]; //new state[arraySize];

	for (int ctr = 0; ctr < arraySize * 2; ctr++) {
		tempMainArray[ctr] = mainArray[ctr];
		tempIndexState[ctr] = indexState[ctr];
	}

	delete[] mainArray;
	delete[] indexState;
	mainArray = new T[arraySize];
	indexState = new state[arraySize];
	for (int ctr = 0; ctr < arraySize; ctr++) {
		indexState[ctr] = EMPTY;
	}
	
	

	for (int ctr = 0; ctr<arraySize * 2; ctr++) {

		if (tempIndexState[ctr] == OCCUPIED) {
			tempObj = tempMainArray[ctr];
			insert(tempObj);
		}
	}

	delete[] tempMainArray;
	delete[] tempIndexState;
	return;
}

template<typename T >
void LinearHashTable<T >::clear() {
	for (int ctr = 0; ctr < arraySize; ctr++) {
		indexState[ctr] = EMPTY;
	}
	counter = 0;
}

template<typename T >
void LinearHashTable<T >::print() const{
	// enter your implemetation here
}

#endif
