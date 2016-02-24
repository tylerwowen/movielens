//
//  argparser.hpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/23/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#ifndef argparser_hpp
#define argparser_hpp

#include <argp.h>
#include <stdlib.h>
#include <string.h>

static const char *program_version = "moivelens v0.1 Weimin Ouyang";

static char doc[] = "Movie lens data analyzer";

static struct argp_option options[] = {
  {"train-data", 'r', "file",     0, "Path to training data"},
  {"test-data",  't', "file",     0, "Path to test data" },
  {"method",     'd', "method",   0, "Method used for similarity measure.\n0: LMax, 1: L1, 2: L2, 3: PCC" },
  {"movie-num",  'm', "number",   0, "Max number of movies"},
  {"user-num",   'u', "number",   0, "Max number of users"},
  {"neighbor-num",'k', "number",  0, "Number of neighbora."},
  { 0 }
};

struct arguments {
  char *trainFile;
  char *testFile;
  int userNum;
  int moiveNum;
  int method;
};

error_t parse_opt (int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, 0, doc };

#endif /* argparser_hpp */
