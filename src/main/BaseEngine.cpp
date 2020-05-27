#include "../headers/BaseEngine.h"

BaseEngine::BaseEngine() :
  seats(2),
  turns(0),
  active(0),
  playing(false),
  printer(new Printer()),
  bag(new Bag()),
  players(new std::vector<Player*>()),
  centreFactory(new CentreFactory()),
  factories(new Factory*[FACTORIES_NUM]())
{
  for (int i = 0; i < FACTORIES_NUM; ++i) {
    factories[i] = new Factory();
  }
}

BaseEngine::BaseEngine(int seats) :
  seats(seats),
  turns(0),
  active(0),
  playing(false),
  printer(new Printer()),
  bag(new Bag()),
  players(new std::vector<Player*>()),
  centreFactory(new CentreFactory()),
  factories(new Factory*[FACTORIES_NUM]())
{
  for (int i = 0; i < FACTORIES_NUM; ++i) {
    factories[i] = new Factory();
  }
}

BaseEngine::~BaseEngine() {

  for (int i = 0; i < (int) players->size(); ++i) {
    delete players->at(i);
    players->erase(players->begin() + i);
  }

  for (int i = 0; i < FACTORIES_NUM; ++i) {
    delete factories[i];
  }

  delete printer;
  delete bag;
  delete players;
  delete centreFactory;
  delete [] factories;
}

void BaseEngine::run(bool isLoadedGame) {}

void BaseEngine::resetGame() {}

void BaseEngine::addPlayer(Player* player) {
  players->push_back(player);
}

void BaseEngine::addPlayer(int id, std::string name, int score, bool starter) {
  players->push_back(new Player(id, name, score, starter));
}

void BaseEngine::removePlayer(int index) {
  if (index < (int)players->size() && players->at(index) != nullptr) {
    players->erase(players->begin() + index);
  }
}

void BaseEngine::clearPlayers() {
  for (int i = 0; i < (int) players->size(); ++i) {
    players->erase(players->begin() + i);
  }
}

int BaseEngine::getSeats() {
  return seats;
}

void BaseEngine::setSeats(int seats) {
  this->seats = seats;
}

int BaseEngine::getTurns() {
  return turns;
}

void BaseEngine::setTurns(int turn) {
  this->turns = turn;
}

int BaseEngine::getActivePlayer() {
  return active;
}

void BaseEngine::setActivePlayer(int active) {
  this->active = active;
}

Factory* BaseEngine::getFactory(int index) {
  Factory* result = nullptr;
  
  if (index >= 0 && index < FACTORIES_NUM) {
    result = factories[index];
  }

  return result;
}

CentreFactory* BaseEngine::getCentreFactory() {
  return centreFactory;
}

std::vector<Player*>* BaseEngine::getPlayers() {
  return players;
}

Player* BaseEngine::getPlayer(int index) {
  return players->at(index);
}

Bag* BaseEngine::getBag() {
  return bag;
}