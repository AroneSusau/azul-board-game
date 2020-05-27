#include "../headers/Player.h"

Player::Player() :
  id(0),
  name("defult"),
  score(0),
  starter(0),
  mosaic(new Mosaic())
{}

Player::Player(int id, std::string name, int score, bool starter) :
  id(id),
  name(name),
  score(score),
  starter(starter),
  mosaic(new Mosaic())
{}

Player::~Player() {
  delete mosaic;
}

void Player::setId(int id) {
  this->id = id;
}

int Player::getId() {
  return id;
}

void Player::setName(std::string name) {
  this->name = name;
}

std::string Player::getName() {
  return name;
}

void Player::setScore(int score) {
  this->score = score;
}

void Player::addScore(int points) {
  this->score += points;
}

int Player::getScore() {
  return score;
}

Mosaic* Player::getMosaic() {
  return mosaic;
}

bool Player::getStarter() {
  return starter;
}

void Player::setStarter(bool starter) {
  this->starter = starter;
}
