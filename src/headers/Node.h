#ifndef NODE_H
#define NODE_H

#include "Tile.h"
#include "Colour.h"

class Node {
  
  public:
    Node(Colour colour, Node* next, Node* prev);
    Node(Node* &other);
    ~Node();

    // Returns the tile associated with this node
    Tile* getData();

    // Gets the next node in the linked list
    Node* getNext();

    // Gets the previous node in the linked list
    Node* getPrev();

    // Sets the colour of the tile this node's associated with
    void setDataColour(Colour colour);

    // Sets the tile this node's associated with
    void setDataTile(Tile* data);

    // Sets the next node object this node is pointing to in the list
    void setNext(Node* next);

    // Sets the next node object this node is pointing from in the list
    void setPrev(Node* prev);

    // Places this node inbetween two specified nodes
    void link(Node* next, Node* prev);

  private:
    Tile* data;
    Node* next;
    Node* prev;

};

#endif
