//
//  argparser.cpp
//  moivelens
//
//  Created by Tyler Weimin Ouyang on 2/23/16.
//  Copyright © 2016 Tyler Ouyang. All rights reserved.
//

#include "argparser.hpp"

/* Parse a single option. */
error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
   know is a pointer to our arguments structure. */
  struct arguments *arguments = (struct arguments *)state->input;
  
  switch (key) {
    case 'r':
      arguments->trainFile = arg;
      break;
    case 't':
      arguments->testFile = arg;
      break;
    case 'd':
      arguments->method = atoi(arg);
      break;
    case 'm':
      arguments->moiveNum = atoi(arg);
      break;
    case 'u':
      arguments->userNum = atoi(arg);
      break;
    case 'k':
      arguments->k = atoi(arg);
      break;
    case 'o':
      arguments->matchedOnly = true;
      break;
    case 'p':
      arguments->prettyPrint = true;
      break;
    case ARGP_KEY_END:
      if (state->argc < 2) {
        /* No inputs. */
        argp_state_help(state, stdout, ARGP_HELP_STD_HELP);
        exit(0);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}