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
    case COS:
      calculateAllDistances(distances, &NeighborsLocator::cosineSimilarity);
      sort_nth_elemet(distances, k+1, false);
      break;
    case L1:
      calculateAllDistances(distances, &NeighborsLocator::cityBlockDistance);
      sort_nth_elemet(distances, k+1, true);
      break;
    case L2:
      calculateAllDistances(distances, &NeighborsLocator::euclideanDistance);
      sort_nth_elemet(distances, k+1, true);
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
  Ratings::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      rating1 = iter1->second;
      rating2 = defaultRatingForItem(iter1->first);
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
    }
    else {
      rating1 = defaultRatingForItem(iter2->first);
      rating2 = iter2->second;
      iter2++;
    }
    distance += pow(rating1 - rating2, 2);
  }
  
  Rating last = iter1 == r1.cend() ? r1.back() : r2.back();
  Ratings::const_iterator activeIter = iter1 == r1.cend() ? iter2 : iter1,
  activeIterCend = iter1 == r1.cend() ? r2.cend() : r1.cend();
  
  while (activeIter != activeIterCend) {
    double rating1 = activeIter->second, rating2 = 0;
    if (activeIter->first != last.first) {
      rating2 = defaultRatingForItem(activeIter->first);
    }
    else {
      rating2 = last.second;
    }
    activeIter++;
    distance += pow(rating1 - rating2, 2);
  }
  return sqrt(distance);
}

double NeighborsLocator::cityBlockDistance(Ratings &r1, Ratings &r2) {
  double distance = 0;
  
  Ratings::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      rating1 = iter1->second;
      rating2 = defaultRatingForItem(iter1->first);
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
    }
    else {
      rating1 = defaultRatingForItem(iter2->first);
      rating2 = iter2->second;
      iter2++;
    }
    distance += fabs(rating1 - rating2);
  }
  
  Rating last = iter1 == r1.cend() ? r1.back() : r2.back();
  Ratings::const_iterator activeIter = iter1 == r1.cend() ? iter2 : iter1,
  activeIterCend = iter1 == r1.cend() ? r2.cend() : r1.cend();
  
  while (activeIter != activeIterCend) {
    double rating1 = activeIter->second, rating2 = 0;
    if (activeIter->first != last.first) {
      rating2 = defaultRatingForItem(activeIter->first);
    }
    else {
      rating2 = last.second;
    }
    activeIter++;
    distance += fabs(rating1 - rating2);
  }
  
  return distance;
}

double NeighborsLocator::cosineSimilarity(Ratings &r1, Ratings &r2) {
  double dotProduct = 0, norm1SQ = 0, norm2SQ = 0;
  
  Ratings::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      rating1 = iter1->second;
      rating2 = defaultRatingForItem(iter1->first);
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
    }
    else {
      rating1 = defaultRatingForItem(iter2->first);
      rating2 = iter2->second;
      iter2++;
    }
    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }
  
  Rating r1Last = r1.back();
  while (iter2 != r2.cend()) {
    double rating1 = 0, rating2 = iter2->second;
    if (iter2->first != r1Last.first) {
      rating1 = defaultRatingForItem(iter2->first);
    }
    else {
      rating1 = r1Last.second;
    }
    iter2++;
    
    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }
  Rating r2Last = r2.back();
  while (iter1 != r1.cend()) {
    double rating1 = iter1->second, rating2 = 0;
    if (iter2->first != r1Last.first) {
      rating2 = defaultRatingForItem(iter2->first);
    }
    else {
      rating2 = r1Last.second;
    }
    iter1++;
    
    dotProduct += rating1 * rating2;
    norm1SQ += rating1 * rating1;
    norm2SQ += rating2 * rating2;
  }

  // TODO: handle non-0 situations
  return dotProduct/(sqrt(norm1SQ) * sqrt(norm2SQ));
}

double NeighborsLocator::pcc(Ratings &r1, Ratings &r2) {
  double sum1 = 0, sum1SQ = 0,
  sum2 = 0, sum2SQ = 0, sum12 = 0;
  
  Ratings::const_iterator iter1 = r1.cbegin(), iter2 = r2.cbegin();
  while (iter1 != r1.cend() && iter2 != r2.cend()) {
    double rating1 = 0, rating2 = 0;
    
    if (iter1->first < iter2->first) {
      rating1 = iter1->second;
      rating2 = defaultRatingForItem(iter1->first);
      iter1++;
    }
    else if (iter1->first == iter2->first) {
      rating1 = iter1->second;
      rating2 = iter2->second;
      iter1++;
      iter2++;
    }
    else {
      rating1 = defaultRatingForItem(iter2->first);
      rating2 = iter2->second;
      iter2++;
    }
    sum12 += rating1 * rating2;
    sum1 += rating1;
    sum1SQ += rating1 * rating1;
    sum2 += rating2;
    sum2SQ += rating2 * rating2;
  }
  
  Rating r1Last = r1.back();
  while (iter2 != r2.cend()) {
    double rating1 = 0, rating2 = iter2->second;
    if (iter2->first != r1Last.first) {
      rating1 = defaultRatingForItem(iter2->first);
    }
    else {
      rating1 = r1Last.second;
    }
    iter2++;
    
    sum12 += rating1 * rating2;
    sum1 += rating1;
    sum1SQ += rating1 * rating1;
    sum2 += rating2;
    sum2SQ += rating2 * rating2;
  }
  Rating r2Last = r2.back();
  while (iter1 != r1.cend()) {
    double rating1 = iter1->second, rating2 = 0;
    if (iter2->first != r1Last.first) {
      rating2 = defaultRatingForItem(iter2->first);
    }
    else {
      rating2 = r1Last.second;
    }
    iter1++;
    
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

int defaultRatingForItem(int id) {
  return 0;
}
