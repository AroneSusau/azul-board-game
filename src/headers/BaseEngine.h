#ifndef BASE_ENGINE_H
#define BASE_ENGINE_H

#include <vector>

#include "Printer.h"
#include "Player.h"
#include "Bag.h"
#include "Factory.h"
#include "CentreFactory.h"
#include "Types.h"
#include "MenuOptions.h"

/**
 *
 * Base implementation for GameEngine classes
 *  
 **/
class BaseEngine {

  public:
    BaseEngine();
    BaseEngine(int seats);
    virtual ~BaseEngine();

    int getSeats();
    void setSeats(int seats);
    
    int getTurns();
    void setTurns(int turn);

    int getActivePlayer();
    void setActivePlayer(int active);

    Bag* getBag();

    std::vector<Player*>* getPlayers();
    void addPlayer(int id, std::string name, int score, bool starter);
    void addPlayer(Player* player);
    void removePlayer(int index); 
    void clearPlayers();

    Factory* getFactory(int index);
    CentreFactory* getCentreFactory();
    Player* getPlayer(int index);

    virtual void run(bool isLoadedGame);
    virtual void resetGame();

  protected:
    int seats;
    int turns;
    int active;
    bool playing;
    Printer* printer;
    Bag* bag;

    std::vector<Player*>* players;
    CentreFactory* centreFactory;
    Factory** factories;

};

#endif