/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  j2dias@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  Fall 2016
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *    -
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 *    -
 *****************************************/

#ifndef TRIE_H
#define TRIE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Trie_node.h"
#include "Exception.h"
#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class Trie {
	private:
		Trie_node *root_node;
		int trie_size;

	public:
		Trie();
		~Trie();

		// Accessors

		int size() const;
		bool empty() const;
		bool member( std::string const & ) const;
		Trie_node *root() const;
		// Mutators

		bool insert( std::string const & );
		bool erase( std::string const & );
		void clear();

	// Friends

	friend std::ostream &operator<<( std::ostream &, Trie const & );
};

Trie::Trie():
root_node( nullptr ),
trie_size( 0 ) {
	// empty constructor
}

Trie::~Trie() {
	clear();//Clear all tree Nodes
	delete root_node; //Delete pointer to root node
}

int Trie::size() const {
	return trie_size;
}

bool Trie::empty() const {
	return (trie_size == 0);
}

Trie_node *Trie::root() const {
	return root_node;
}

bool Trie::member( std::string const &str ) const {
	if (empty()) return false;

	//Check if there are any invalid characters, turn characters to lowercase
	string new_str = str;
	int size = new_str.size();
	for (int c = 0; c < size; c++) {
		if (isalpha(new_str[c])) {
			new_str[c] = tolower(new_str[c]);
		}else {
			throw illegal_argument();
		}
	}

	//Recursively search the tree for the word
	return root_node->member(new_str, 0);
}

bool Trie::insert( std::string const &str ) {
	//Check if there are any invalid characters, turn characters to lowercase
	string new_str = str;
	int size = new_str.size();
	for (int c = 0; c < size; c++) {
		if (isalpha(new_str[c])) {
			new_str[c] = tolower(new_str[c]);
		}
		else {
			throw illegal_argument();
		}
	}
	
	//check if root has been created, if not create it
	if (root_node == nullptr) root_node = new Trie_node();

	//add string to tree recursively
	bool inserted = root_node->insert(new_str, 0);
	
	//increment tree size
	if(inserted) trie_size ++;
	return inserted;
}

bool Trie::erase( std::string const &str ) {
	if (empty()) return false;
	//Check if there are any invalid characters, turn characters to lowercase
	string new_str = str;
	int size = new_str.size();
	for (int c = 0; c < size; c++) {
		if (isalpha(new_str[c])) {
			new_str[c] = tolower(new_str[c]);
		}
		else {
			throw illegal_argument();
		}
	}

	//erase string from tree recursively
	bool erased = root_node->erase(new_str, 0, root_node);
	if (erased) trie_size--;
	return erased;
}

void Trie::clear() {
	if (root_node != nullptr) {
		//clear nodes from tree
		root_node->clear();
		root_node = nullptr;
		trie_size = 0;
	}
}

// You can modify this function however you want:  it will not be tested

std::ostream &operator<<( std::ostream &out, Trie const &trie ) {

	return out;
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
