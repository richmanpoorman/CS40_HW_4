 #! /bin/sh

IMAGE_PATH="Test/InputFiles/"
# Removed Tests:
# "empty" "one_by_one" "singlePixel" "verticalLongBar" "horizontalLongBar"
# "mobo"
IMAGES=("flowers" "animals" "desert" "erosion" "from-wind-cave" \
        "halligan"  "twoByTwo" \
        "nOddMEven" "nOddMOdd"  "nEvenMEven" "nEvenMOdd" \
        "blackSquare" "whiteSquare" "redSquare" "greenSquare" "blueSquare" \
        "colorBar" "evilDitherTest" "gradient" \
        ) 
OUTPUT_PATH="Test/OutputFiles/"


valgrindStep() {
        touch $OUTPUT_PATH$imageOutput.valgrind
        valgrind ./testSteps $1 > $OUTPUT_PATH$imageOutput.valgrind
        rm $OUTPUT_PATH$imageOutput.valgrind
}

jpegSelfCompare() {
        cjpeg $1 | djpeg -pnm | ./ppmdiff $1 -
}

comp40SelfCompare() {
        touch $OUTPUT_PATH$imageOutput.ppm
        ./testSteps $1 > $OUTPUT_PATH$imageOutput.ppm
        ./ppmdiff $1 $OUTPUT_PATH$imageOutput.ppm
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
        
        cat $IMAGE_PATH$image.ppm > $OUTPUT_PATH$image.out
        
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