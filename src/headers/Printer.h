#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <string>
#include <vector>

class Printer {

  public:
    Printer();
    ~Printer();

    // Allows the user to input a char then returns it
    char inputChar();

    // Allows the user to input a line, then returns the words
    std::vector<std::string> inputArgs();

    // Allows the user to input a word, then returns it
    std::string inputString();

    // Prints a message to output
    void log(std::string message);

    // Notifies the user of an invalid input
    void error();

    // Prints out the options of the main menu
    void menu();

    // Prints program's authors' details
    void credits();

    // Exits the program
    void exit();

    // Displays command information
    void helpMenu();

};

#endif
