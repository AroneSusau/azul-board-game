#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <vector>
#include <iostream>
#include <tuple>
#include <iomanip>

#include "Colour.h"
#include "Saver.h"
#include "Printer.h"
#include "BaseEngine.h"

class Validator {

  public: 
  
    Validator();
    ~Validator();

    // validates input for each turn
    bool validateTurnInput(std::vector<std::string> moves, BaseEngine* gameEngine);

    // validates input for discard turn
    bool validateDiscardInput(std::vector<std::string> moves, BaseEngine* gameEngine);

  private:

    Printer* printer;

    // validates factory input for turn command
    bool validateFactoryInput(int input, Colour colour, BaseEngine* gameEngine);

    // validates colour input for turn command
    bool validateColourInput(Colour input, BaseEngine* gameEngine);

    // validates row input for turn command
    bool validateRowInput(int input, BaseEngine* gameEngine);

};

#endif