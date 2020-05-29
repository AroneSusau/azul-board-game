#ifndef FONTS_H
#define FONTS_H

#include <string>

#define C_RESET std::string("\u001b[0m")
#define C_DARK std::string("\u001b[2m")

#define C_BLACK std::string("\u001b[30m")
#define C_RED std::string("\u001b[31m")
#define C_GREEN std::string("\u001b[32m")
#define C_YELLOW std::string("\u001b[33m")
#define C_BLUE std::string("\u001b[34m")
#define C_MAGENTA std::string("\u001b[35m")
#define C_CYAN std::string("\u001b[36m")
#define C_WHITE std::string("\u001b[37m")

#define BG_BLACK std::string("\u001b[40;1m") 
#define BG_RED std::string("\u001b[41;1m") 
#define BG_GREEN std::string("\u001b[42;1m") 
#define BG_YELLOW std::string("\u001b[43;1m")
#define BG_BLUE std::string("\u001b[44;1m")
#define BG_MAGENTA std::string("\u001b[45;1m") 
#define BG_CYAN std::string("\u001b[46;1m")
#define BG_WHITE std::string("\u001b[47m") 

#endif
