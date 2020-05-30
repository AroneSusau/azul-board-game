#include "../headers/Validator.h"

Validator::Validator() :
  printer(new Printer())
{}

Validator::~Validator() {
  delete printer;
}

bool Validator::validateTurnInput(std::vector<std::string> moves, BaseEngine* gameEngine) {
  bool valid = false;
  bool validArgumentCount = moves.size() == 4;

  if (validArgumentCount) {
    try {
      int factory = std::stoi(moves.at(1));
      int row = std::stoi(moves.at(3));
      Colour colour = (Colour) moves.at(2)[0];
      Player* player = gameEngine->getPlayer(gameEngine->getActivePlayer());
      
      bool validFactory = validateFactoryInput(factory, colour, gameEngine);
      bool validRow = validateRowInput(row, gameEngine);
      bool validColour = validateColourInput(colour, gameEngine);
      
      bool validPatternInput = player->getMosaic()->validatePatternLineInput(colour, row - 1);
      bool validWallInput = player->getMosaic()->validateWallColourInput(colour, row - 1);
      
      valid = validFactory && validRow && validColour && validPatternInput && validWallInput;

    } catch (std::runtime_error &e) {
      printer->error("Error: Invalid arguments passed in for discard command.");
    } catch(std::invalid_argument &e) {
      printer->error("Error: Bad character passed in for turn command argument.");
    }
  } else {
    printer->error("Error: Invalid argument count.");
  }

  return valid;
}

bool Validator::validateDiscardInput(std::vector<std::string> moves, BaseEngine* gameEngine) {
  bool valid = false;
  bool validArgumentCount = moves.size() == 3;
  
  if (validArgumentCount) {
    try {
      int factory = std::stoi(moves.at(1));
      Colour colour = (Colour) moves.at(2)[0];
      
      bool validFactory = validateFactoryInput(factory, colour, gameEngine);
      bool validColour = validateColourInput(colour, gameEngine);
      valid = validFactory && validColour;

    } catch (std::runtime_error &e) {
      printer->error("Error: Invalid arguments passed in for discard command. Use 'help' for command usage.");
    } catch(std::invalid_argument &e) {
      printer->error("Error: Bad character passed in for row argument.");
    }
  } else {
    printer->error("Error: Invalid arguments passed in for discard command. Use 'help' for command usage.");
  }

  return valid;
}

bool Validator::validateFactoryInput(int input, Colour colour, BaseEngine* gameEngine) {
  
  bool valid = true;
  bool validFactory = input >= 0 && input < gameEngine->getFactoryLength() + gameEngine->getCentreFactoryLength();
  
  if (validFactory) {

    int index = input;
    bool isCentreFactory = index == 0 || index == 1;
    bool isNormalFactory = index >= 2;
    bool factoryEmpty = false;
    bool containsColour = false;
  
    if (isCentreFactory) {
      factoryEmpty = gameEngine->getCentreFactory(index)->isEmpty();
      containsColour = gameEngine->getCentreFactory(index)->count(colour) < 1;
    } else if (isNormalFactory) {
      index -= 2;
      factoryEmpty = gameEngine->getFactory(index)->isEmpty();
      containsColour = gameEngine->getFactory(index)->count(colour) < 1;
    }

    if (factoryEmpty) {
      printer->error("Error: " + std::to_string(index) + " is an empty factory");
      valid = false;
    } else if (containsColour) {
      printer->error("Error: Factory does not contain colour of type " + std::string(1, (char) colour) + ".");
      valid = false;
    }

  } else {
    printer->error("Error: " + std::to_string(input) + " is not a valid factory");
    valid = false;
  }

  return valid;
}

bool Validator::validateColourInput(Colour input, BaseEngine* gameEngine) { 

  const int length = 5;
  Colour colours[5] = {RED, YELLOW, DARK_BLUE, LIGHT_BLUE, BLACK};
  bool valid = true;
  bool matchFound = false;

  for (int i = 0; i < length; ++i) {
    if (input == colours[i]) {
      matchFound = true;
    }
  }

  if (!matchFound) {
    valid = false;
    printer->error("Error: " + std::string(1, (char) input) + " is not a valid colour tile");
  }

  return valid;
}

bool Validator::validateRowInput(int input, BaseEngine* gameEngine) {

  bool valid = true;
  bool validRow = input > 0 && input <= ROWS;

  if(!validRow) {
    printer->error("Error: " + std::to_string(input) + " is not a valid row");
    valid = false;
  }

  return valid;
}