//
//  neighborsfinder.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "neighborsfinder.hpp"
using namespace std;

NeighborsLocator::NeighborsLocator(UsersMap *trainUsers, int numOfItems) {
  this->trainUsers = trainUsers;
  this->numOfItems = numOfItems;
}

UsersPtr NeighborsLocator::getNeighbors(Ratings *targetUser, int k, int method) {
  this->targetUser = targetUser;
  Distances distances(trainUsers->size());
  
  switch (method) {
    case LMax:
      calculateAllDistances(distances, &NeighborsLocator::euclideanDistance);
      sort_nth_elemet(distances, k+1, true);
      break;
    case L1:
      calculateAllDistances(distances, &NeighborsLocator::cityBlockDistance);
      sort_nth_elemet(distances, k+1, true);
      break;
    case L2:
      calculateAllDistances(distances, &NeighborsLocator::cosineSimilarity);
      sort_nth_elemet(distances, k+1, false);
      break;
    case PCC:
      calculateAllDistances(distances, &NeighborsLocator::pcc);
      sort_nth_elemet(distances, k+1, false);
      break;
    default:
      break;
  }
  
  UsersPtr neighbors(k);
  
  for (int i = 1; i <= k; i++) {
    int userId = distances[i].first;
    neighbors[i-1] = &((*trainUsers)[userId]);
  }
  return neighbors;
}

void NeighborsLocator::calculateAllDistances(Distances &distances, double (NeighborsLocator::*distanceFunc)(Ratings&, Ratings&)) {
  int i = 0;
  for (auto& otherUser: *trainUsers) {
    distances[i].first = otherUser.first;
    distances[i].second = (this->*distanceFunc)(otherUser.second, *targetUser);
    i++;
  }
}

double NeighborsLocator::euclideanDistance(Ratings &r1, Ratings &r2) {
  double distance = 0;
  // assume that the moive ids are from 1 to numOfItems
  for (auto& r: r1) {
    int itemId = r.first;
    double rating1 = r.second;
    Ratings::const_iterator r2itr = r2.find(itemId);
    double rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    distance += pow(rating1 - rating2, 2);
  }
  for (auto& r: r2) {
    int itemId = r.first;
    double rating2 = r.second;
    Ratings::const_iterator r1itr = r1.find(itemId);
    double rating1 = r1itr == r1.end() ?  0 : r1itr->second;
    
    distance += pow(rating1 - rating2, 2);
  }
  
  return sqrt(distance);
}

double NeighborsLocator::cityBlockDistance(Ratings &r1, Ratings &r2) {
  double distance = 0;
  
  for (auto& r: r1) {
    int itemId = r.first;
    double rating1 = r.second;
    Ratings::const_iterator r2itr = r2.find(itemId);
    double rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    distance += fabs(rating1 - rating2);
  }
  
  for (auto& r: r2) {
    int itemId = r.first;
    double rating2 = r.second;
    Ratings::const_iterator r1itr = r1.find(itemId);
    double rating1 = r1itr == r1.end() ?  0 : r1itr->second;
    
    distance += fabs(rating1 - rating2);
  }
  
  return distance;
}

double NeighborsLocator::cosineSimilarity(Ratings &r1, Ratings &r2) {
  double dotProduct = 0, norm1SQ = 0, norm2SQ = 0;
  
  for (auto& r: r1) {
    int itemId = r.first;
    double rating1 = r.second;
    Ratings::const_iterator r2itr = r2.find(itemId);
    double rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }
  
  for (auto& r: r2) {
    int itemId = r.first;
    double rating2 = r.second;
    Ratings::const_iterator r1itr = r1.find(itemId);
    double rating1 = r1itr == r1.end() ?  0 : r1itr->second;

    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }

  return dotProduct/(sqrt(norm1SQ) * sqrt(norm2SQ));
}

double NeighborsLocator::pcc(Ratings &r1, Ratings &r2) {
  double sum1 = 0, sum1SQ = 0,
  sum2 = 0, sum2SQ, sum12 = 0;
  
  
  for (auto& r: r1) {
    int itemId = r.first;
    double rating1 = r.second;
    Ratings::const_iterator r2itr = r2.find(itemId);
    double rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    sum12 += rating1 * rating2;
    sum1 += rating1;
    sum1SQ += rating1 * rating1;
    sum2 += rating2;
    sum2SQ += rating2 * rating2;
  }
  for (auto& r: r2) {
    int itemId = r.first;
    double rating2 = r.second;
    Ratings::const_iterator r1itr = r1.find(itemId);
    double rating1 = r1itr == r1.end() ?  0 : r1itr->second;
    
    sum12 += rating1 * rating2;
    sum1 += rating1;
    sum1SQ += rating1 * rating1;
    sum2 += rating2;
    sum2SQ += rating2 * rating2;
  }

  double covariance = (sum12 - sum1 * sum2 / numOfItems) / numOfItems;
  double mean1 = sum1 / numOfItems;
  double mean2 = sum2 / numOfItems;
  double variance1 = sum1SQ / numOfItems - mean1 * mean1;
  double variance2 = sum2SQ / numOfItems - mean2 * mean2;
  
  return covariance / (sqrt(variance1) * sqrt(variance2));
}

void sort_nth_elemet(Distances &distances, int k, bool ascending) {
  if (ascending) {
    nth_element(distances.begin(), distances.begin() + k, distances.end(), cmp);
    sort(distances.begin(), distances.begin() + k, cmp);
  }
  else {
    nth_element(distances.begin(), distances.begin() + k, distances.end(), reverseCmp);
    sort(distances.begin(), distances.begin() + k, reverseCmp);
  }
}

bool cmp(const Distance &a, const Distance &b) {
  return a.second < b.second;
}

bool reverseCmp(const Distance &a, const Distance &b) {
  return a.second > b.second;
}
