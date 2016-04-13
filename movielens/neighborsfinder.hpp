//
//  neighborsfinder.hpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef neighborsfinder_hpp
#define neighborsfinder_hpp

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "common.hpp"

/**
 *  {userId, distance value}
 */
typedef std::pair<int, double> Distance;

/**
 *  Vector of distances from one user to others.
 */
typedef std::vector<Distance> Distances;

class NeighborsLocator {
public:
  NeighborsLocator(UsersMap *trainUsers, int numOfItems, int k, int method, int maxRating);
  UsersPtr getNeighbors();
  UsersPtr getMatchedKNeighbors(int targetItem);
  void setTargetUser(int targetUserId, Ratings *targetUserRatings);
  void calculateDistancesToNeighbors(int numUsers);
  
private:
  UsersMap *trainUsers;
  int numOfItems;
  int k;
  int method;
  int maxRating;
  
  int targetUserId;
  Ratings *targetUserRatings;
  
  std::vector<double> cachedDistances;
  int targetItemId;
  
  /**
   *  Calculates all the distances from `this->targetUserRatings` to all other users
   *
   *  @param distances    Calculated results
   *  @param distanceFunc The function used to calculate distances
   */
  void calculateSortableAllDistances(Distances &distances, double(NeighborsLocator::*distanceFunc)(Ratings_list&, Ratings_list&));
  void calculateAllDistances(double(NeighborsLocator::*distanceFunc)(Ratings_list&, Ratings_list&));
  void calculateDistances(Distances &distances);
  double euclideanDistance(Ratings_list &r1, Ratings_list &r2);
  double cityBlockDistance(Ratings_list &r1, Ratings_list &r2);
  double cosineSimilarity(Ratings_list &r1, Ratings_list &r2);
  double pcc(Ratings_list &r1, Ratings_list &r2);
  double llr(Ratings_list &r1, Ratings_list &r2);
};

/**
 *  Sort `distances`'s least(or largest) k elements.
 *  Time complexity: O(n + k*log(k)). When n >> k, O(n)
 *
 *  @param distances Collection to sort
 *  @param k         number of elements to sort
 *  @param ascending Sort in ascending order if set to true
 */
void sort_nth_elemet(Distances &distances, int k, bool ascending);
bool cmp(const Distance &a, const Distance &b);
bool reverseCmp(const Distance &a, const Distance &b);

int defaultRatingForItem(int id);
#endif /* neighborsfinder_hpp */
