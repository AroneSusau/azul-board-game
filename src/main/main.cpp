#include "../headers/Azul.h"

#define EXIT_SUCCESS 0

/**
 * 
 * COSC1070 Assignment 2 - Milestone 1: Group Component. 
 * 
 * Please refer to the project README.md file 
 * for instructions on how to run the application.
 * 
 **/
int main(int argc, char** argv) {

  Azul* azul = new Azul(argc, argv);
  
  azul->run();
  
  delete azul;
  
  return EXIT_SUCCESS;

}
