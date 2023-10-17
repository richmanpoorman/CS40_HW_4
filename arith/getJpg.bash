IMAGE_PATH="/comp/40/bin/images/"
IMAGES=("flowers" "animals" "desert" "erosion" "from-wind-cave" \
        "halligan") 
OUTPUT_PATH="Test/InputFiles/"

for image in "${IMAGES[@]}"; do
        touch $OUTPUT_PATH$image.ppm
        djpeg $IMAGE_PATH$image.jpg > $OUTPUT_PATH$image.ppm
done