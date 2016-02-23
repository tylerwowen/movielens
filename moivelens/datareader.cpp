//
//  datareader.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "datareader.hpp"

using namespace std;

void readData(string const &filename, UsersMap &users) {
  ifstream ifs(filename);
  while (ifs.good()) {
    int userId, itermId, rating, timestamp;
    ifs >>  userId >> itermId >> rating >> timestamp;
    Ratings *ratings = &users[userId];
    ratings->insert(pair<int, int>(itermId, rating));
  }
}

//  int numberOfLines = (int)count(istreambuf_iterator<char>(ifs),
//                                 istreambuf_iterator<char>(), '\n');