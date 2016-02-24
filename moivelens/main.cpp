//
//  main.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>

#include "common.hpp"
#include "datareader.hpp"
#include "neighborsfinder.hpp"
using namespace std;

double getPredication(vector<Ratings*> neighbors, int itemId) {
  double validRatings = 0, ratingSum = 0;
  for (int i = 0; i < neighbors.size(); i++) {
    Ratings::const_iterator ratingItr = neighbors[i]->find(itemId);
    if (ratingItr != neighbors[i]->end()) {
      validRatings++;
      ratingSum += ratingItr->second;
    }
  }
  if (validRatings == 0) {
    return 0;
  }
  return ratingSum/validRatings;
}

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
  
  NeighborsLocator locator(&trainUsers, numOfMovies);
  
  for (auto& user: testUsers) {
    vector<Ratings*> neighbors = locator.getNeighbors(&(user.second), 5, PCC);
    cout << "user with id:" << user.first << endl;
    for (auto&rating: user.second) {
      int actual = rating.second;
      double prediction = getPredication(neighbors, rating.first);
      if (prediction != 0) {
        cout << "actual:    " << actual << "\npredicted: " << prediction << endl;
      }
    }
  }
  
  exit(0);
}

