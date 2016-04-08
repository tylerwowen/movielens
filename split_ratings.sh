#!/bin/sh

trap `rm -f tmp_base.$$ tmp_test.$$; exit 1` 1 2 15
RATING_DATA="ratings_shuffled.dat"

if [ ! -f $RATING_DATA ]; then
  cat ratings.dat | perl -MList::Util=shuffle -wne 'print shuffle <>;' > $RATING_DATA
fi

RATINGS_COUNT=`wc -l ratings_shuffled.dat | xargs | cut -d ' ' -f 1`
echo "ratings count: $RATINGS_COUNT"
SET_SIZE=`expr $RATINGS_COUNT / 5`
echo "set size: $SET_SIZE"
REMAINDER=`expr $RATINGS_COUNT % 5`
echo "remainder: $REMAINDER"

for i in 1 2 3 4 5
  do
    head -`expr $i \* $SET_SIZE` $RATING_DATA | tail -$SET_SIZE > tmp_test.$$

    # XXX: OSX users will see the message "head: illegal line count -- 0" here,
    #      but this is just a warning; the script still works as intended.
    head -`expr \( $i - 1 \) \* $SET_SIZE` $RATING_DATA > tmp_base.$$
    tail -`expr \( 5 - $i \) \* $SET_SIZE` $RATING_DATA >> tmp_base.$$


    if [ $i -eq 5 ]; then
       tail -$REMAINDER $RATING_DATA >> tmp_test.$$
    else
       tail -$REMAINDER $RATING_DATA >> tmp_base.$$
    fi
    sed -e "s/::/ /g" tmp_test.$$ | sort -k 1,1n -k 2,2n > u$i.test
    sed -e "s/::/ /g" tmp_base.$$ | sort -k 1,1n -k 2,2n > u$i.base
    rm tmp_base.$$ tmp_test.$$

    echo "u$i.test created.  `wc -l u$i.test | xargs | cut -d " " -f 1` lines."
    echo "u$i.base created.  `wc -l u$i.base | xargs | cut -d " " -f 1` lines."
done

./allbut.pl ra 1 10 0 $RATING_DATA
echo "ra.test created.  `wc -l ra.test | xargs | cut -d " " -f 1` lines."
echo "ra.base created.  `wc -l ra.train | xargs | cut -d " " -f 1` lines."

./allbut.pl rb 11 20 0 $RATING_DATA
echo "rb.test created.  `wc -l rb.test | xargs | cut -d " " -f 1` lines."
echo "rb.base created.  `wc -l rb.train | xargs | cut -d " " -f 1` lines."
