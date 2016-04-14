//
//  neighborsfinder_ignore.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "neighborsfinder.hpp"
using namespace std;

NeighborsLocator::NeighborsLocator(UsersMap *trainUsers, int numOfItems, int k, int method, int maxRating) {
  this->trainUsers = trainUsers;
  this->numOfItems = numOfItems;
  this->k = k;
  this->method = method;
  this->maxRating = maxRating;
}

void NeighborsLocator::setTargetUser(int targetUserId, Ratings *targetUserRatings) {
  this->targetUserId = targetUserId;
  this->targetUserRatings = targetUserRatings;
}

UsersPtr NeighborsLocator::getNeighbors() {
  Distances distances(trainUsers->size());
  
  switch (method) {
    case COS:
      calculateSortableAllDistances(distances, &NeighborsLocator::cosineSimilarity);
      sort_nth_elemet(distances, k+1, false);
      break;
    case L1:
      calculateSortableAllDistances(distances, &NeighborsLocator::cityBlockDistance);
      sort_nth_elemet(distances, k+1, true);
      break;
    case L2:
      calculateSortableAllDistances(distances, &NeighborsLocator::euclideanDistance);
      sort_nth_elemet(distances, k+1, true);
      break;
    case PCC:
      calculateSortableAllDistances(distances, &NeighborsLocator::pcc);
      sort_nth_elemet(distances, k+1, false);
      break;
    default:
      break;
  }
  
  UsersPtr neighbors(k);
  for (int i = 0, j = 0; i < k; j++) {
    int userId = distances[j].first;
    if (userId == targetUserId) {
      continue;
    }
    neighbors[i++] = &((*trainUsers)[userId]);
  }
  return neighbors;
}

void NeighborsLocator::calculateDistancesToNeighbors(int numUsers) {
  cachedDistances.assign(numUsers + 1, 0.0);
  switch (method) {
    case COS:
      calculateAllDistances(&NeighborsLocator::cosineSimilarity);
      break;
    case L1:
      calculateAllDistances(&NeighborsLocator::cityBlockDistance);
      break;
    case L2:
      calculateAllDistances(&NeighborsLocator::euclideanDistance);
      break;
    case PCC:
      calculateAllDistances(&NeighborsLocator::pcc);
      break;
    default:
      break;
  }
}

UsersPtr NeighborsLocator::getMatchedKNeighbors(int targetItem) {
  this->targetItemId = targetItem;
  Distances distances;
  distances.reserve(100);
  
  calculateDistances(distances);
  if (distances.size() > k) {
    switch (method) {
      case COS:
      case PCC:
        sort_nth_elemet(distances, k + 1, false);
        break;
      case L1:
      case L2:
        sort_nth_elemet(distances, k + 1, true);
        break;
      default:
        break;
    }
  }
  
  UsersPtr neighbors;
  int end = k < distances.size() ? k : (int)distances.size();
  for (int i = 0, j = 0; i < end; j++) {
    int userId = distances[j].first;
    if (userId == targetUserId) {
      if (end != k) {
        end--;
      }
      continue;
    }
    neighbors.push_back(&((*trainUsers)[userId]));
    i++;
  }
  return neighbors;
}

void NeighborsLocator::calculateSortableAllDistances(Distances &distances, double (NeighborsLocator::*distanceFunc)(Ratings_list&, Ratings_list&)) {
  int i = 0;
  for (auto& otherUser: *trainUsers) {
    distances[i].first = otherUser.first;
    distances[i].second = (this->*distanceFunc)(otherUser.second.r_list, targetUserRatings->r_list);
    i++;
  }
}

void NeighborsLocator::calculateAllDistances(double (NeighborsLocator::*distanceFunc)(Ratings_list&, Ratings_list&)) {
  for (auto& otherUser: *trainUsers) {
    cachedDistances[otherUser.first] = (this->*distanceFunc)(otherUser.second.r_list, targetUserRatings->r_list);
  }
}

void NeighborsLocator::calculateDistances(Distances &distances) {
  for (auto& otherUser: *trainUsers) {
    Ratings_map *ratings = &otherUser.second.r_map;
    Ratings_map::const_iterator ratingItr = ratings->find(targetItemId);
    if (ratingItr != ratings->end()) {
      distances.push_back({otherUser.first, cachedDistances[otherUser.first]});
    }
  }
}

double NeighborsLocator::euclideanDistance(Ratings_list &r1, Ratings_list &r2) {
  double distance = 0;
  Ratings_list::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
      distance += pow(rating1 - rating2, 2);
    }
    else {
      iter2++;
    }
  }
  if (distance == 0) {
    return 1E10;
  }
  
  return sqrt(distance);
}

double NeighborsLocator::cityBlockDistance(Ratings_list &r1, Ratings_list &r2) {
  double distance = 0;
  
  Ratings_list::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
      distance += fabs(rating1 - rating2);
    }
    else {
      iter2++;
    }
  }
  if (distance == 0) {
    return 1E10;
  }
  
  return distance;
}

double NeighborsLocator::cosineSimilarity(Ratings_list &r1, Ratings_list &r2) {
  double dotProduct = 0, norm1SQ = 0, norm2SQ = 0;
  
  Ratings_list::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
      dotProduct += rating1 * rating2;
      norm1SQ += rating1 * rating1;
      norm2SQ += rating2 * rating2;
    }
    else {
      iter2++;
    }
  }

  if (dotProduct < 1) {
    return 0;
  }
  return dotProduct/(sqrt(norm1SQ) * sqrt(norm2SQ));
}

double NeighborsLocator::pcc(Ratings_list &r1, Ratings_list &r2) {
  double sum1 = 0, sum1SQ = 0,
  sum2 = 0, sum2SQ = 0, sum12 = 0;
  
  Ratings_list::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
      sum12 += rating1 * rating2;
      sum1 += rating1;
      sum1SQ += rating1 * rating1;
      sum2 += rating2;
      sum2SQ += rating2 * rating2;
    }
    else {
      iter2++;
    }
  }
  
  if (sum12 < 1) {
    return -1;
  }

  double mean1 = sum1 / numOfItems;
  double mean2 = sum2 / numOfItems;

  return (sum12 - numOfItems * mean1 * mean2) / (sqrt(sum1SQ - numOfItems * mean1 * mean1) * sqrt(sum2SQ - numOfItems * mean2 * mean2));
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

