//
//  common.hpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/22/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef common_hpp
#define common_hpp

#include <unordered_map>
#include <vector>

#define LMax 0
#define L1 1
#define L2 2
#define PCC 3

/**
 *  {itemId, rating}
 */
typedef std::unordered_map<int, float> Ratings;

/**
 *  <Ratings>
 */
typedef std::vector<Ratings> Users;

/**
 *  <Ratings*>
 */
typedef std::vector<Ratings*> UsersPtr;

#endif /* common_hpp */
