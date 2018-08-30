#!/bin/sh
for i in cough laugh
do
    touch $i.raw
    SZ=`wc -c $i.raw | cut -d' ' -f1`
    while [ "$SZ" -lt 300000000 ]
    do
        cat $i/*.raw >> $i.raw
        SZ=`wc -c $i.raw | cut -d' ' -f1`
    done
done
