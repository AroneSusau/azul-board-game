#include "../headers/Azul.h"

Azul::Azul(int argc, char** argv): 
  loader(new Loader()),
  gameEngine(new GameEngine()),
  printer(new Printer())
{

  int seed = 0;

  if (argc == 3 && (std::string) argv[1] == "-s") {
    try {
      seed = std::stoi(argv[2]);
    } catch (std::runtime_error &e) {
      std::cout << "Error: Invalid seed argument passed in, using seed 0 instead." << std::endl;
    }
  } else {
    std::cout << "Info: No seed argument detect, using seed 0 instead." << std::endl;
  }

  gameEngine->getBag()->setSeed(seed);

}

Azul::~Azul() {}

void Azul::run() {
  menu();
}

void Azul::startGame() {
  gameEngine->run(false);
}

void Azul::loadGame() {
  
  std::cout << "Enter the filename from which load a game" << std::endl;
  std::cout << "> ";
  std::string filename;
  std::cin >> filename;
  std::cin.clear();
  std::cin.ignore(10000,'\n');

  bool loaded = !loader->load(filename, gameEngine);
  
  if (loaded) {
    gameEngine->run(loaded);
  }
}

void Azul::credits() {
  
  printer->credits();

}

void Azul::menu() {
  
  char command = DEFAULT;
  bool running = true;

  printer->menu();

  while (running && !std::cin.eof()) {

    command = printer->inputChar();
    
  	if (command == START) {
    	
      startGame();
      
  	} else if (command == LOAD) {
    	
      loadGame();
      
  	} else if (command == CREDITS) {
    	
      credits();
      
  	} else if (command == EXIT || std::cin.eof()) {
      
      running = false;
      
    } else {
      
      printer->error();
      
    }
  }

  printer->exit();
}

GameEngine* Azul::getGameEngine() {
  return gameEngine; 
}

Printer* Azul::getPrinter() {
  return printer; 
}