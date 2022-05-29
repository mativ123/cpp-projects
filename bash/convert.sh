#!/bin/bash

num=28

for (( i=1; i<$1; i++ ))
do
    ((num++))
    picI=$(( 1 + $RANDOM % 28 ))
    echo $picI

    brig=$(( 20 + $RANDOM % 150 ))
    sat=$(( 100 + $RANDOM % 200 ))
    hue=$(( 50 + $RANDOM % 200 ))

    convert "$2/$picI.jpg" -modulate $brig,$sat,$hue "$3/$num.jpg"
done

echo "done"
