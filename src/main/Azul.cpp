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
      printer->pause();
    }
  } else {
    std::cout << "Info: No seed argument detect, using seed 0 instead." << std::endl;
    printer->pause();
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
  
  printer->clear();
  std::cout << "Enter the filename from which load a game" << std::endl;
  printer->cursor();
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
  printer->pause();

}

void Azul::menu() {
  
  char command = DEFAULT;
  bool running = true;

  while (running && !std::cin.eof()) {

    printer->clear();
    printer->title();
    printer->menu();

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
      printer->pause();
      
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