#!/bin/sh

#  experiement.sh
#  moivelens
#
#  Created by Tyler Weimin Ouyang on 3/25/16.
#  Copyright © 2016 Tyler Ouyang. All rights reserved.

TIMEFORMAT=%R

DATA_ROOT="./data/"
EXECUTABLE="./movielens"

# DATA_SETS="ml-100k ml-1m"
DATA_SETS="ml-1m"
MOVIES=(1682 3952)
USERS=(1000 6040)

# INDEX=0
# for set in $DATA_SETS; do
#   DATA_DIR=$DATA_ROOT$set
#   OUTPUT=${set}_all_included.txt
#   echo "k method num_movies num_users base test MAE RMSE Recall time matched_only" > $OUTPUT
#   for i in 1 2 3 4 5; do
#     TEST=${DATA_DIR}/u$i.test
#     BASE=${DATA_DIR}/u$i.base
#     for method in 0 1 2 3; do
#       for k in 5 10 20; do
#         for r in 1 2 3; do
#           command="$EXECUTABLE -k$k -d$method -m${MOVIES[$INDEX]} -u${USERS[$INDEX]} -r$BASE -t$TEST"
#           echo $command
#           result=$( { time $command; } 2>&1)
#           echo "$k $method ${MOVIES[$INDEX]} ${USERS[$INDEX]} u$i.base u$i.test $result 0" >> $OUTPUT
#         done
#       done
#     done
#   done
#   let INDEX=INDEX+1
# done

INDEX=1
for set in $DATA_SETS; do
  DATA_DIR=$DATA_ROOT$set
  OUTPUT=${set}_matched_only.txt
  echo "k method num_movies num_users base test MAE RMSE Recall time matched_only" > $OUTPUT
  for i in 1 2 3 4 5; do
    TEST=${DATA_DIR}/u$i.test
    BASE=${DATA_DIR}/u$i.base
    for method in 0 1 2 3; do
      for k in 5 10 20; do
        for r in 1 2 3; do

          command2="$EXECUTABLE -k$k -d$method -m${MOVIES[$INDEX]} -u${USERS[$INDEX]} -r$BASE -t$TEST -o"
          echo $command2
          result2=$( { time $command2; } 2>&1)
          echo "$k $method ${MOVIES[$INDEX]} ${USERS[$INDEX]} u$i.base u$i.test $result2 1" >> $OUTPUT
        done
      done
    done
  done
  let INDEX=INDEX+1
done

./plot.r
