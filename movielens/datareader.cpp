//
//  datareader.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "datareader.hpp"

using namespace std;

int readData(string const &filename, UsersMap &users) {
  ifstream ifs(filename);
  locale colon(std::locale::classic(), new my_ctype);
  ifs.imbue(colon);
  int ratingCount = 0;
  
  while (true) {
    int userId, itermId, timestamp;
    double rating;
    ifs >>  userId >> itermId >> rating >> timestamp;
    if (ifs.fail()) {
      break;
    }
    Ratings *ratings = &users[userId];
    ratings->insert(itermId, rating);
    ratingCount++;
  }
  for (auto& user: users) {
    user.second.sortList();
  }
  
  return ratingCount;
}

