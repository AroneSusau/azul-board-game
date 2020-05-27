#include "../headers/Printer.h"

Printer::Printer() {}

Printer::~Printer() {}

char Printer::inputChar() {
  char input = ' ';
  
  cursor();
  std::cin >> input;
  std::cin.ignore(10000, '\n');

  return input;
}

std::vector<std::string> Printer::inputArgs() {
  std::string input = "";
  std::string word = "";
  std::vector<std::string> args;
  
  cursor();
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

void Printer::startPlay() {
  std::cout << BG_YELLOW;
  std::cout << C_BLACK << "Lets play!" << std::endl;
  std::cout << C_RESET;
}

std::string Printer::inputString() {
  std::string input = "";
  std::string word = "";
  
  cursor();
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

void Printer::clear() {
  system("clear");
}

void Printer::error() {
  std::cout << std::endl;
  std::cout << C_RED << "Error: Invalid input." << C_RESET << std::endl;
  pause();
}

void Printer::error(std::string message) {
  std::cout << std::endl;
  std::cout << C_RED << message << C_RESET << std::endl;
  pause();
}

void Printer::title() {
  std::cout << std::endl;
  std::cout << C_BLUE << "       db" << C_MAGENTA << "   8888P" << C_RED << " 8    8" << C_YELLOW << " 8    " << std::endl;
  std::cout << C_BLUE << "     dPYb" << C_MAGENTA << "    dP  " << C_RED << "8    8" << C_YELLOW << " 8     " << std::endl;
  std::cout << C_BLUE << "   dPwwYb" << C_MAGENTA << "  dP   " << C_RED << "8b..d8" << C_YELLOW << " 8      " << std::endl;
  std::cout << C_BLUE << " dP    Yb" << C_MAGENTA << " d8888 " << C_RED << "`Y88P'" << C_YELLOW << " 8888   " << std::endl;
  std::cout << C_RESET << std::endl;
}

void Printer::pause() {
  std::cout << std::endl;
  std::cout << C_DARK << "Press enter to continue.." << C_RESET;
  std::cin.ignore();
}

void Printer::cursor() {
  std::cout << C_GREEN << "> " << C_RESET;
}

void Printer::menu() {
  std::cout << "Enter a number" << std::endl;
  std::cout << C_WHITE << "1." << C_DARK << " Start Game" << C_RESET << std::endl; 
  std::cout << C_WHITE << "2." << C_DARK << " Load Game" << C_RESET << std::endl;
  std::cout << C_WHITE << "3." << C_DARK << " Credits" << C_RESET << std::endl;
  std::cout << C_WHITE << "4." << C_DARK << " Exit" << C_RESET << std::endl;

  std:: cout << C_RESET;
}

void Printer::credits() {
  std::cout << std::endl;
  
  std::cout << "Name: Arone Susau\nStudent ID: s3728977\nEmail: s3728977@student.rmit.edu.au" << std::endl;
  std::cout << std::endl;
  std::cout << "Name: Julian Bitetto\nStudent ID: s3690935\nEmail: s3690935@student.rmit.edu.au" << std::endl;
  
  std::cout << std::endl;

  std::cout << "Name: Travis Stella\nStudent ID: s3783006\nEmail: s3783006@student.rmit.edu.au" << std::endl;

  std::cout << std::endl;
}

void Printer::helpMenu() {
  std::cout << std::endl;
  std::cout << "=== Help Menu ===" << std::endl;
  std::cout << "COMMAND:" << std::setw(14) << "EXAMPLE" << std::setw(40) << "DESCRIPTION" << std::endl;
  std::cout << std::endl;
  std::cout << "turn:" << std::setw(40) << "turn <factory> <colour> <pile>" << std::setw(70) << "Player turns, moving tiles from factory to the player pile line." << std::endl;
  std::cout << "discard:" << std::setw(33) << "discard <factory> <colour>" << std::setw(77) << "Player turns, moving tiles from factory to the player discard pile." << std::endl;
  std::cout << "display:" << std::setw(14) << "display" << std::setw(82) << "Displays all factories and the active players mosaic." << std::endl;
  std::cout << "save:" << std::setw(25) << "save <filename>" << std::setw(62) << "Saves the game to the specified filename." << std::endl;
  std::cout << "help:" << std::setw(14) << "help" << std::setw(54) << "Prints this help menu." << std::endl;
  std::cout << "exit / ^D:" << std::setw(9) << "exit" << std::setw(47) << "Exits the game." << std::endl;
}

void Printer::exit() {
  clear();
  std::cout << std::endl;
  std::cout << C_GREEN << " __                      _        _                " << std::endl;
  std::cout << C_GREEN << " //_ _  _  /_ /_/_      /_`_  _  /_//_     ._  _  /" << std::endl;
  std::cout << C_GREEN << "// //_|/ //\\  / /_//_/ /  /_//  /  //_|/_/// //_/. " << std::endl;
  std::cout << C_GREEN << "                                       _/     _/   " << std::endl;
  std::cout << C_RESET << std::endl;
}
