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
  int validRatings = 0;
  double ratingSum = 0;
  for (int i = 0; i < neighbors.size(); i++) {
    Ratings *neighbor = neighbors[i];
    Ratings::const_iterator ratingItr = neighbor->cbegin();
    for (; ratingItr != neighbor->cend(); ratingItr++) {
      if (ratingItr->first == itemId) {
        validRatings++;
        ratingSum += ratingItr->second;
        break;
      }
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
  args.matchedOnly = false;
  args.prettyPrint = false;
  
  argp_parse (&argp, argc, argv, 0, 0, &args);
  
  UsersMap trainUsers(args.userNum), testUsers;
  readData(args.trainFile, trainUsers);
  int testSize = readData(args.testFile, testUsers);

  NeighborsLocator locator(&trainUsers, args.moiveNum);
  double sum = 0, sumSQ = 0;
  int predictedCount = 0;
  
  if (args.matchedOnly) {
    for (auto& user: testUsers) {
      locator.setTargetUser(user.first, &(user.second));
      
      for (auto& rating: user.second) {
        UsersPtr neighbors = locator.getMatchedNeighbors(args.k, args.method, rating.first);
        double actual = rating.second;
        double prediction = getPredication(neighbors, rating.first);
        if (prediction == 0) {
          continue;
        }
        predictedCount++;
        sum += fabs(actual - prediction);
        sumSQ += pow(actual - prediction, 2);
      }
    }
  }
  else {
    for (auto& user: testUsers) {
      locator.setTargetUser(user.first, &(user.second));
      UsersPtr neighbors = locator.getNeighbors(args.k, args.method);
      
      for (auto& rating: user.second) {
        double actual = rating.second;
        double prediction = getPredication(neighbors, rating.first);
        if (prediction == 0) {
          continue;
        }
        predictedCount++;
        sum += fabs(actual - prediction);
        sumSQ += pow(actual - prediction, 2);
      }
    }
  }
  double mae = sum / testSize,
  rmse = sqrt(sumSQ / testSize),
  recall = (double)predictedCount / (double)testSize;
  
  if (args.prettyPrint) {
    cout << "MAE = " << mae << endl;
    cout << "RMSE = " << rmse << endl;
    cout << "Recall = " << recall << "%" << endl;
  }
  else {
    cout << mae << " " << rmse << " " <<  recall << " ";
  }
  
  exit(0);
}

