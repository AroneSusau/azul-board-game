#include "../headers/Wall.h"

template <typename T>
Wall<T>::Wall() :
  tiles(new T**[DIMENSIONS]()),
  printer(new Printer())
{
  for (int i = 0; i < DIMENSIONS; ++i) {
    tiles[i] = new T*[DIMENSIONS]();
    for (int j = 0; j < DIMENSIONS; ++j) {
      tiles[i][j] = new T();
    } 
  }
}

template <typename T>
Wall<T>::~Wall() {
  for (int i = 0; i < DIMENSIONS; ++i) {
    for (int j = 0; j < DIMENSIONS; ++j) {
      delete tiles[i][j];
    } 
    delete tiles[i];
  }

  delete [] tiles;
  delete printer;
}

template <typename T>
T* Wall<T>::get(int row, int col) {
  T* value = nullptr;
  
  if (row >= 0 && row < DIMENSIONS && col >= 0 && col <= DIMENSIONS) {
    value = tiles[row][col];
  } else {
    printer->error("Error: Attempting to access out of bounds Wall tile.");
  }

  return value;
}

template <typename T>
void Wall<T>::set(int row, int col, T* value) {
  if (row >= 0 && row < DIMENSIONS && col >= 0 && col <= DIMENSIONS) {
    tiles[row][col] = value;
  } else {
    printer->error("Error: Attempting to access out of bounds Wall tile.");
  }
}

template class Wall<bool>;
template class Wall<Colour>;
