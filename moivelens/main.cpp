//
//  main.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <vector>

#include "common.hpp"
#include "datareader.hpp"
#include "neighborsfinder.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
  if (argc < 4) {
    cout << "Please give me a set of files.\n";
    exit(1);
  }
  
  int numOfMovies = atoi(argv[1]);
  UsersMap trainUsers, testUsers;
  readData(argv[2], trainUsers);
  readData(argv[3], testUsers);
  // consider converting map to vector to improve performance after data is read
  
  NeighborsLocator neighborsLocator(&trainUsers, &trainUsers[10], numOfMovies);
  neighborsLocator.getNeighbors(5, LMax);
  neighborsLocator.getNeighbors(5, L1);
  neighborsLocator.getNeighbors(5, L2);
  exit(0);
}

