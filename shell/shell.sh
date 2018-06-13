#!/bin/bash
#####求数列的第100项
array[0] = 1
array[1] = 2
for((i=2;i<=99;++i))
do
    array[$i] = `expr${array[$i-1]} + ${array[$i-2]}`
done

echo ${array[99]}
