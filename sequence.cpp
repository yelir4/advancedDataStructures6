//FILE: sequence.cpp
//CLASS IMPLEMENTED: sequence (see sequence.h for documentation)
//
//
//  COEN 79
//  --- Farokh Eskafi, COEN, SCU ---
//
//
//INVARIANT for the sequence class
//  1. The number of items in the sequence is stored in the member variable "many_nodes"
//
//  2. For an empty sequence, we do not care what is stored in any of data and head_ptr
//      and tail_ptr point to NULL; for a non-empty sequence, the items stored in the
//      sequence can be accessed using head_ptr and tail_ptr and we don't care what is
//      stored in the rest of the data.
//
//  3. If there is a current item, then it lies in *cursor; if there is no
//     current item, then cursor* equals NULL.
//
//  4. If there is a previous item, then it lies in precursor*.  If there is no previous
//       item, then precursor equals NULL.


#include <iostream>
#include <algorithm> //provides copy function
#include <cassert> //for assert function
#include "sequence.h" //header file for class
#include "node.h" // provides node class

using namespace std; //For copy function

namespace coen79_lab6
{
    // Default private member variable initialization function.
    void sequence::init() {
        //Initializing pointers to NULL
        head_ptr = NULL;
        tail_ptr = NULL;
        cursor = NULL;
        precursor = NULL;
        //Initializiing many_nodes (or our counter) to 0
        many_nodes = 0;
    }

    //CONSTRUCTOR IMPLEMENTATION for default constructor
    sequence::sequence () {
        init();
    }

    // Copy Constructor
    sequence::sequence(const sequence& source) {
        // handle case: no current item
        if(source.is_item() == false) {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            precursor = NULL;
            cursor = NULL;

        // handle case: current item is first item
        } else if(source.cursor == source.head_ptr) {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            precursor = NULL;
            cursor = head_ptr;
        
        // handle all other cases
        } else {
            list_piece(source.head_ptr, source.cursor, head_ptr, precursor);
            list_piece(source.cursor, source.tail_ptr->link(), cursor, tail_ptr);

            precursor->set_link(cursor);
        }
    }

    // destructor
    sequence::~sequence() {
        list_clear(head_ptr);
    }

    // start()
    //
    // postcondition: precursor & cursor adjusted such that
    // first item in sequence is current item
    //
    // if sequence is empty, there is no current item
    void sequence::start() {
        if(many_nodes == 0) { // handle empty sequence case
            precursor = NULL;
            cursor = NULL;
            return;
        }

        

        precursor = NULL;
        cursor = head_ptr;
    }

    // end()
    //
    // postcondition: precursor & cursor adjusted such that
    // last item in sequence is current item
    //
    // if sequence is empty, there is no current item
    void sequence::end() {
        // handle case: empty sequence
        if(many_nodes == 0) {
            precursor = NULL;
            cursor = NULL;
            return;
        
        // handle case: one node in sequence
        } else if(many_nodes == 1) {
            precursor = NULL;
            cursor = tail_ptr;

        // rest of cases
        } else {
            precursor = head_ptr;
            cursor = tail_ptr;
            while((precursor->link() != tail_ptr))
                precursor = precursor->link();
        }
    }

    // advance()
    //
    // precondition: is_item returns true
    //
    // postcondition: if current item was last item in sequence,
    // there is no longer any current item
    // otherwise, current item is item immediately
    // after original current item
    void sequence::advance() {
        assert(is_item()); // precondition

        if(cursor == tail_ptr) {
            cursor = NULL;
        } else {
            precursor = cursor;
            cursor = cursor->link();
        }
    }

    // insert()
    //
    // postcondition: new copy of entry inserted in sequence
    // before current item
    // if no current item, new entry inserted at front of
    // sequence
    //
    // newly inserted item is now current item of sequence
    void sequence::insert(const value_type &entry) {
        node *insert; // node to insert

        if(is_item()) { // there is current item in cursor
            insert = new node(entry, cursor);
            if(precursor != NULL) // not first item
                precursor->set_link(insert);
            else
                head_ptr = insert;

            insert->set_link(cursor);
            cursor = insert;
        } else { // insert at front of sequence
            insert = new node(entry, head_ptr);
            precursor = NULL;
            head_ptr = insert;
            cursor = insert;
            if(cursor->link() == NULL)
                tail_ptr = insert;
        }
        ++many_nodes;
    }
    
    // attach()
    //
    // postcondition: new copy of entry inserted in sequence
    // after current item
    // if no current item, new entry inserted at end of
    // sequence
    //
    // newly inserted item is now current item of sequence
    void sequence::attach(const value_type &entry) {
        node *insert;

        if(is_item()) { // there is current item in cursor
            insert = new node(entry, cursor->link());
            cursor->set_link(insert);
            precursor = cursor;
            cursor = cursor->link();

            if(insert->link() == NULL)
                tail_ptr = cursor;

        } else {
            insert = new node(entry);

            if(many_nodes == 0) { // empty sequence
                precursor = NULL;
                cursor = insert;
                head_ptr = insert;
                tail_ptr = insert;
            } else { // insert at end of sequence
                precursor = tail_ptr;
                precursor->set_link(insert);
                cursor = insert;
                tail_ptr = insert;
            }
        }
        ++many_nodes;
    }

    // = assignment operator
    void sequence::operator =(const sequence &source) {
        // handle case: no current item
        if(source.is_item() == false) {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            precursor = NULL;
            cursor = NULL;

        // handle case: current item is first item
        } else if(source.cursor == source.head_ptr) {
            list_copy(source.head_ptr, head_ptr, tail_ptr);
            precursor = NULL;
            cursor = head_ptr;
        
        // handle all other cases
        } else {
            list_piece(source.head_ptr, source.cursor, head_ptr, precursor);
            list_piece(source.cursor, source.tail_ptr->link(), cursor, tail_ptr);

            precursor->set_link(cursor);
        }
    }

    // remove_current()
    //
    // precondition: is_item is true
    //
    // postcondition: current item removed from sequence
    // following item (if exists) is new current item
    void sequence::remove_current() {
        assert(is_item());

        // handle case: one node as current node
        if(many_nodes == 1) {
            cursor = NULL;
        } else if(cursor == head_ptr) { // remove head
            head_ptr = cursor->link();
            cursor = cursor->link();
        } else if(cursor == tail_ptr) { // remove tail
            tail_ptr = precursor;
            cursor = precursor;
            precursor = head_ptr;
            while((precursor->link() != tail_ptr))
                precursor = precursor->link();
            
            cursor->set_link(NULL);
        } else { // remove item at current position
            precursor->set_link(cursor->link());
            cursor = cursor->link();
        }
        --many_nodes;
    }

    // size()
    //
    // postcondition: return value is number of items in sequence
    sequence::size_type sequence::size() const {
        return many_nodes;
    }

    // is_item()
    //
    // postcondition: true return value indicated
    // there is valid "current" item that may be
    // retrieved by activating current() member function
    //
    // false return value indicates no valid current item
    bool sequence::is_item() const {
        return (cursor != NULL) ? 1 : 0;
    }

    sequence::value_type sequence::current() const {
        assert(is_item()); // precondition

        return cursor->data();
    }
}
