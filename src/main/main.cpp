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
  
  std::cout << std::endl;

  std::cout << "      db    8888P 8    8 8    " << std::endl;
  std::cout << "    dPYb     dP  8    8 8    " << std::endl;
  std::cout << "  dPwwYb   dP   8b..d8 8    " << std::endl;
  std::cout << "dP    Yb d8888 `Y88P' 8888" << std::endl;

  std::cout << std::endl;

  Azul* azul = new Azul(argc, argv);
  
  azul->run();
  
  delete azul;
  
  return EXIT_SUCCESS;

}
