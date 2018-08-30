#!/bin/sh
set -e
[ -d TSPspeech ] || (
    mkdir -p TSPspeech
    cd TSPspeech
    7z x ../TSPspeech.iso
)
for i in `seq -w 01 10`
do
    for j in TSPspeech/48k/*/*$i.wav
    do
        [ ! -e "$j" ] || ffmpeg -i "$j" -f s16le -ac 1 -ar 48000 -
    done
done > TSPspeech.raw
