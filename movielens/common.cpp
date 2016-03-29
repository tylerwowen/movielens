//
//  common.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 3/28/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include <stdio.h>
#include "common.hpp"

bool compare(const Rating &a, const Rating &b) {
  return a.first < b.first;
}

void Ratings::insert(int itemId, double rating) {
  r_list.emplace_back(itemId, rating);
  r_map.insert(std::pair<int, double>(itemId, rating));
}

void Ratings::sortList() {
  r_list.sort(compare);
}
