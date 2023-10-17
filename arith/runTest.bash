 #! /bin/sh

IMAGE_PATH="/comp/40/bin/images/"
IMAGES=("flowers" "animals" "desert" "erosion" "from-wind-cave" \
        "halligan") 
OUTPUT_PATH="Test/OutputFiles/"


jpegSelfCompare() {
        cjpeg $1 | djpeg -pnm | ./ppmdiff $1 -
}

comp40SelfCompare() {
        ./testSteps $1 | ./ppmdiff $1 -
}

jpegComp40Compare() {
        touch $1.cs40Out
        touch $1.jpegOut
        ./testSteps $1 > $1.cs40Out
        cjpeg $1 | djpeg > $1.jpegOut
        ./ppmdiff $1.cs40Out $1.jpegOut
        rm $1.cs40Out
        rm $1.jpegOut
}

for image in "${IMAGES[@]}"; do
        
        djpeg $IMAGE_PATH$image.jpg > $OUTPUT_PATH$image.out
        
        echo $image
        echo "---------------------"

        echo JPEG vs Original
        jpegSelfCompare $OUTPUT_PATH$image.out

        echo Our Implementation vs Original
        comp40SelfCompare $OUTPUT_PATH$image.out

        echo JPEG vs Our Implementation
        jpegComp40Compare $OUTPUT_PATH$image.out

        echo 
        rm $OUTPUT_PATH$image.out
        
done