//
//  datareader.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "datareader.hpp"

using namespace std;

void readData(string const &filename, Users &users) {
  ifstream ifs(filename);
  locale colon(std::locale::classic(), new my_ctype);
  ifs.imbue(colon);
  
  while (ifs.good()) {
    int userId, itermId, timestamp;
    float rating;
    ifs >>  userId >> itermId >> rating >> timestamp;
//    if (users[userId]) {
//      <#statements#>
//    }
    
    Ratings *ratings = &users[userId];
    ratings->insert(pair<int, float>(itermId, rating));
  }
}