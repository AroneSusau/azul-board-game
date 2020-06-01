#include "../headers/Loader.h"

Loader::Loader():
  error(false),
  printer(new Printer())
{}

Loader::~Loader() {
  delete printer;
}

bool Loader::load(std::string filename, GameEngine* gameEngine) {

  error = false;
  std::ifstream file(filename);
  std::string line;

  gameEngine->clearPlayers();
  gameEngine->setFactoryLength(0);

  if (validateFile(&file, filename)) {
    
    while (std::getline(file, line) && !error) {
      if (line == "$" && !detectComments(line)) {
        
        SaveDataArgs args = parseData(&file, line);
        
        if (validateTypeField(args)) {
          parseTypes(args, gameEngine);
        }
      }
    }

    file.close();
  } 

  if (error) {
    gameEngine->resetGame();
  } else {
    std::cout << C_GREEN << "Azul game successfully loaded" << C_RESET << std::endl;
    printer->pause();
  }

  return error;
}

SaveDataArgs Loader::parseData(std::ifstream* file, std::string line) {
  std::string word = "";
  bool parsing = true;

  SaveDataArgs args;
  std::vector<std::string> data;

  while (!detectComments(line) && getline(*file, line) && parsing && !error) {
    for (int i = 0; i < (int) line.size(); i++) {
      if(line == "#") {

        parsing = false;

      } else if (line[i] == ':') {
        
        data.push_back(word);
        word = "";

      } else if (i == (int)line.size() - 1) {
        
        word += line[i];
        data.push_back(word);
        args.push_back(data);
        data.clear();
        word = "";

      } else { word += line[i]; }
    }
  }

  return args;
}

void Loader::parseTypes(SaveDataArgs args, GameEngine* gameEngine) {
  
  std::string type = args[0][1];

  if (type == "player") {
    if (validateFields(args, type, PLAYER_FIELDS_LENGTH)) {
      parsePlayer(args, gameEngine);
    }
  } else if (type == "factory") {
    if (validateFields(args, type, (int) args.size())) {
      parseFactory(args, gameEngine);
    }
  } else if (type == "game") {
    if (validateFields(args, type, GAME_FIELDS_LENGTH)) {
      parseGame(args, gameEngine);
    }
  } else {
    error = true;
    printer->error("Error: Unexpected type '" + type + "' detected.");
  }
}

void Loader::parsePlayer(SaveDataArgs data, GameEngine* gameEngine) {

  int id = 0;
  std::string name = "";
  int points = 0;
  bool starter = false;
  bool isHuman = false;
  Player* player = new Player();

  for (int i = 0; i < (int) data.size(); i++) {

    std::string field = data[i][0];
    std::string value = data[i][1];

    try {
      if (field == "id") {
        id = std::stoi(value);
        player->setId(id);
      } else if (field == "name") {
        name = value;
        player->setName(name);
      } else if (field == "points") {
        points = std::stoi(value);
        player->setScore(points);
      } else if (field == "mosaic") {
        
        int row = 0;
        int col = 0;

        for (int j = 0; j < (int) value.size(); ++j) {
          if (value[j] != ',') {
            bool result = (int) value[j] != EMPTY;
            player->getMosaic()->getWall()->getWall()->set(row, col, new bool(result));
            ++col;
          } else {
            col = 0;
            ++row;
          }
        }

      } else if (field == "pile") {
        
        int row = 0;
        int col = 0;

        for (int j = 0; j < (int) value.size(); ++j) {
          if (value[j] != ',') {
            Colour result = (Colour) value[j];
            
            if (result != FIRST_PLAYER && result != EMPTY) {
              player->getMosaic()->getPattern()->add(result, 1, row);
            }

            ++col;
          } else {
            ++row;
          }
        }

      } else if (field == "broken") {
        for (int j = 0; j < (int) value.size(); ++j) {
          Colour result = (Colour) value[j];
          
          if (result != FIRST_PLAYER && result != EMPTY) {
            player->getMosaic()->getDiscard()->add((Colour) result);
          }
        }
      } else if (field == "isHuman") {
        isHuman = std::stoi(value) == 1;
        player->setIsHuman(isHuman);
      } else if (field == "starter") {
        starter = std::stoi(value) == 1;
        player->setStarter(starter);

        if (starter) {
          player->getMosaic()->getPattern();
        }
      }
    } catch (std::exception &e) {
      error = true;
      printer->error("Error: Player parsing error on field [" + field + "], with value [" + value + "].");
    }
  }

  if (!error) {
    gameEngine->addPlayer(player);
  }
}

void Loader::parseFactory(SaveDataArgs data, GameEngine* gameEngine) {

  for (int i = 0; i < (int) data.size(); i++) {

    std::string field = data[i][0];
    std::string value = data[i][1];

    try {
      if (field[0] == 'f' && field.size() == 2) {
        for (int j= 0; j < 9; ++j) {
          if (field == "f" + std::to_string(j)) {
            
            Factory* factory = new Factory();

            for (int k = 0; k < (int) value.size(); ++k) {
              Colour colour = (Colour) value[k]; 
              factory->update(k, colour);
            }

            gameEngine->addFactory(factory);
          }
        }
      } else if (field == "mid0" || field == "mid1") {
        
        CentreFactory* centreFactory = new CentreFactory();
        int index = std::stoi(std::string(1, field[3]));
        centreFactory->setToken(true);
        
        for (int j = 0; j < (int) value.size(); ++j) {
          if (value[0] != '=') {
            centreFactory->add((Colour) value[j]);
          }
        }

        for (int i = 0; i < (int) gameEngine->getPlayers()->size(); ++i) {
          if (gameEngine->getPlayer(i)->getStarter()) {
            centreFactory->setToken(false);
          }
        }

        if (gameEngine->getCentreFactories()[index] != nullptr) {
          delete gameEngine->getCentreFactories()[index];
        }

        gameEngine->getCentreFactories()[index] = centreFactory;

      } else if (field == "centreFactoryLength") {
        gameEngine->setCentreFactoryLength(std::stoi(value));
      }
    } catch (std::exception &e) {
      error = true;
      printer->error("Error: Factory parsing error on field [" + field + "], with value [" + value + "].");
    }
  }
}

void Loader::parseGame(SaveDataArgs data, GameEngine* gameEngine) {
  for (int i = 0; i < (int) data.size(); i++) {

    std::string field = data[i][0];
    std::string value = data[i][1];

    try {
      if (field == "turns") {
        gameEngine->setTurns(std::stoi(value));
      } else if (field == "active") {
        gameEngine->setActivePlayer(std::stoi(value));
      } else if (field == "seats") {
        gameEngine->setSeats(std::stoi(value));
      } else if (field == "seed") {
        gameEngine->getBag()->setSeed(std::stoi(value));
      } else if (field == "bag") {
        for (int j = 0; j < (int) value.size(); ++j) {
          gameEngine->getBag()->addTile((Colour) value[j]);
        }
      } else if (field == "lid") {
        if (value[0] != '=') {
          for (int j = 0; j < (int) value.size(); ++j) {
            gameEngine->getBag()->addLidTile((Colour) value[j]);
          }
        }
      }
    } catch (std::exception &e) {
      error = true;
      printer->error("Error: Game parsing error on field [" + field + "], with value [" + value + "].");
    }
  }
}

bool Loader::detectComments(std::string line) {
  return line.length() >= 2 &&  line[0] == '/' &&  line[1] == '/';
}

bool Loader::validateTypeField(SaveDataArgs args) {

  bool typeFieldExists = (int) args.size() > 0 && args[0][0] == "type";
  bool typeCorrectSize = (int) args[0].size() > 1;

  if (!typeFieldExists) {
    typeFieldExists = false;
    error = true;
    printer->error("Error: Invalid save file format, type field has no value.");
  } else if (typeFieldExists && !typeCorrectSize) {
    typeFieldExists = false;
    error = true;
    printer->error("Error: Invalid save file format, type field arguments missing.");
  }

  return typeFieldExists;
}

bool Loader::validateFile(std::ifstream* file, std::string filename) {

  bool result = false;

  if (file->good()) {
    result = true;
  } else {
    error = true;
    printer->error("Error: Save file " + filename + " does not exist.");
  }

  return result;
}

bool Loader::validateFields(SaveDataArgs args, std::string type, int length) {
  int result = true;

  std::string playerFields [PLAYER_FIELDS_LENGTH] = {"type", "id", "name", "points", "mosaic", "pile", "broken", "isHuman", "starter"};
  std::string factoryFields [FACTORY_FIELDS_LENGTH] = {"type", "centreFactoryLength", "factoryLength", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "mid0", "mid1"};
  std::string gameFields [GAME_FIELDS_LENGTH] = {"type", "turns", "active", "seats", "seed", "bag", "lid"}; 

  int arrayLength = 0;
  std::string* array;

  for (int i = 0; i < length; ++i) {
    
    if (type == "player") {
      arrayLength = PLAYER_FIELDS_LENGTH;
      array = playerFields;
    } else if (type == "factory") {
      arrayLength = FACTORY_FIELDS_LENGTH;
      array = factoryFields;
    } else if (type == "game") {
      arrayLength = GAME_FIELDS_LENGTH;
      array = gameFields;
    }

    try {
      std::string field = args[i][0];
      bool match = false;

      for (int j = 0; j < arrayLength; ++j) {
        if (field == array[j]) {
          match = true;
          j = arrayLength;
        }
      }

      if (!match) {
        error = true;
        result = false;
        i = length;
        printer->error("Error: Parsing error on " + type + " object, unexpected field [" + field + "] does not exist.");
      }
    } catch (std::exception &e) {
        error = true;
        result = false;
        printer->error("Error: Issue parsing " + type + " object field.");
        i = length;
    }
  }

  return result;
}
