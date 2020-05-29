#include "../headers/GameEngine.h"

GameEngine::GameEngine() : 
  BaseEngine(), 
  saver(new Saver()) 
{}

GameEngine::GameEngine(int seats) : 
  BaseEngine(seats), 
  saver(new Saver())
{}

GameEngine::~GameEngine() {
  delete saver;
}

void GameEngine::run(bool isLoadedGame) {
  
  if (!isLoadedGame) {
    setupGame();
  } else {
    playing = true;
  }

  printer->clear();
  printer->startPlay();
  printer->helpMenu();
  printer->pause();
  printer->clear();

  while (playing && !std::cin.eof()) {

    if (!isLoadedGame) {
      setupRound();
    } else {
      isLoadedGame = false;
    }
        
    playMenu();

    if (!std::cin.eof() && playing) {
      endRound();
    }
  }

  resetGame();

}

void GameEngine::setupGame() {
  playing = true;
  setupPlayerCount();
  setupFactoryCount();
  bag->shuffle();
  setupPlayers();
}

void GameEngine::playMenu() {

  bool exit = false;

  while (!exit && !factoriesEmpty() && !std::cin.eof()) {

    bool turnInProgress = true;

    while (!exit && turnInProgress && !std::cin.eof()) {

      display();
      std::vector<std::string> moves = printer->inputArgs();
      std::string command = moves[0];

      if (command == TURN) {

        turnInProgress = !turn(moves);

      } else if (command == DISCARD) {

        turnInProgress = !discardTurn(moves);

      } else if (command == DISPLAY) {

        display();
    
      } else if (command == SAVE) {

        saver->save(moves[1], this);

      } else if (command == HELP) {

        printer->helpMenu();
        printer->pause();

      } else if (command == EXIT_GAME || std::cin.eof()) {

        playing = false;
        turnInProgress = false;
        exit = true;

      } else {

        printer->error();

      }

      printer->clear();
    }

    turns++;
    nextPlayer();
    printer->clear();

  }
}

bool GameEngine::turn(std::vector<std::string> moves) {
  
  bool validInput = validateTurnInput(moves);
  bool successfulTurn = false;

  if (validInput) {
    
    // parse move values
    int factory = std::stoi(moves.at(1)) - 1;
    int row = std::stoi(moves.at(3)) - 1;
    Colour colour = (Colour) moves.at(2)[0];

    bool isCentreFactory = factory == -1;
    std::tuple<int, bool> results = factoryMovement(isCentreFactory, factory, colour);
    int amount = std::get<0>(results);
    int firstPlayer = std::get<1>(results);
    
    if (firstPlayer) {
      players->at(active)->setStarter(firstPlayer);
    }
    
    players
      ->at(active)
      ->getMosaic()
      ->place(colour, amount, row, firstPlayer, bag);

    successfulTurn = true;

  }

  return successfulTurn;
}

bool GameEngine::discardTurn(std::vector<std::string> moves) {
  
  bool validInput = validateDiscardInput(moves);
  bool successfulTurn = false;

  if (validInput) {

    // parse move values
    int factory = std::stoi(moves.at(1)) - 1;
    Colour colour = (Colour) moves.at(2)[0];
    
    bool isCentreFactory = factory == -1;
    std::tuple<int, bool> results = factoryMovement(isCentreFactory, factory, colour);
    int amount = std::get<0>(results);
    bool firstPlayer = std::get<1>(results);

    for (int i = 0; i < amount; i++) {
      Colour toAdd = colour;

      if (firstPlayer) {
        toAdd = FIRST_PLAYER;
        firstPlayer = false;
        players->at(active)->setStarter(firstPlayer);
      } 

      bool added = players
        ->at(active)
        ->getMosaic()
        ->getDiscard()
        ->add(toAdd);

      if (!added && colour != EMPTY && colour != FIRST_PLAYER) {
        bag->addLidTile(toAdd);
      }
    }

    successfulTurn = true;
  }

   return successfulTurn;
}

void GameEngine::nextPlayer() {
  if (active < seats - 1) {
    active++;
  } else {
    active = 0;
  }
}

std::tuple<int, bool> GameEngine::factoryMovement(bool isCentreFactory, int factory, Colour colour) {

  bool firstPlayer = false;
  int amount = 0;

  if(isCentreFactory) {
      
      amount = centreFactory->count(colour);
      firstPlayer = centreFactory->getToken();
      
      if (firstPlayer) {
        centreFactory->setToken(false);
        players->at(active)->setStarter(true);
      }

      centreFactory->remove(colour);

    } else {

      amount = factories[factory]->count(colour);
      factories[factory]->remove(colour);
      
      for (int i = 0; i < FACTORIES_SIZE; i++) {
        
        Colour remaining = factories[factory]->get(i);
        
        if (remaining != EMPTY) {
          centreFactory->add(remaining);
          factories[factory]->update(i, EMPTY);
        }
      }
    }

  return std::make_tuple(amount, firstPlayer);
}

void GameEngine::setupRound() {
  fillFactories();
  setupStartingPlayer();
}

void GameEngine::endRound() {

  std::cout << "=== END OF ROUND ===" << std::endl;
  
  scoreRound();
  bag->refillBag();

  bool endConditionMet = checkEndCondition();

  if (endConditionMet) {
    endGameScoring();
    calculateWinner();
    playing = false;
  }

  printer->pause();
  printer->clear();
}

bool GameEngine::checkEndCondition() {
  
  bool result = false;
  
  for (int i = 0; i < (int) players->size(); ++i) {
    for (int row = 0; row < ROWS; ++row) {
      
      bool rowFull = players->at(i)->getMosaic()->getWall()->rowTrue(row);
      
      if (rowFull) {
        result = true;
      }
    }
  }
  
  return result;
}

void GameEngine::setupStartingPlayer() {

  active = -1;

  for (int i = 0; i < seats; ++i) {
    if (players->at(i)->getStarter()) {
      active = i;
      players->at(i)->setStarter(false);
    }
  }

  if (active == -1) {
    active = 0;
  }

}

void GameEngine::scoreRound() {

  for (int i = 0; i < (int) players->size(); ++i) {

    Player* player = players->at(i);

    for (int row = 0; row < ROWS; ++row) {
      bool rowFull = player->getMosaic()->getPattern()->isRowFull(row);

      if (rowFull) {
        Colour colour = (Colour) player->getMosaic()->getPattern()->getColour(row);
        int col = player->getMosaic()->getWall()->getColourDim(colour, row);
        int points = 0;

        player->getMosaic()->getWall()->getWall()->set(row, col, new bool(true));
        points += player->getMosaic()->getWall()->roundPoints(row, col);
        points -= player->getMosaic()->getDiscard()->getMinusPoints();

        player->addScore(points);
        
        player->getMosaic()->getPattern()->clearRow(row);
        
        for (int i = 0; i < player->getMosaic()->getDiscard()->count(); i++) {
          Colour colour = player->getMosaic()->getDiscard()->getColour(i);
          
          if (colour != EMPTY && colour != FIRST_PLAYER) {
            bag->addLidTile(colour);
          }
        }
      }
    }

    // Print player points
    std::cout << "Player " << player->getName() << " points: " << player->getScore() << std::endl;

    player->getMosaic()->getDiscard()->clear();
  }
}

void GameEngine::endGameScoring() {

  for (int i = 0; i < (int)players->size(); ++i) {
    
    Player* player = players->at(i);

    int points = player->getMosaic()->getWall()->endPoints();

    player->addScore(points);
  }
}

void GameEngine::calculateWinner() {
  
  std::string winner = "";
  int winningPoints = 0;

  for (int i = 0; i < (int) players->size(); i++) {
    
    std::string name = players->at(i)->getName();
    int score = players->at(i)->getScore();

    std::cout << "Player: " << name << " finished with " << score << " points." << std::endl;
    
    if (score > winningPoints) {
      winner = name;
      winningPoints = score;
    }
  }

  std::cout << "Player " << winner << " wins!! " << std::endl;

}

bool GameEngine::factoriesEmpty() {
  bool empty = true;

  for(int i = 0; i < factoryLength; i++) {
    if(!factories[i]->isEmpty()) {
      empty = false;
    }
  }

  if(!centreFactory->isEmpty()) {
    empty = false;
  }

  return empty;
}

void GameEngine::fillFactories() {
  
  for (int i = 0; i < factoryLength; ++i) {
    for (int j = 0; j < FACTORIES_SIZE; ++j) {  
      factories[i]->update(j, bag->nextBagTile());
    }
  }  

  centreFactory->setToken(true);
}

void GameEngine::resetGame() {

  bag->clear();
  players->clear();
  centreFactory->clear();
  playing = false;
  
  for (int i = 0; i < factoryLength; ++i) {
    factories[i]->clear();
  }
  
}

bool GameEngine::validateTurnInput(std::vector<std::string> moves) {
  bool valid = false;
  bool validArgumentCount = moves.size() == 4;

  if (validArgumentCount) {
    try {
      int factory = std::stoi(moves.at(1));
      int row = std::stoi(moves.at(3));
      Colour colour = (Colour) moves.at(2)[0];
      
      bool validFactory = validateFactoryInput(factory, colour);
      bool validRow = validateRowInput(row);
      bool validColour = validateColourInput(colour);
      
      bool validPatternInput = players->at(active)->getMosaic()->validatePatternLineInput(colour, row - 1);
      bool validWallInput = players->at(active)->getMosaic()->validateWallColourInput(colour, row - 1);
      
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

bool GameEngine::validateDiscardInput(std::vector<std::string> moves) {
  bool valid = false;
  bool validArgumentCount = moves.size() == 3;
  
  if (validArgumentCount) {
    try {
      int factory = std::stoi(moves.at(1));
      Colour colour = (Colour) moves.at(2)[0];
      
      bool validFactory = validateFactoryInput(factory, colour);
      bool validColour = validateColourInput(colour);
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

bool GameEngine::validateFactoryInput(int input, Colour colour) {
  
  bool valid = true;
  bool validFactory = input >= 0 && input <= factoryLength;
  
  if (validFactory) {

    int index = input - 1;
    bool isCentreFactory = index == -1;
    bool isNormalFactory = index >= 0;
    bool factoryEmpty = false;
    bool containsColour = false;
  
    if (isCentreFactory) {
      factoryEmpty = centreFactory->isEmpty();
      containsColour = centreFactory->count(colour) < 1;
    } else if (isNormalFactory) {
      factoryEmpty = factories[index]->isEmpty();
      containsColour = factories[index]->count(colour) < 1;
    }

    if (factoryEmpty) {
      printer->error("Error: " + std::string(1, (char) colour) + " is an empty factory");
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

bool GameEngine::validateColourInput(Colour input) { 

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

bool GameEngine::validateRowInput(int input) {

  bool valid = true;
  bool validRow = input > 0 && input <= ROWS;

  if(!validRow) {
    printer->error("Error: " + std::to_string(input) + " is not a valid row");
    valid = false;
  }

  return valid;
}

void GameEngine::display() {
  
  std::string name = players->at(active)->getName();
  
  printer->log("TURN FOR PLAYER: " + name);
  std::cout << std::endl;
  
  printFactories();

  std::cout << std::endl;

  int rowLimit = 2;
  bool stacked = (double) seats / (double) rowLimit > 1;
  int iterations = stacked ? 2 : 1;

  for (int count = 0; count < iterations; ++count) {
    
    std::cout << std::endl;
    std::cout << std::endl;

    rowLimit = seats == 3 && count == 1 ? 1 : rowLimit;

    // Pretty print player names
    for (int j = 0; j < rowLimit; ++j) {
      
      int modifier = seats == 3 && count == 1 ? 1 : 0;
      int index = (j + (rowLimit * count)) + modifier;
      std::string name = players->at(index)->getName();
      std::string activeOutput = BG_BLACK + C_WHITE + "Mosaic for " + name + C_RESET;
      std::string normalOutput = "Mosaic for " + name;
      std::string output = index == active ? activeOutput : normalOutput;
      std::cout << output;
      
      int spacing = 36 - output.size();

      if (index == active) {
        spacing += 16;
      }

      for (int l = 0; l < spacing; ++l) { 
        std::cout << " "; 
      }
    }

    std::cout << std::endl;
    std::cout << std::endl;

    // Pretty print player mosaic and pattern lines
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < rowLimit; ++j) {
        int modifier = seats == 3 && count == 1 ? -1 : 0;
        int index = j + (rowLimit * count) + modifier;

        Player* player = players->at(index);
        PatternLine* pattern = player->getMosaic()->getPattern();
        WallManager* wall = player->getMosaic()->getWall();
        
        std::cout << std::setw(j * 6) << i + 1 << ": ";
        pattern->printPattern(i);
        std::cout << "|| ";
        wall->printWall(i);
      }
      std::cout << std::endl;
    }

    // Pretty print discard lines
    std::cout << std::endl;

    for (int i = 0; i < rowLimit; ++i) {
      int index = i + (rowLimit * count);
      int spacing = index % 2 == 0 ? 22 : 0;

      Player* player = players->at(index);
      
      player->getMosaic()->getDiscard()->printDiscard();
      std::cout << std::setw(spacing);
    }
  }

  std::cout << std::endl;
}

void GameEngine::setupPlayers() {

  for (int i = 0; i < seats; ++i) {
    std::string name = "";
    int id = i + 1;
    
    printer->clear();
    std::cout << "Enter a name for player " << id << std::endl;
    name = printer->inputString();

    addPlayer(id, name, 0, false);
  }

}

void GameEngine::setupPlayerCount() {

  bool processing = true;

  int min = 2;
  int max = 4;
  int count = -1;

  while (processing && !std::cin.eof()) {
    
    printer->clear();
    std::cout << "Please enter the number of players (2-4).." << std::endl;
    std::string value = printer->inputString();

    try {

      count = std::stoi(value);

      if (count < min) {
        printer->error("Error: Player count [" + std::to_string(count) + "] entered, cannot be bellow " + std::to_string(min) + ".");
      } else if (count > max) {
        printer->error("Error: Player count [" + std::to_string(count) + "] entered, cannot exceed " + std::to_string(max) + ".");
      } else {
        processing = false;
        setSeats(count);
      }
    } catch(std::invalid_argument &e) {
      printer->error("Error: Value [" + value + "] is not a valid integer.");
    }
    
  }

}

void GameEngine::setupFactoryCount() {

  for (int i = 0; i < factoryLength; ++i) {
    delete factories[i];
  }

  delete [] factories;

  if (seats == 3) {
    factoryLength = 7;
  } else if (seats == 4) {
    factoryLength = 9;
  }

  factories = new Factory*[factoryLength];

  for (int i = 0; i < factoryLength; ++i) {
    factories[i] = new Factory();
  }

}

void GameEngine::printFactories() {
  printer->log("Factories:");
  printer->log(centreFactory->toString());

  for (int i = 0; i < factoryLength; ++i) {
    printer->log(std::to_string(i + 1) + ": " + factories[i]->toString());
  }
}

Saver* GameEngine::getSaver() {
  return saver;
}
