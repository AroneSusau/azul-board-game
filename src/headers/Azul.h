#ifndef AZUL_H
#define AZUL_H

#include <iostream>
#include <vector>

#include "Player.h"
#include "GameEngine.h"
#include "Saver.h"
#include "Loader.h"
#include "Printer.h"
#include "MenuOptions.h"
#include "Saver.h"
#include "Loader.h"

class Azul {
  
  public:
    Azul(int argc, char** argv);
    ~Azul();
    
    // Runs the azul program
    void run();

    // Displays a menu with game related options
    void menu();

    // Starts a new game of azul
    void startGame();

    // Allow the player to load a game from a formatted file on disk
    void loadGame();
    
    // Display information about program's authors
    void credits();

    // Returns a reference to the game engine object
    GameEngine* getGameEngine();

    // Returns a reference to the printer object
    Printer* getPrinter();

  private:
    Loader* loader;
    GameEngine* gameEngine;
    Printer* printer;
  
};

#endif
