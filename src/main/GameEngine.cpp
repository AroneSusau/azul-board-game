#include "../headers/GameEngine.h"

GameEngine::GameEngine() : 
  BaseEngine(), 
  saver(new Saver()), 
  validator(new Validator())
{}

GameEngine::GameEngine(int seats) : 
  BaseEngine(seats), 
  saver(new Saver()), 
  validator(new Validator())
{}

GameEngine::~GameEngine() {
  delete saver;
  delete validator;
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
  setupAiCount();
  setupFactoryCount();
  setupCentreFactoryCount();
  bag->shuffle();
  setupPlayers();
}

void GameEngine::playMenu() {

  bool exit = false;

  while (!exit && !factoriesEmpty() && !std::cin.eof()) {

    bool turnInProgress = true;

    while (!exit && turnInProgress && !std::cin.eof()) {

      display();
      std::vector<std::string> moves = evaluateMoveSet();
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
  
  bool validInput = validator->validateTurnInput(moves, this);
  bool successfulTurn = false;

  if (validInput) {
    
    // parse move values
    int factory = std::stoi(moves.at(1));
    int row = std::stoi(moves.at(3)) - 1;
    Colour colour = (Colour) moves.at(2)[0];

    bool oneCentre = centreFactoryLength == 1;
    bool isCentreFactory = oneCentre ? factory == 0 : factory == 0 || factory == 1;
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
  
  bool validInput = validator->validateDiscardInput(moves, this);
  bool successfulTurn = false;

  if (validInput) {

    // parse move values
    int factory = std::stoi(moves.at(1));
    Colour colour = (Colour) moves.at(2)[0];

    bool isCentreFactory = factory == 0 || factory == 1;
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

  if (isCentreFactory) {
      
      amount = centreFactories[factory]->count(colour);
      firstPlayer = centreFactories[factory]->getToken();
      
      if (firstPlayer) {
        
        for (int i = 0; i < centreFactoryLength; ++i) {
          centreFactories[i]->setToken(false);
        }

        players->at(active)->setStarter(true);
      }

      centreFactories[factory]->remove(colour);

    } else {
      
      factory -= centreFactoryLength;
      amount = factories[factory]->count(colour);
      factories[factory]->remove(colour);
      
      int centreFactoryIndex = players->at(active)->getIsHuman() ? 
          getCentreFactoryIndex() :
          aiPlayer->evaluateCentreFactoryIndex(active, this);
      
      for (int i = 0; i < FACTORIES_SIZE; i++) {
        
        Colour remaining = factories[factory]->get(i);
      
        if (remaining != EMPTY) {
          centreFactories[centreFactoryIndex]->add(remaining);
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

int GameEngine::getCentreFactoryIndex() {

  int result = -1;
  std::string input = "";
  bool prompting = true;

  std::string amount = centreFactoryLength == 1 ? "0" : "0 or 1";

  while (prompting && !std::cin.eof()) {

    std::cout << "Which centre pile would you like to select? [" << amount << "]" << std::endl;
    input = printer->inputString();

    try {
      result = std::stoi(input);

      if (result < 0 || (result > 1 && centreFactoryLength == 2) || (result == 1 && centreFactoryLength == 1)) {
        printer->error("Error: Invalid centre facrtory value. Please enter " + amount + ".");  
      } else {
        prompting = false;
      }
    } catch (std::invalid_argument &e) {
      printer->error("Error: Invalid character type entered. Please enter " + amount + ".");
    }
  }

  return result;
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

    int broken = player->getMosaic()->getDiscard()->getMinusPoints();
    player->setScore(player->getScore() - broken);

    // Print player points
    std::cout << player->getName() << ":";
    int space = 15 - player->getName().size();

    for (int i = 0; i < space; ++i) {
      std::cout << " ";
    }

    std::cout << player->getScore() << std::endl;

    player->getMosaic()->getDiscard()->clear();
  }

  for (int i = 0; i < (int) players->size(); ++i) {
    if (players->at(i)->getStarter()) {
      std::cout << std::endl;
      std::cout << BG_WHITE << C_BLACK << "Player " << players->at(i)->getName() << " is starting the next round.." << C_RESET << std::endl;
    }
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
  
  std::cout << std::endl;
  std::cout << "=== END OF GAME - FINAL SCORES ===" << std::endl;

  std::string winner = "";
  int winningPoints = 0;

  for (int i = 0; i < (int) players->size(); i++) {
    
    std::string name = players->at(i)->getName();
    int score = players->at(i)->getScore();

    std::cout << name << ":";
    int space = 15 - name.size();

    for (int i = 0; i < space; ++i) {
      std::cout << " ";
    }

    std::cout << score << std::endl;
    
    if (score > winningPoints) {
      winner = name;
      winningPoints = score;
    }
  }
  
  std::cout << std::endl;
  std::cout << C_GREEN << "Player " << winner << " wins!! " << C_RESET << std::endl;

}

bool GameEngine::factoriesEmpty() {
  bool empty = true;

  for(int i = 0; i < factoryLength; i++) {
    if(!factories[i]->isEmpty()) {
      empty = false;
    }
  }

  for (int i = 0; i < centreFactoryLength; ++i) {
    if (centreFactories[i] == nullptr) {
      std::cout << "NULL NULL " << i << std::endl;
    }
    
    if(!centreFactories[i]->isEmpty()) {
      empty = false;
    }
  }

  return empty;
}

void GameEngine::fillFactories() {
  
  for (int i = 0; i < factoryLength; ++i) {
    for (int j = 0; j < FACTORIES_SIZE; ++j) {  
      factories[i]->update(j, bag->nextBagTile());
    }
  }  

  for (int i = 0; i < centreFactoryLength; ++i) {
    centreFactories[i]->setToken(true);
  }
}

std::vector<std::string> GameEngine::evaluateMoveSet() {

  bool activePlayerHuman = players->at(active)->getIsHuman();
  std::vector<std::string> input;

  if (activePlayerHuman) {
    input = printer->inputArgs();
  } else {
    input = aiPlayer->turn(active, this);
  }

  return input;
}

void GameEngine::resetGame() {

  bag->clear();
  players->clear();
  playing = false;
  
  for (int i = 0; i < factoryLength; ++i) {
    if (factories[i] != nullptr) {
      factories[i]->clear();
    }
  }

  for (int i = 0; i < centreFactoryLength; ++i) {
    if (centreFactories[i] != nullptr) {
      centreFactories[i]->clear();
    }
  }
  
}

void GameEngine::setupPlayers() {

  int humans = seats - aiLength;
  const int length = 13;
  std::string belivableHumanNames[length] = {
    "Shirley",
    "Patricia",
    "Barbara",
    "Betty",
    "Helen",
    "George",
    "James",
    "Donald",
    "John",
    "William",
    "Gladys",
    "Imaani",
    "Melton"
  };

  // Setup humans
  for (int i = 0; i < humans; ++i) {
    std::string name = "";
    std::string backup = belivableHumanNames[std::rand() % length];
    int id = i + 1;
    
    printer->clear();
    std::cout << "Enter a name for player " << id << std::endl;
    name = printer->inputString();

    if (name.size() == 0) {
      name = backup;
    }

    addPlayer(id, name, 0, false, true);
  }

  // Setup evil machines D:
  std::shuffle(
    std::begin(belivableHumanNames), 
    std::end(belivableHumanNames), 
    std::default_random_engine(std::time(nullptr)));

  for (int i = 0; i < aiLength; ++i) {
    std::string name = belivableHumanNames[i] + "_AI";
    int id = humans + i + 1;

    addPlayer(id, name, 0, false, false);
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

void GameEngine::setupAiCount() {

  bool processing = true;

  int min = 1;
  int max = seats;//- 1;
  int count = -1;

  while (processing && !std::cin.eof()) {
    
    printer->clear();
    std::cout << "Please enter the number of AI you want to play against (" << min << " to " << max << ").." << std::endl;
    std::string value = printer->inputString();

    try {

      count = std::stoi(value);

      if (count < min) {
        printer->error("Error: Player count [" + std::to_string(count) + "] entered, cannot be bellow " + std::to_string(min) + ".");
      } else if (count > max) {
        printer->error("Error: Player count [" + std::to_string(count) + "] entered, cannot exceed " + std::to_string(max) + ".");
      } else {
        processing = false;
        aiLength = count;
      }
    } catch(std::invalid_argument &e) {
      printer->error("Error: Value [" + value + "] is not a valid integer.");
    }
    
  }

}

void GameEngine::setupCentreFactoryCount() {

  int result = -1;
  std::string input = "";
  bool prompting = true;

  for (int i = 0; i < centreFactoryLength; ++i) {
    if (centreFactories[i] != nullptr) {
      delete centreFactories[i];
    }
  }

  if (centreFactories != nullptr) {
    delete [] centreFactories;
  }

  while (prompting && !std::cin.eof()) {

    printer->clear();
    std::cout << "Please enter the number of centre factrories you would like? 1-2" << std::endl;
    input = printer->inputString();

    try {

      result = std::stoi(input);
      
      if (result < 1 || result > 2) {
        printer->error("Error: Invalid centre facrtory value. Please enter 1 or 2.");  
      } else {
        prompting = false;
      }
    } catch (std::invalid_argument &e) {
      printer->error("Error: Invalid centre facrtory value. Please enter 1 or 2.");  
    }
  }

  centreFactoryLength = result;
  centreFactories = new CentreFactory*[centreFactoryLength];

  for (int i = 0; i < centreFactoryLength; ++i) {
    centreFactories[i] = new CentreFactory();
  }

  printer->clear();
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
  for (int i = 0; i < centreFactoryLength; ++i) {
    printer->log("Centre " + std::to_string(i) + centreFactories[i]->toString());
  }

  std::cout << std::endl;

  for (int i = 0; i < factoryLength; ++i) {
    int id = i + centreFactoryLength;
    std::string space = id >= 10 ? " " : "  ";

    printer->log(std::to_string(id) + ":" + space + factories[i]->toString());
  }
}

Saver* GameEngine::getSaver() {
  return saver;
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
      std::string activeOutput = BG_GREEN + C_WHITE + "Mosaic for " + name + C_RESET;
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
