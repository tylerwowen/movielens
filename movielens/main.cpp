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
    Ratings_map::const_iterator ratingItr = neighbors[i]->r_map.find(itemId);
    if (ratingItr != neighbors[i]->r_map.end()) {
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
  args.matchedOnly = false;
  args.prettyPrint = false;
  args.maxRating = 5;
  
  argp_parse (&argp, argc, argv, 0, 0, &args);
  
  UsersMap trainUsers(args.userNum), testUsers;
  readData(args.trainFile, trainUsers);
  int testSize = readData(args.testFile, testUsers);
  int validTestSize = testSize;

  NeighborsLocator locator(&trainUsers, args.moiveNum, args.k, args.method, args.maxRating);
  double sum = 0, sumSQ = 0;
  int predictedCount = 0;
  int count_not_found = 0;
  if (args.matchedOnly) {
    for (auto& user: testUsers) {
      UsersMap::iterator trainUserItr = trainUsers.find(user.first);
      if (trainUserItr == trainUsers.end()) {
        cout << "not good " << count_not_found++ << " " << user.first << endl;
        validTestSize--;
        continue;
      }
      locator.setTargetUser(user.first, &trainUserItr->second);
      locator.calculateDistancesToNeighbors(args.userNum);
      
      for (auto& rating: user.second.r_list) {
        UsersPtr neighbors = locator.getMatchedKNeighbors(rating.first);
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
      UsersMap::iterator trainUserItr = trainUsers.find(user.first);
      if (trainUserItr == trainUsers.end()) {
        cout << "not good\n" ;
        validTestSize--;
        continue;
      }
      locator.setTargetUser(user.first, &trainUserItr->second);
      UsersPtr neighbors = locator.getNeighbors();
      
      for (auto& rating: user.second.r_list) {
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
  double mae = sum / predictedCount,
  rmse = sqrt(sumSQ / predictedCount),
  recall = (double)predictedCount / (double)validTestSize;
  
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

