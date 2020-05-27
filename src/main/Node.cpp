#include "../headers/Node.h"

Node::Node(Colour colour, Node* next, Node* prev):
  data(new Tile(colour)),
  next(next),
  prev(prev)
{
  link(next, prev);
}

Node::Node(Node* &other) :
  data(new Tile(other->getData()->getColour())),
  next(other->getNext()),
  prev(other->getPrev())
{
  link(next, prev);
}

Node::~Node() {
  delete data;
}

Tile* Node::getData() {
  return data;
}

Node* Node::getNext() {
  return next;
}

Node* Node::getPrev() {
  return prev;
}

void Node::link(Node* next, Node* prev) {
  if (next != nullptr) {
    next->setPrev(this);
  }

  if (prev != nullptr) {
    prev->setNext(this);
  }
}

void Node::setDataColour(Colour colour) {
  data = new Tile(colour);
  data->setColour(colour);
}

void Node::setDataTile(Tile* data) {
  this->data = data;
}

void Node::setNext(Node* next) {
  this->next = next;
}

void Node::setPrev(Node* prev) {
  this->prev = prev;
}
