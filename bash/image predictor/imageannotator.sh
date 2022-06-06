#!/bin/bash
imagePath=$1
echo "loading $imagePath..."
number=$2
tempObject=$(python ai.py $imagePath)
object=$(echo "${tempObject%% *}" | sed 's/_/ /g')
imageW=$(identify -format '%[width]' $imagePath)
tempH=$(identify -format '%[height]' $imagePath)
while [ $(($tempH % 2)) != 0 ]
do
    imageH=$(($tempH - 1))
    tempH=$imageH
done
if [ -z ${imageH+x} ]; then
    imageH=$tempH
fi
echo "finished loading"

echo "annotating $imagePath"
convert -size ${imageW}x$(($imageH/6)) xc:white out.jpg
convert {out.jpg,$imagePath} -append out.jpg
convert out.jpg -fill black -pointsize $(($imageH/12)) -draw "text 0,$(($imageH/12)) '$object'" $number.jpg
echo "finished annotating"

echo "making audio"
tts --text "$object" --out_path ./$number.wav > /dev/null
echo "done"
# ffmpeg -loglevel error -framerate 1 -i out.jpg -c:v libx264 -r 30 temp.mp4
# ffmpeg -loglevel error -y -i temp.mp4 -i out.wav -shortest -vf scale=$imageW:$imageH out.mp4
# rm temp.mp4
