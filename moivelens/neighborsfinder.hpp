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
  NeighborsLocator(UsersMap *users, int numOfItems);
  std::vector<Ratings*> getNeighbors(Ratings *targetUser, int k, int method);

private:
  UsersMap *users;
  Ratings *targetUser;
  int numOfItems;
  
  void calculateAllDistances(Distances &distances, double (NeighborsLocator::*distanceFunc)(Ratings&, Ratings&));
  
  double euclideanDistance(Ratings &r1, Ratings &r2);
  double cityBlockDistance(Ratings &r1, Ratings &r2);
  double cosineSimilarity(Ratings &r1, Ratings &r2);
};

/**
 *  Sort `distances`'s leaset(or largest) k elements.
 *  Time complexity: O(n + k*log(k)). When n >> k, O(n)
 *
 *  @param distances Collection to sort
 *  @param k         number of elements to sort
 *  @param ascending Sort in ascending order if set to true
 */
void sort_nth_elemet(Distances &distances, int k, bool ascending);
bool cmp(const Distance &a, const Distance &b);
bool reverseCmp(const Distance &a, const Distance &b);

#endif /* neighborsfinder_hpp */
