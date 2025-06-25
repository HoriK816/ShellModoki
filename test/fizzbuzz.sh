#!/usr/bin/bash

i=0
while [ $i -le 30 ]
do

    if [ $((i%15)) -eq 0 ]; then
        echo "FizzBuzz"

    elif [ $((i%3)) -eq 0 ]; then
        echo "Fizz"

    elif [ $((i%5)) -eq 0 ]; then 
        echo "Buzz"
    else
        echo $i
    fi
    
    ((i++))
done
