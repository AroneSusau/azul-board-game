#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include <iostream>
#include <tuple>
#include <iomanip>

#include "BaseEngine.h"
#include "LinkedList.h"
#include "GameOptions.h"
#include "Saver.h"

class GameEngine : public BaseEngine {
  public:
    GameEngine();
    GameEngine(int seats);
    virtual ~GameEngine();

    // Runs the game of Azul
    virtual void run(bool isLoadedGame);

    // Resets the game
    virtual void resetGame();

    // Returns the object used for saving game state
    Saver* getSaver();

  private:
  
    Saver* saver;

    // Setup for inital game state
    void setupGame();

    // Setup for inital player info
    void setupPlayers();

    // Setup number of players in game
    void setupPlayerCount();

    // Setup number of factories in game
    void setupFactoryCount();

    // setup of a new round, fill factories, first player token back to centre
    void setupRound();

    // Configures the starting player based on first player token or id
    void setupStartingPlayer();

    // Enter game menu, awaiting player input
    void playMenu();

    // Continues a loaded game
    void continueGame();

    // Moves turn to the next player
    void nextPlayer();

    // play a turn, returns true if turn was successful
    bool turn(std::vector<std::string> moves);

    // move tiles to discard, returns true if turn was successful
    bool discardTurn(std::vector<std::string> moves);

    // fills factories with tiles from the bag
    void fillFactories();

    // moves factories tiles and returns number of tiles to move and a check for first player token
    std::tuple<int, bool> factoryMovement(bool isCentreFactory, int factory, Colour colour);

    // end of round updates, includes tile movements to mosaic, point scoring etc
    void endRound();

    // updates the score for each player
    void scoreRound();

    // updates the score for each player with the special end game scoring conditions
    void endGameScoring();

    // calculates the winning player and outputs the result
    void calculateWinner();

    // returns true if all factories are empty
    bool factoriesEmpty();

    // checks if the game has met the end condition
    bool checkEndCondition();

    // validates input for each turn
    bool validateTurnInput(std::vector<std::string> moves);

    // validates input for discard turn
    bool validateDiscardInput(std::vector<std::string> moves);

    // validates factory input for turn command
    bool validateFactoryInput(int input, Colour colour);

    // validates colour input for turn command
    bool validateColourInput(Colour input);

    // validates row input for turn command
    bool validateRowInput(int input);

    // display factory and mosaic information for the player
    void display();

    // Print factories tiles data
    void printFactories();
    
};

#endif
