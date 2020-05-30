#include "../headers/CentreFactory.h"

CentreFactory::CentreFactory():
  tiles(new std::vector<Tile*>()),
  hasToken(true),
  printer(new Printer())
{}

CentreFactory::~CentreFactory() {
  for (int i = 0; i < (int) tiles->size(); ++i) {
    delete tiles->at(i);
  }

  delete tiles;
  delete printer;
}

Colour CentreFactory::get(int index) {
  Colour colour = EMPTY;
  
  if (index >= 0 && index < (int) tiles->size()) {
    colour = tiles->at(index)->getColour();
  } else {
    printer->error("Error: Attempting to access out of bounds Centre Factory tile.");
  }

  return colour;
}

void CentreFactory::add(Colour colour) {
  tiles->push_back(new Tile(colour));
}

void CentreFactory::remove(Colour colour) {
  
  int size = (int) tiles->size();

  for (int i = 0; i < size; ++i) {
    if (tiles->at(i)->getColour() == colour) {
      delete tiles->at(i);
      tiles->erase(tiles->begin() + i);
      i = -1;
      size = (int) tiles->size();
    }
  }
}

void CentreFactory::remove(int index) {
  if (index >= 0 && index < (int) tiles->size()) {

    for (int i = 0; i < (int) tiles->size(); ++i) {
      if (i == index) {
        delete tiles->at(i);
        tiles->erase(tiles->begin() + i);
      }
    }

  } else {
    printer->error("Error: Attempting to access out of bounds Centre Factory tile.");
  }
}

void CentreFactory::clear() {
  tiles->clear();
}

int CentreFactory::count(Colour colour) {

  int amount = 0;

  for (int i = 0; i < (int) tiles->size(); ++i) {
    if (tiles->at(i)->getColour() == colour) {
      amount++;
    }
  }

  return amount;  
}

void CentreFactory::setToken(bool token) {
  hasToken = token;
}

bool CentreFactory::getToken() {
  return hasToken;
}

bool CentreFactory::isEmpty() {
  bool empty = true;

  if(tiles->size() > 0) {
    empty = false;
  }

  return empty;
}

int CentreFactory::size() {
  return tiles->size();
}

std::string CentreFactory::toString() {
  std::string output = ": ";

  if(hasToken) {
    output += BG_WHITE;
    output += C_BLACK;
    output += " ";
    output += (char) Colour::FIRST_PLAYER;
    output += " ";
    output += C_RESET;
  }

  for(int i = 0; i < (int) tiles->size(); ++i) {

    Colour colour = tiles->at(i)->getColour();

    if (colour == RED) {
      output += BG_RED;
    } else if (colour == YELLOW) {
      output += BG_YELLOW;
      output += C_BLACK;
    }  else if (colour == LIGHT_BLUE) {
      output += BG_CYAN;
    }  else if (colour == DARK_BLUE) {
      output += BG_BLUE;
    }  else if (colour == BLACK) {
      output += BG_BLACK;
    }  else if (colour == EMPTY) {
      output += BG_WHITE;
      output += C_BLACK;
    }
    
    output += " ";
    output += (char) colour;
    output += " ";
    output += C_RESET;
  }

  return output;
}

std::string CentreFactory::toSaveString() {
  std::string output = "";

  if ((int) tiles->size() > 0) {

    for (int i = 0; i < (int) tiles->size(); ++i) {
      output += (char) tiles->at(i)->getColour();
    }

  } else {
    output += "=";
  } 

  return output;
}
