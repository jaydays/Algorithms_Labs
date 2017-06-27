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

#ifndef SINGLE_NODE_H
#define SINGLE_NODE_H

#ifndef nullptr
#define nullptr 0
#endif

#include <string>
#include <cctype>
#include "ece250.h"

using namespace std;

class Trie;

class Trie_node {
	private:
		Trie_node **children;//first pointer indicates an array, second pointer indicates the array is full of pointers
		bool is_terminal;
		static int const CHARACTERS = 26;


	public:
		Trie_node();
		Trie_node *child( int ) const;

		bool member( std::string const &, int ) const;

		bool insert( std::string const &, int );
		bool erase( std::string const &, int, Trie_node *& );
		void clear();

		friend class Trie;

		// if ptr is a pointer to a Trie_node object
		// in one of the friendly classes, you should:
		//    use   ptr->next_   to modify it
		//    use   ptr->next()      to access it
};

Trie_node::Trie_node():
children( nullptr ),
is_terminal( false ) {
	// nothing else needs to go here
}

Trie_node *Trie_node::child( int n ) const {
	if ( children == nullptr ) {
		return nullptr;
	} else {
		return children[n];
	}
}

bool Trie_node::member(std::string const &str, int depth) const {
	if (depth == str.size()){
		return is_terminal; //word is only in tree is last charaster is a terminal
	}
	
	int pos = str[depth] - 97; //map lowercase letters to index, A = 0 ....
	if (child(pos) == nullptr) {
		return false; //The next char is not in the tree, word is not in the tree
	}
	return children[pos]->member(str, ++depth); //Recursively search for word
}

bool Trie_node::insert( std::string const &str, int depth ) {
	if (depth == str.size()) {
		if (is_terminal) return false; //word is already in tree
		is_terminal = true;	//all the words character are in the tree, but the word hasn't been put there
		return true;
	}

	int pos = str[depth] - 97; //map lowercase letters to index, A = 0 ....
	if (children == nullptr) {
		children = new Trie_node*[CHARACTERS]; //If array is pointing to null, declare new array of 26
		//set all pointers in the array to nullptr
		for (int c = 0; c < CHARACTERS; c++) {
			children[c] = nullptr;	
		}
	}

	//if the character does not exist in the tree, create new node
	if (children[pos] == nullptr) {
		children[pos] = new Trie_node();
	}
	return children[pos]->insert(str, ++depth); //recursively insert word into tree
}

bool Trie_node::erase( std::string const &str, int depth, Trie_node *&ptr_to_this ) {
	bool delete_node; //variable to denote wehter to delete node or not
	int pos = str[depth] - 97; //map lowercase letters to index, A = 0 ....

	if (depth == str.size()) {
		if (!is_terminal) return false;	//characters are in the tree, but word is not
		is_terminal = false; //word no longer in tree
		
		//If the node has no children, delete node
		delete_node = true;
		for (int c = 0; c < CHARACTERS; c++) {
			if (child(c) != nullptr) {
				delete_node = false;
				break;
			}
		}
		if (delete_node) {
			ptr_to_this = nullptr; //set pointer to this node to null
			delete this; //delete node
		}
		return true; //word was in tree, and has been removed
	}
	
	if (child(pos) == nullptr) {
		return false; //next char not in tree, word can't be deleted
	}

	bool ereased = children[pos]->erase(str, ++depth, children[pos]); //recursively delete word
	
	//if the word was in the tree, and the current node is not a terminal, the node can be deleted if it has no children
	if (ereased && !is_terminal) {
		delete_node = true;
		//check for children
		for (int c = 0; c < CHARACTERS; c++) {
			if (child(c) != nullptr) {
				delete_node = false;
				break;
			}
		}
		//if there are no children delete node
		if (delete_node) {
			delete[] children;
			ptr_to_this = nullptr;
			delete this;
		}
	}
	// return whether word was erased or not
	return ereased;
}
void Trie_node::clear() {
	//children array needs to be deleted two different ways depending on if its been intialised or not
	if (children == nullptr) {
		delete children; //children point to null
	}
	else {
		for (int c = 0; c < CHARACTERS; c++) {
			if (child(c) != nullptr) {
				children[c]->clear(); //recursively clear nodes
				children[c] = nullptr; //set pointers to null pointer
			}
			delete children[c];
		}
		delete[] children; //children points to an array
	}
	
	delete this; //delete current node
}

// Is an error showing up in ece250.h or elsewhere?
// Did you forget a closing '}' ?

#endif
