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


#define L1 1
#define L2 2
#define LMax 3
#define PCC 4

/**
 *  {itemId, rating}
 */
typedef std::unordered_map<int, float> Ratings;

/**
 *  {userId, Ratings}
 */
typedef std::unordered_map<int, Ratings> UsersMap;


#endif /* common_hpp */
