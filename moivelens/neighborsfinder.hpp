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
  NeighborsLocator(UsersMap *trainUsers, int numOfItems);
  UsersPtr getNeighbors(Ratings *targetUser, int k, int method);

private:
  UsersMap *trainUsers;
  Ratings *targetUser;
  int numOfItems;
  
  /**
   *  Calculates all the distances from `this->targetUser` to all other users
   *
   *  @param distances    Calculated results
   *  @param distanceFunc The function used to calculate distances
   */
  void calculateAllDistances(Distances &distances, double(NeighborsLocator::*distanceFunc)(Ratings&, Ratings&));
  
  double euclideanDistance(Ratings &r1, Ratings &r2);
  double cityBlockDistance(Ratings &r1, Ratings &r2);
  double cosineSimilarity(Ratings &r1, Ratings &r2);
  double pcc(Ratings &r1, Ratings &r2);
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
