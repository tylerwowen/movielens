#!/bin/sh

#  experiement.sh
#  moivelens
#
#  Created by Tyler Weimin Ouyang on 3/25/16.
#  Copyright © 2016 Tyler Ouyang. All rights reserved.

TIMEFORMAT=%R

DATA_ROOT="./data/"

if [ $1 = "ignore" ]; then
  OUTPUT_ROOT="./raw_results_ignore/"
  EXECUTABLE="./movielens_ignore.exec"
  PLOT="st ./plot.r ignore"
else
  OUTPUT_ROOT="./raw_results/"
  EXECUTABLE="./movielens.exec"
  PLOT="st ./plot.r"
fi


DATA_SETS="ml-100k ml-1m"
#DATA_SETS="ml-1m"
MOVIES=(1682 3952)
USERS=(1000 6040)

INDEX=0
for set in $DATA_SETS; do
  DATA_DIR=$DATA_ROOT$set
  OUTPUT=$OUTPUT_ROOT${set}_all_included.txt
  echo "k method num_movies num_users base test MAE RMSE Recall time matched_only" > $OUTPUT
  for i in 1 2 3 4 5; do
    TEST=${DATA_DIR}/u$i.test
    BASE=${DATA_DIR}/u$i.base
    for method in 0 1 2 3; do
      for k in 5 10 20; do
        for r in 1 2 3; do
          command1="$EXECUTABLE -k$k -d$method -m${MOVIES[$INDEX]} -u${USERS[$INDEX]} -r$BASE -t$TEST"
          echo $command1
          echo "$k $method ${MOVIES[$INDEX]} ${USERS[$INDEX]} u$i.base u$i.test $( { time $command1; } 2>&1) 0" >> $OUTPUT &
        done
        wait
      done
    done
  done
  let INDEX=INDEX+1
done

INDEX=0
for set in $DATA_SETS; do
 DATA_DIR=$DATA_ROOT$set
 OUTPUT=$OUTPUT_ROOT${set}_matched_only.txt
 echo "k method num_movies num_users base test MAE RMSE Recall time matched_only" > $OUTPUT
 for i in 1 2 3 4 5; do
   TEST=${DATA_DIR}/u$i.test
   BASE=${DATA_DIR}/u$i.base
   for method in 0 1 2 3; do
     for k in 5 10 20; do
       for r in 1 2 3; do
         command2="$EXECUTABLE -k$k -d$method -m${MOVIES[$INDEX]} -u${USERS[$INDEX]} -r$BASE -t$TEST -o"
         echo $command2
         echo "$k $method ${MOVIES[$INDEX]} ${USERS[$INDEX]} u$i.base u$i.test $( { time $command2; } 2>&1) 1" >> $OUTPUT &
       done
       wait
     done
   done
 done
 let INDEX=INDEX+1
done

./plot.r
