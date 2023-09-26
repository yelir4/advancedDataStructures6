// FILE: node.cpp
//
//
//  COEN 79
//  --- Farokh Eskafi, COEN, SCU ---
//
//
// IMPLEMENTS: The functions of the node class and the
// linked list toolkit (see node1.h for documentation).
//
// INVARIANT for the node class:
//   The data of a node is stored in data_field
//   and the link to the next node is stored in link_field.
//
// 

#include "node.h"
#include <cassert>    // Provides assert
#include <cstdlib>    // Provides NULL and size_t
#include <iostream>

using namespace std;

namespace coen79_lab6
{
    size_t list_length(const node* head_ptr) {
	// Library facilities used: cstdlib
		const node *cursor;
		size_t answer;

		answer = 0;
		for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
			++answer;

		return answer;
    }

    void list_head_insert(node*& head_ptr, const node::value_type& entry) {
    	head_ptr = new node(entry, head_ptr);
    }

    void list_insert(node* previous_ptr, const node::value_type& entry) {
    	node *insert_ptr;

    	insert_ptr = new node(entry, previous_ptr->link( )); // new node with entry, points to what previous node points to
    	previous_ptr->set_link(insert_ptr);
    }

    node* list_search(node* head_ptr, const node::value_type& target) {
    	node *cursor;

    	for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
    	    if (target == cursor->data( ))
        		return cursor;

    	return NULL;
    }

    const node* list_search(const node* head_ptr, const node::value_type& target) {
	// Library facilities used: cstdlib
    	const node *cursor;

    	for (cursor = head_ptr; cursor != NULL; cursor = cursor->link( ))
    	    if (target == cursor->data( ))
        		return cursor;

    	return NULL;
    }

    node* list_locate(node* head_ptr, size_t position) {
	// Library facilities used: cassert, cstdlib
    	node *cursor;
    	size_t i;

    	assert (0 < position);
    	cursor = head_ptr;
    	for (i = 1; (i < position) && (cursor != NULL); i++)
    	    cursor = cursor->link( );

    	return cursor;
    }

    const node* list_locate(const node* head_ptr, size_t position) {
    // Library facilities used: cassert, cstdlib
    	const node *cursor;
    	size_t i;

    	assert (0 < position);
    	cursor = head_ptr;
    	for (i = 1; (i < position) && (cursor != NULL); i++)
    	    cursor = cursor->link( );

    	return cursor;
    }

    void list_head_remove(node*& head_ptr) {
    	node *remove_ptr;

    	remove_ptr = head_ptr;
    	head_ptr = head_ptr->link( );
    	delete remove_ptr; // free memory previously held by head_ptr
    }

    void list_remove(node* previous_ptr) {
    	node *remove_ptr;

    	remove_ptr = previous_ptr->link( ); // remove_ptr becomes node to delete
    	previous_ptr->set_link( remove_ptr->link( ) ); // prev_ptr points to node after remove_ptr
    	delete remove_ptr;
    }

    void list_clear(node*& head_ptr) {
	// Library facilities used: cstdlib
    	while (head_ptr != NULL)
    	    list_head_remove(head_ptr);
    }

    void list_copy(const node* source_ptr, node*& head_ptr, node*& tail_ptr) {
    // Library facilities used: cstdlib
    	head_ptr = NULL;
    	tail_ptr = NULL;

    	// Handle the case of the empty list.
    	if (source_ptr == NULL)
    	    return;

    	// Make the head node for the newly created list, and put data in it.
    	list_head_insert(head_ptr, source_ptr->data( ));
    	tail_ptr = head_ptr;

    	// Copy the rest of the nodes one at a time, adding at the tail of new list.
    	source_ptr = source_ptr->link( );
    	while (source_ptr != NULL) {
    	    list_insert(tail_ptr, source_ptr->data( ));
    	    tail_ptr = tail_ptr->link( );
    	    source_ptr = source_ptr->link( );
    	}
    }


	// functions to implement
	//
	//

	// list_piece()
	//
	// precondition: start_ptr and end_ptr are pointers on same linked list
	// where start_ptr node is at or before end_ptr node
	//
	// postcondition: head_ptr and tail_ptr are head and tail pointers for
	// new list that contains items from start_ptr up to but not including
	// end_ptr
	//
	// end_ptr may be NULL, in which case the list contains elements
	// from start_ptr to end of list
	void list_piece(node *start_ptr, node *end_ptr,	node *&head_ptr, node *&tail_ptr) {
		head_ptr = NULL;
		tail_ptr = NULL;

		// handle cases: empty list or start_ptr equals end_ptr
		if(start_ptr == NULL || start_ptr == end_ptr)
			return;

		// copy nodes one at a time, adding at tail of new list
		// until end_ptr is reached or start_ptr == NULL
		while(start_ptr != NULL && start_ptr != end_ptr) {
			if(head_ptr == NULL) {
				// make head node for new list, insert data
				list_head_insert(head_ptr, start_ptr->data());
				tail_ptr = head_ptr;
			} else {
				list_insert(tail_ptr, start_ptr->data());
				tail_ptr = tail_ptr->link();
			}

			start_ptr = start_ptr->link(); // iterate start_ptr
		}
	}

	// list_occurences()
	//
	// precondition: head_ptr is head pointer of linked list
	//
	// postcondition: return value is number of times
	// target appears as data portion of node on linked list
	// list is unchanged
	size_t list_occurrences(node *head_ptr, const node::value_type &target) {
		const node *cursor;
		size_t count = 0;

		for(cursor = head_ptr; cursor != NULL; cursor=cursor->link())
			if(cursor->data() == target)
				++count;
		
		return count;
	}

	// list_insert_at()
	//
	// precondition: head_ptr is head pointer of linked list,
	// position > 0, position <= list_length(head_ptr)+1
	//
	// postcondition: new node added to linked list,
	// entry as data
	// new node occurs at specified position in list
	// where head node is position 1, next is position 2, etc.
	// nodes following new node shifted to make room
	void list_insert_at(node *&head_ptr, const node::value_type &entry, size_t position) {
		assert(position > 0);
		assert(position <= list_length(head_ptr)+1); // preconditions


		if(position == 1) {// insert at head
			list_head_insert(head_ptr, entry);
			return;
		}
		
		node *cursor;
		cursor = head_ptr;
		for(size_t i=2; i<position; ++i) // iterate to node BEFORE position
			cursor = cursor->link();
		
		list_insert(cursor, entry); // inserts to node after cursor with entry as data
	}

	// list_remove_at()
	//
	// precondition: head_ptr is head pointer of linked list,
	// position > 0, position <= list_length(head_ptr)
	//
	// postcondition: node at specified position has been removed
	// from linked list, function returns copy of data from removed
	// node
	node::value_type list_remove_at(node *&head_ptr, size_t position) {
		assert(position > 0);
		assert(position <= list_length(head_ptr)); // preconditions

		if(position == 1) {// position == 1, remove head
			node::value_type temp = head_ptr->data();
			list_head_remove(head_ptr);
			return temp;
		}

		node *cursor;
		cursor = head_ptr;
		for(size_t i=2; i<position; ++i) // iterate to node BEFORE position
			cursor = cursor->link();

		node::value_type temp = cursor->link()->data();
		// get copy of data from node to remove

		list_remove(cursor);
		return temp;
	}

	// list_copy_segment()
	//
	// precondition: head_ptr is head pointer of linked list,
	// 1 <= start, start <= finish, finish <= list_length(head_ptr)
	//
	// postcondition: value returned is head pointer for new list
	// contains copies of items from start to finish position
	// list pointed to by head_ptr is unchanged
	node* list_copy_segment(node *head_ptr, size_t start, size_t finish) {
		assert(start >= 1);
		assert(start <= finish);
		assert(finish <= list_length(head_ptr)); // preconditions

		node *cursor, *source_ptr, *tail_ptr; // functions returns source_ptr
		cursor = head_ptr;
		for(size_t i=1; i<start; ++i) // iterate to node at start position
			cursor = cursor->link();
		
		source_ptr = new node(cursor->data()); // source_ptr becomes head_node

		// handle case of length 1
		if(start == finish)
			return source_ptr;

		source_ptr->set_link(new node(cursor->data()));
		tail_ptr = source_ptr->link();
		// link source_ptr to tail_ptr
		for(size_t i=start; i<finish; ++i) { // iterate to node at finish position
			cursor = cursor->link();
			tail_ptr->set_data(cursor->data());
			// set tail_ptr to cursor

			if(i+1 != finish) { // another node to add to new list
				tail_ptr->set_link(new node(cursor->data()));
				// sets link of tail pointer
				tail_ptr = tail_ptr->link();
				// iterates to new tail
			}
		}
		return source_ptr;
	}

	// list_remove_dups()
	//
	// precondition: head_ptr is head pointer of linked list
	//
	// postcondition: all duplicates removed from linked list
	// ex: if list contains 1,1,1,2
	// list contains 1, 2 after running function
	void list_remove_dups(node *head_ptr) {
		if(head_ptr->link() != NULL)
			list_remove_dups(head_ptr->link()); // calls function recursively

		node *cursor;
		for(cursor=head_ptr; (cursor != NULL) && (cursor->link() != NULL); cursor=cursor->link()) {
			node *cursor2 = cursor->link();
			if(cursor2->data() == head_ptr->data()) // duplicate found
				list_remove(cursor);
		}
	}			


	// list_print()
	//
	// precondition: head_ptr is head pointer of linked list
	//
	// operator << defined for value_type
	//
	// postcondition: value_type of all nodes in linked list are printed
	void list_print(const node *head_ptr) {
		const node *cursor;

		for(cursor = head_ptr; (cursor != NULL) && (cursor->link() != NULL); cursor=cursor->link())
			std::cout << cursor->data() << ", "; // print value type of nodes
		
		if(cursor != NULL) // one more pointer to print
			std::cout << cursor->data() << std::endl;
		else
			std::cout << std::endl;
	}

	// list_detect_loop()
	//
	// precondition: head_ptr is head pointer of linked list
	//
	// uses floyd's cycle finding algorithm
	//
	// postcondition: if there is loop in linked list, returned value
	// is pointer to start of loop
	// otherwise return value is NULL
	node* list_detect_loop(node *head_ptr) {
		node *fast, *slow;

		fast = head_ptr;
		slow = head_ptr;

		// floyd's cycle finding algorithm
		while(slow != NULL && fast != NULL && fast->link() != NULL) {
			slow = slow->link();
			fast = fast->link()->link();

			// loop found, reset slow pointer and find starting location
			if(slow == fast) {
				slow = head_ptr;

				while(slow != fast) {
					slow = slow->link();
					fast = fast->link();
				}

				return slow;
			}
		}
	
	// no loop found in list
	return NULL;	
	}
}
