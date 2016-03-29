//
//  common.hpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include <list>
#include <unordered_map>
#include <vector>

#define COS 0
#define L1 1
#define L2 2
#define PCC 3

/**
 *  {itemId, rating}
 */
typedef std::pair<int, double> Rating;

/**
 *  [{itemId, rating}, ...]
 */
typedef std::list<std::pair<int, double>> Ratings_list;
typedef std::unordered_map<int, double> Ratings_map;

class Ratings {
public:
  Ratings_list r_list;
  Ratings_map r_map;
  void insert(int itemId, double rating);
  void sortList();
};

/**
 *  [{userId, Ratings}, ...]
 */
typedef std::unordered_map<int, Ratings> UsersMap;

/**
 *  [{Ratings*}, ...]
 */
typedef std::vector<Ratings*> UsersPtr;

#endif /* common_hpp */
