#include "../headers/Saver.h"

Saver::Saver(): printer(new Printer()) {}

Saver::~Saver() {
  delete printer;
}

void Saver::save(std::string name, BaseEngine* gameEngine) {
  try {

    if (name.size() > 0) {
      std::string filename(name);
      std::ofstream file;

      file.open(filename);

      savePlayer(&file, gameEngine);
      saveFactory(&file, gameEngine);
      saveGame(&file, gameEngine);

      file.close();

      std::cout << C_GREEN << "Game successfully saved to '" <<  filename << "'" << C_RESET << std::endl;
      printer->pause();
    } else {
      printer->error("Error: Filename cannot be empty.");
    }
  } catch (std::runtime_error &e) {
    printer->error("Error: Saving error occured, please try again.");
  } 
}

void Saver::savePlayer(std::ofstream* file, BaseEngine* gameEngine) {
  
  for (int i = 0; i < gameEngine->getSeats(); ++i) {

    Player* player = gameEngine->getPlayer(i);

    *file << std::endl;
    *file << "$" << std::endl;

    *file << "type:player" << std::endl;
    *file << "id:" << player->getId() << std::endl;
    *file << "name:" << player->getName() << std::endl;
    *file << "points:" << player->getScore() << std::endl;
    *file << "mosaic:" << player->getMosaic()->getWall()->toSaveString() << std::endl;
    *file << "pile:" << player->getMosaic()->getPattern()->toSaveString() << std::endl;
    *file << "broken:" << player->getMosaic()->getDiscard()->toSaveString() << std::endl;
    *file << "starter:" << player->getStarter() << std::endl;

    *file << "#" << std::endl;

  }

}

void Saver::saveFactory(std::ofstream* file, BaseEngine* gameEngine) {
  *file << std::endl;
  *file << "$" << std::endl;

  *file << "type:factory" << std::endl;

  *file << "centreFactoryLength:" << gameEngine->getCentreFactoryLength() << std::endl;

  for (int i = 0; i < gameEngine->getFactoryLength(); ++i) {
    *file << "f" << i + 1 << ":" << gameEngine->getFactory(i)->toSaveString() << std::endl;
  }

  for (int i = 0; i < gameEngine->getCentreFactoryLength(); ++i) {
    *file << "mid" << i << ":" << gameEngine->getCentreFactory(i)->toSaveString() << std::endl;
  }
  
  *file << "#" << std::endl;
}

void Saver::saveGame(std::ofstream* file, BaseEngine* gameEngine) {
  *file << std::endl;
  *file << "$" << std::endl;

  *file << "type:game" << std::endl;
  *file << "turns:" << gameEngine->getTurns() << std::endl;
  *file << "active:" << gameEngine->getActivePlayer() << std::endl;
  *file << "seats:" << gameEngine->getSeats() << std::endl;
  *file << "seed:" << gameEngine->getBag()->getSeed() << std::endl;
  *file << "bag:" << gameEngine->getBag()->toBagSaveString() << std::endl;
  *file << "lid:" << gameEngine->getBag()->toLidSaveString() << std::endl;
  
  *file << "#" << std::endl;
}
