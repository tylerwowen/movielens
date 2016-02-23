//
//  datareader.hpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/21/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef datareader_hpp
#define datareader_hpp

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "common.hpp"

void readData(std::string const &filename, UsersMap &users);

#endif /* datareader_hpp */
