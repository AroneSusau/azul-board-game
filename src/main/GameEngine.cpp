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
    printer->helpMenu();
  } else {
    playing = true;
    printer->helpMenu();
  }

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
  bag->shuffle();
  setupPlayers();
}

void GameEngine::playMenu() {

  bool exit = false;
  printer->clear();
  printer->startPlay();
  printer->helpMenu();
  printer->pause();
  printer->clear();

  while (!exit && !factoriesEmpty() && !std::cin.eof()) {

    bool turnInProgress = true;
    display();

    while (!exit && turnInProgress && !std::cin.eof()) {

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

      } else if (command == EXIT_GAME || std::cin.eof()) {

        playing = false;
        turnInProgress = false;
        exit = true;

        if (command == EXIT_GAME) {
          std::cout << "Returning to main menu.." << std::endl;
        }

      } else {

        printer->error();

      }
    }

    turns++;
    nextPlayer();

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
    std::cout << "Turn successful." << std::endl;
    std::cout << std::endl;

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
    std::cout << "Turn successful." << std::endl;
    std::cout << std::endl;
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
  std::cout << std::endl;
  std::cout << "=== Start Round ===" << std::endl;
  std::cout << std::endl;
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
    std::cout << "Returning to main menu.." << std::endl;
  }
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

  for(int i = 0; i < FACTORIES_NUM; i++) {
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
  
  for (int i = 0; i < FACTORIES_NUM; ++i) {
    
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
  
  for (int i = 0; i < FACTORIES_NUM; ++i) {
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
      std::cout << "Error: Invalid arguments passed in for discard command." << std::endl;
    } catch(std::invalid_argument &e) {
      std::cout << "Error: Bad character passed in for turn command argument." << std::endl;
    }
  } else {
    std::cout << "Error: Invalid argument count." << std::endl;
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
      std::cout << "Error: Invalid arguments passed in for discard command. Use 'help' for command usage." << std::endl;
    } catch(std::invalid_argument &e) {
      std::cout << "Error: Bad character passed in for row argument." << std::endl;
    }
  } else {
    std::cout << "Error: Invalid arguments passed in for discard command. Use 'help' for command usage." << std::endl;
  }

  return valid;
}

bool GameEngine::validateFactoryInput(int input, Colour colour) {
  
  bool valid = true;
  bool validFactory = input >= 0 && input <= FACTORIES_NUM;
  
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
      std::cout << "Error: " << input << " is an empty factory" << std::endl;
      valid = false;
    } else if (containsColour) {
      std::cout << "Error: Factory does not contain colour of type " << (char) colour << "." << std::endl;
      valid = false;
    }

  } else {
    std::cout << "Error: " << input << " is not a valid factory" << std::endl;
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
    std::cout << "Error: " << (char) input << " is not a valid colour tile" << std::endl;
  }

  return valid;
}

bool GameEngine::validateRowInput(int input) {

  bool valid = true;
  bool validRow = input > 0 && input <= ROWS;

  if(!validRow) {
    std::cout << "Error: " << input << " is not a valid row" << std::endl;
    valid = false;
  }

  return valid;
}

void GameEngine::display() {
  
  std::string name = players->at(active)->getName();
  
  std::cout << "TURN FOR PLAYER: " << name << std::endl;
  
  // print factory information
  printer->log("Factories:");

  std::cout << centreFactory->toString() << std::endl;

  for (int i = 0; i < FACTORIES_NUM; ++i) {
    std::cout << i + 1 << ": " << factories[i]->toString() << std::endl;
  }

  // print mosaic information
  std::cout << std::endl;
  players->at(active)->getMosaic()->printMosaic(name);

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

Saver* GameEngine::getSaver() {
  return saver;
}
