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
#include <chrono>

#include "argparser.hpp"
#include "common.hpp"
#include "datareader.hpp"
#include "neighborsfinder.hpp"
using namespace std;
using Clock = chrono::high_resolution_clock;
using TimePoint = chrono::time_point<Clock>;

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
  
  int numUsers = args.userNum / 32 * 32;
  UsersMap trainUsers(numUsers), testUsers(numUsers);
  readData(args.trainFile, trainUsers, numUsers);
  int testSize = readData(args.testFile, testUsers, numUsers);
  int validTestSize = testSize;

  NeighborsLocator locator(&trainUsers, args.moiveNum, args.k, args.method, args.maxRating);
  double sum = 0, sumSQ = 0;
  int predictedCount = 0;
  int count_not_found = 0;
  Clock::duration t_dist = chrono::seconds(0);
  Clock::duration t_knn = chrono::seconds(0);
  
  if (args.matchedOnly) {
    auto t_start = Clock::now();
    for (auto& user: testUsers) {
      
      auto t_before = Clock::now();
      
      UsersMap::iterator trainUserItr = trainUsers.find(user.first);
      if (trainUserItr == trainUsers.end()) {
        cout << "not good " << count_not_found++ << " " << user.first << endl;
        validTestSize--;
        continue;
      }
      locator.setTargetUser(user.first, &trainUserItr->second);
      locator.calculateDistancesToNeighbors(args.userNum);
      
      auto t_after = Clock::now();
      t_dist += t_after - t_before;
      
      t_before = Clock::now();
      for (auto& rating: user.second.r_list) {
        UsersPtr neighbors = locator.getMatchedKNeighbors(rating.first);
        double actual = rating.second;
        double prediction = getPredication(neighbors, rating.first);
        if (prediction == 0) {
          continue;
        }
//        cout << user.first << ", " << rating.first
//        << ", " << actual << ", "<< prediction << "\n";
//        cout << "user: " << user.first << " item: " << rating.first
//        << " actual = " << actual << " predicted = "<< prediction << "\n";
        predictedCount++;
        sum += fabs(actual - prediction);
        sumSQ += pow(actual - prediction, 2);
      }
      t_after = Clock::now();
      t_knn += t_after - t_before;
    }
    auto t_end = Clock::now();
    cout << "Distance computation time: " << chrono::duration_cast<std::chrono::milliseconds>(t_dist).count() << " ms\n";
    cout << "KNN computation time: " << chrono::duration_cast<std::chrono::milliseconds>(t_knn).count() << " ms\n";
    cout << "Total computation time: " << chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << " ms\n";
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
    cout << "Train users " << trainUsers.size() << endl;
    cout << "Test users " << testUsers.size() << endl;
    cout << "Testsize = " << testSize << ", valid test size = " << validTestSize << ", predicted = " << predictedCount << endl;
    cout << "MAE = " << mae << endl;
    cout << "RMSE = " << rmse << endl;
    cout << "Recall = " << recall * 100 << "%" << endl;
  }
  else {
    cout << mae << " " << rmse << " " <<  recall << " ";
  }
  
  exit(0);
}

