#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

#include "Node.h"
#include "Printer.h"

class LinkedList {
  
  public:
    LinkedList();
    ~LinkedList();

    Node* get(int index);

    // Returns the size of the linked list
    int size();
    
    // Deletes all the nodes in the linked list
    void clear();

    // Searches the linked list and removes tiles with same colour, returns the
    // number of tiles removed
    int remove(Colour colour);

    // Searches the linked list and removes the tile at the specified index
    int remove(int index);

    // Adds a tile with the colour 'toAdd' to the front of the linked list
    void addFront(Colour toAdd);

    //Adds a tile with the colour 'toAdd' to the end of the linked list
    void addBack(Colour toAdd);

    // returns the head node
    Node* getHead();

    // returns the tail node
    Node* getTail();
    
  private:
    Node* head;
    Node* tail;
    Printer* printer;
    
};

#endif
