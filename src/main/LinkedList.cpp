#include "../headers/LinkedList.h"

LinkedList::LinkedList() :
  head(nullptr),
  tail(nullptr)
{}

LinkedList::~LinkedList() {
  
  Node* curr = head;
  
  while (curr != nullptr) {
    
    Node* next = nullptr;

    if (curr != nullptr) {
      next = curr->getNext();
    }
    
    delete curr;

    curr = next;
  }
}

Node* LinkedList::get(int index) {

  Node* curr = head;

  for(int i = 0; i < index; i++) {
    curr = curr->getNext();
  }

  return curr;
}

int LinkedList::size() {
  int count = 0;
  Node* curr = head;

  while (curr != nullptr) {
    ++count;
    curr = curr->getNext();
  }

  return count;
}


void LinkedList::clear() {
  head = nullptr;  
}

int LinkedList::remove(Colour colour) {
  
  Node* curr = head;
  int count = 0;

  while (curr != nullptr) {
    if (curr->getData()->getColour() == colour) {
    
      Node* toDelete = curr;
      Node* prev = toDelete->getPrev();
      Node* next = toDelete->getNext();

      if (prev != nullptr) {
        prev->setNext(next);
      }

      if (next != nullptr) {
        next->setPrev(prev);
      }

      ++count;
    }

    curr = curr->getNext();
  }
  
  return count;
}

int LinkedList::remove(int index) {
  
  int deleted = 0;
  
  if (index < size()) {
    
    Node* toDelete = get(index);
    Node* prev = toDelete->getPrev();
    Node* next = toDelete->getNext();
    
    if (prev != nullptr) {
      prev->setNext(next);
    }

    if (next != nullptr) {
      next->setPrev(prev);
    }

    if (index == 0) {
      head = next;
    }

    ++deleted;
    
  } else {
    std::cout << "Error: Attempting to remove node from linked list outside of size." << std::endl;
  }

  return deleted;
}

void LinkedList::addFront(Colour toAdd) {
  Node* add = new Node(toAdd, head, nullptr);

  if (tail == nullptr) {
    tail = head;
  }

  head = add;
}

void LinkedList::addBack(Colour toAdd) {
  Node* add = new Node(toAdd, nullptr, tail);
  
  if (head == nullptr) {
    head = add;
  }
  
  tail = add;
}

Node* LinkedList::getHead() {
  return head;
}

Node* LinkedList::getTail() {
  return tail;
}
