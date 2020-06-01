#include "../headers/BaseEngine.h"

BaseEngine::BaseEngine() :
  seats(2),
  turns(0),
  centreFactoryLength(2),
  factoryLength(5),
  aiLength(0),
  active(0),
  playing(false),
  printer(new Printer()),
  bag(new Bag()),
  aiPlayer(new AIPlayer()),
  players(new std::vector<Player*>()),
  centreFactories(new CentreFactory*[CENTRE_FACTORY_LENGTH]),
  factories(new Factory*[factoryLength]())
{
  for (int i = 0; i < factoryLength; ++i) {
    factories[i] = new Factory();
  }
}

BaseEngine::BaseEngine(int seats) :
  seats(seats),
  turns(0),
  centreFactoryLength(2),
  factoryLength(5),
  aiLength(0),
  active(0),
  playing(false),
  printer(new Printer()),
  bag(new Bag()),
  aiPlayer(new AIPlayer()),
  players(new std::vector<Player*>()),
  centreFactories(new CentreFactory*[CENTRE_FACTORY_LENGTH]),
  factories(new Factory*[factoryLength]())
{
  for (int i = 0; i < factoryLength; ++i) {
    factories[i] = new Factory();
  }
}

BaseEngine::~BaseEngine() {

  for (int i = 0; i < (int) players->size(); ++i) {
    delete players->at(i);
    players->erase(players->begin() + i);
  }

  for (int i = 0; i < factoryLength; ++i) {
    delete factories[i];
  }

  delete printer;
  delete bag;
  delete players;
  delete aiPlayer;
  delete [] centreFactories;
  delete [] factories;
}

void BaseEngine::run(bool isLoadedGame) {}

void BaseEngine::resetGame() {}

void BaseEngine::addPlayer(Player* player) {
  players->push_back(player);
}

void BaseEngine::addPlayer(int id, std::string name, int score, bool starter, bool isHuman) {
  players->push_back(new Player(id, name, score, starter, isHuman));
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
  
  if (index >= 0 && index < factoryLength) {
    result = factories[index];
  }

  return result;
}

void BaseEngine::addFactory(Factory* factory) {
  
  const int newLength = factoryLength + 1;
  Factory** tmp = new Factory*[newLength];

  for (int i = 0; i < factoryLength; ++i) {
    tmp[i] = factories[i];
  }

  tmp[factoryLength] = factory;
  factoryLength++;

  delete [] factories;
  factories = tmp;

}

CentreFactory** BaseEngine::getCentreFactories() {
  return centreFactories;
}

CentreFactory* BaseEngine::getCentreFactory(int index) {
  
  CentreFactory* factory = nullptr;

  if (index >= 0 && index < centreFactoryLength) {
    factory = centreFactories[index];
  } else {
    printer->error("Error: Attempting to access out of bounds Centre Factory.");
  }

  return factory;
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

int BaseEngine::getFactoryLength() {
  return factoryLength;
}

void BaseEngine::setFactoryLength(int length) {
  factoryLength = length;
}

int BaseEngine::getCentreFactoryLength() {
  return centreFactoryLength;
}

void BaseEngine::setCentreFactoryLength(int length) {
  centreFactoryLength = length;
}
