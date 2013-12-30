#!/bin/bash

for i in `ls cases/*.a` cases/BADA
do
	cat $i | tr -d ' ' > $i.tr
done

