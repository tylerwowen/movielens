//
//  neighborsfinder.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "neighborsfinder.hpp"
using namespace std;

NeighborsLocator::NeighborsLocator(UsersMap *users, Ratings *targetUser, int numOfItems) {
  this->users = users;
  this->targetUser = targetUser;
  this->numOfItems = numOfItems;
}

vector<Ratings*> NeighborsLocator::getNeighbors(int k, int method) {
  Distances distances(users->size());
  
  switch (method) {
    case LMax:
      calculateAllDistances(distances, &NeighborsLocator::euclideanDistance);
      sort_nth_elemet(distances, k, true);
      break;
    case L1:
      calculateAllDistances(distances, &NeighborsLocator::cityBlockDistance);
      sort_nth_elemet(distances, k, true);
      break;
    case L2:
      calculateAllDistances(distances, &NeighborsLocator::cosineSimilarity);
      sort_nth_elemet(distances, k, false);
      break;
    case PCC:
      // TODO: implement PCC
      calculateAllDistances(distances, &NeighborsLocator::cosineSimilarity);
      sort_nth_elemet(distances, k, false);
      break;
    default:
      break;
  }

  vector<Ratings*> neighbors;
  
  for (int i = 0; i < k; i++) {
    int userId = distances[i].first;
    neighbors.push_back(&((*users)[userId]));
    cout << "Neighbor" << i << " is " << userId << endl;
    
  }
  return neighbors;
}

void NeighborsLocator::calculateAllDistances(Distances &distances, double (NeighborsLocator::*distanceFunc)(Ratings&, Ratings&)) {
  int i = 0;
  for (auto& otherUser: *users) {
    distances[i].first = otherUser.first;
    distances[i].second = (this->*distanceFunc)(otherUser.second, *targetUser);
    i++;
  }
}

double NeighborsLocator::euclideanDistance(Ratings &r1, Ratings &r2) {
  double distance = 0;
  // assume that the moive ids are from 1 to numOfItems
  for (int itemId = 1; itemId <= numOfItems; itemId++) {
    int rating1, rating2;
    Ratings::const_iterator r1itr = r1.find(itemId);
    rating1 = r1itr == r1.end() ? 0 : r1itr->second;
    
    Ratings::const_iterator r2itr = r2.find(itemId);
    rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    distance += pow(rating1 - rating2, 2);
  }
  return sqrt(distance);
}

double NeighborsLocator::cityBlockDistance(Ratings &r1, Ratings &r2) {
  double distance = 0;
  for (int itemId = 1; itemId <= numOfItems; itemId++) {
    int rating1, rating2;
    Ratings::const_iterator r1itr = r1.find(itemId);
    rating1 = r1itr == r1.end() ? 0 : r1itr->second;
    
    Ratings::const_iterator r2itr = r2.find(itemId);
    rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    distance += fabs(rating1 - rating2);
  }
  return distance;
}

double NeighborsLocator::cosineSimilarity(Ratings &r1, Ratings &r2) {
  double dotProduct = 0, norm1SQ = 0, norm2SQ = 0;
  for (int itemId = 1; itemId <= numOfItems; itemId++) {
    int rating1, rating2;
    Ratings::const_iterator r1itr = r1.find(itemId);
    rating1 = r1itr == r1.end() ? 0 : r1itr->second;
    
    Ratings::const_iterator r2itr = r2.find(itemId);
    rating2 = r2itr == r2.end() ?  0 : r2itr->second;
    
    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }
  return dotProduct/(sqrt(norm1SQ) * sqrt(norm2SQ));
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
  return a.second >= b.second;
}
