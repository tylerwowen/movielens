//
//  main.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <argp.h>
#include <iostream>
#include <map>
#include <vector>

#include "argparser.hpp"
#include "common.hpp"
#include "datareader.hpp"
#include "neighborsfinder.hpp"
using namespace std;

double getPredication(UsersPtr neighbors, int itemId) {
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

int main(int argc, char ** argv) {
  struct arguments args;
  args.method = 0;
  
  argp_parse (&argp, argc, argv, 0, 0, &args);
  
  UsersMap trainUsers(args.userNum), testUsers;
  readData(args.trainFile, trainUsers);
  readData(args.testFile, testUsers);

  NeighborsLocator locator(&trainUsers, args.moiveNum);
  int count = 0;
  for (auto& user: testUsers) {
    UsersPtr neighbors = locator.getNeighbors(&(user.second), args.k, args.method);
//    cout << "user with id:" << distance(trainUsers.begin(), user) << endl;
    for (auto&rating: user.second) {
      int actual = rating.second;
      double prediction = getPredication(neighbors, rating.first);
      if (prediction != 0) {
        //cout << "actual:    " << actual << "\npredicted: " << prediction << endl;
        count++;
      }
    }
  }
  cout << "processed: " << count << endl;
  exit(0);
}

