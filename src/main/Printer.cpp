#include "../headers/Printer.h"

Printer::Printer() {}

Printer::~Printer() {}

char Printer::inputChar() {
  char input = ' ';
  
  std::cout << "> ";
  std::cin >> input;
  std::cin.ignore(10000, '\n');

  return input;
}

std::vector<std::string> Printer::inputArgs() {
  std::string input = "";
  std::string word = "";
  std::vector<std::string> args;
  
  std::cout << "> ";
  std::getline(std::cin, input);

  for (int i = 0; i < (int)input.size(); i++) {
    
    if (input[i] != ' ' && input[i] != '\n') {
      word += input[i];
    } else {
      args.push_back(word);
      word = "";
    }
    
  }

  args.push_back(word);

  return args;
}

std::string Printer::inputString() {
  std::string input = "";
  std::string word = "";
  
  std::cout << "> ";
  std::getline(std::cin, input);

  for (int i = 0; i < (int) input.size(); ++i) {
    
    if (input[i] != ' ' && input[i] != '\n') {
      word += input[i];
    }
    
  }

  return word;
}

void Printer::log(std::string message) {
  std::cout << message << std::endl;
}

void Printer::error() {
  std::cout << "Wrong input" << std::endl;
}

void Printer::menu() {
  std::cout << "Enter a number\n1. Start Game\n2. Load Game\n3. Credits\n4. Exit" << std::endl;
}

void Printer::credits() {
  std::cout <<"Name: Arone Susau\nStudent ID: s3728977\nEmail: s3728977@student.rmit.edu.au\n\n";
  std::cout <<"Name: Julian Bitetto\nStudent ID: s3690935\nEmail: s3690935@student.rmit.edu.au\n\n";
  std::cout <<"Name: Travis Stella\nStudent ID: s3783006\nEmail: s3783006@student.rmit.edu.au\n\n";
}

void Printer::helpMenu() {
  std::cout << std::endl;
  std::cout << "=== Help Menu ===" << std::endl;
  std::cout << "command: example - description" << std::endl;
  std::cout << std::endl;
  std::cout << "turn: turn <factory> <colour> <pile> - Player turns, moving tiles from factory to the player pile line." << std::endl;
  std::cout << "discard: discard <factory> <colour> - Player turns, moving tiles from factory to the player discard pile." << std::endl;
  std::cout << "display: display - Displays all factories and the active players mosaic." << std::endl;
  std::cout << "save: save <filename> - Saves the game to the specified filename." << std::endl;
  std::cout << "help: help - Prints this help menu." << std::endl;
  std::cout << "exit OR ^D (EOF command): exit - Exits the game." << std::endl;
}

void Printer::exit() {
  std::cout << "See ya :D" << std::endl;
}
