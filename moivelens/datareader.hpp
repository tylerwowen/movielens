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

class my_ctype : public std::ctype<char> {
  mask my_table[table_size];
public:
  my_ctype(size_t refs = 0) : std::ctype<char>(&my_table[0], false, refs) {
    std::copy_n(classic_table(), table_size, my_table);
    my_table[':'] = (mask)space;
    my_table[' '] = (mask)space;
  }
};

int readData(std::string const &filename, UsersMap &users);

#endif /* datareader_hpp */
