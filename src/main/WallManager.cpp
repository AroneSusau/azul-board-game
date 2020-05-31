#include "../headers/WallManager.h"

WallManager::WallManager() :
  wall(new Wall<bool>()),
  colours(new Wall<Colour>()),
  printer(new Printer())
{
  for (int i = 0; i < DIMENSIONS; ++i) {

    int set = i;

    for (int j = 0; j < DIMENSIONS; ++j) {
      wall->set(i, j, new bool(false));
      
      if (set >= 5) {
        colours->set(i, j, new Colour(setWallTile(set - 5)));
      } else {
        colours->set(i, j, new Colour(setWallTile(set)));
      }
      ++set;

    } 
  }
}

WallManager::~WallManager() {
  delete wall;
  delete colours;
  delete printer;
}

bool WallManager::isTileSet(int row, int col) {
  return *wall->get(row, col);
}

bool WallManager::isColourSet(Colour colour, int row) {
  bool result = false;
  
  if (row >= 0 && row < DIMENSIONS) {
    result = *wall->get(row, getColourDim(colour, row));
  } else {
    printer->error("Error: Attempting to access out of bounds Wall tile.");
  }

  return result;
}

int WallManager::getColourDim(Colour colour, int row) {
  int result = -1;

  if (row >= 0 && row < DIMENSIONS) {
    for (int i = 0; i < DIMENSIONS; ++i) {
      if (*colours->get(row, i) == colour) {
        result = i;
      }
    }
  } else {
    printer->error("Error: Attempting to access out of bounds Wall tile.");
  }

  return result;
}

Colour WallManager::nextAvailableColour(int row) {
  Colour result = EMPTY;

  if (row >= 0 && row < DIMENSIONS) {
    for (int i = 0; i < DIMENSIONS; ++i) {
      if (!*wall->get(row, i)) {
        result = *colours->get(row, i);
        i = DIMENSIONS;
      }
    }
  } else {
    printer->error("Error: Attempting to access out of bounds Wall tile.");
  }

  return result;
}

int WallManager::roundPoints(int row, int col) {
  int horizontalPoints = 1;
  int verticalPoints = 1;
  int result = 0;
  
  // Horizontal scoring
  bool leftCheck = true;
  int leftLimit = col - 1;

  bool rightCheck = true;
  int rightLimit = col + 1;
  
  while (leftCheck && leftLimit >= 0) {
    if (*wall->get(row, leftLimit)) {
      leftLimit--;
      horizontalPoints++;
    } else {
      leftCheck = false;
    }
  }
  
  while (rightCheck && rightLimit < DIMENSIONS) {
    if (*wall->get(row, rightLimit)) {
      rightLimit++;
      horizontalPoints++;
    } else {
      rightCheck = false;
    }
  }
  
  // Vertical scoring
  bool upCheck = true;
  int upLimit = row - 1;

  bool downCheck = true;
  int downLimit = row + 1;

  while (upCheck && upLimit >= 0) {
    if (*wall->get(upLimit, col)) {
      upLimit--;
      verticalPoints++;
    } else {
      upCheck = false;
    }
  }

  while (downCheck && downLimit < DIMENSIONS) {
    if (*wall->get(downLimit, col)) {
      downLimit++;
      verticalPoints++;
    } else {
      downCheck = false;
    }
  }

  if (horizontalPoints == 1 && verticalPoints == 1) {
    result = 1;
  } else if (horizontalPoints == 1) {
    result = verticalPoints;
  } else if (verticalPoints == 1) {
    result = horizontalPoints;
  } else {
    result = verticalPoints + horizontalPoints;
  }

  return result;
}

bool WallManager::rowTrue(int row) {
  bool result = true;

  for (int i = 0; i < DIMENSIONS; ++i) {
    if (!*wall->get(row, i)) {
      result = false;
    }
  }

  return result;
}

bool WallManager::colTrue(int col) {
  bool result = true;

  for (int i = 0; i < DIMENSIONS; ++i) {
    if (!*wall->get(i, col)) {
      result = false;
    }
  }

  return result;
}

bool WallManager::colourTrue(Colour colour) {
  bool result = true;

  for (int i = 0; i < DIMENSIONS; ++i) {
    if (!wall->get(i, getColourDim(colour, i))) {
      result = false;
    }
  }

  return result;
}

int WallManager::endPoints() {
  int points = 0;
  Colour colours[5] = {RED, YELLOW, DARK_BLUE, LIGHT_BLUE, BLACK};

  for (int i = 0; i < DIMENSIONS; ++i) {
    //+2 points for each completed row
    if (rowTrue(i)) {
      points += 2;
    }

    //+7 points for each completed column
    if (colTrue(i)) {
      points += 7;
    }

    //+10 points for each completed set of colours
    if (colourTrue(colours[i])) {
      points += 10;
    }
  }

  return points;
}

Colour WallManager::setWallTile(int colour) {
  Colour colours[5] = {RED, YELLOW, DARK_BLUE, LIGHT_BLUE, BLACK};
  Colour result = EMPTY;

  if (colour >= 0 && colour < 5) {
    result = colours[colour];
  }

  return result;
}

Wall<bool>* WallManager::getWall() {
  return wall;
}

Wall<Colour>* WallManager::getColours() {
  return colours;
}

void WallManager::printWall(int row) {
  
  for (int i = 0; i < DIMENSIONS; ++i) {

    Colour colour = *colours->get(row, i);

    if (colour == BLACK) {
        std::cout << BG_BLACK;
      } else if (colour == RED) {
        std::cout << BG_RED;
      } else if (colour == LIGHT_BLUE) {
        std::cout << BG_CYAN;
      } else if (colour == DARK_BLUE) {
        std::cout << BG_BLUE;
      } else if (colour == YELLOW) {
        std::cout << BG_YELLOW << C_BLACK;
      }

    if (*wall->get(row, i)) {
      std::cout << " " << (char) colour << " " << C_RESET;
    } else {
      std::cout << " ." << " " << C_RESET;
    }
  }
}

std::string WallManager::toSaveString() {
  std::string output = "";

  for (int row = 0; row < DIMENSIONS; ++row) {
    for (int col = 0; col < DIMENSIONS; ++col) {
      bool positionFilled = *wall->get(row, col);
      char colour = (char) *colours->get(row, col);

      if (positionFilled) {
        output += colour;
      } else {
        output += EMPTY;
      }
    }
    output += ",";
  }

  return output;
}
