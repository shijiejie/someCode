#!/bin/sh

board_width=9
board_height=6
num_imgs=43
square_size=23.8
imgs_left="./images/left/"
imgs_right="./images/right/"
imgs_directory="./images/"

#imgs_filename="left"
#output_filename="cam_left.yml"

#./calibrate -w ${board_width} -h ${board_height} -n ${num_imgs} -s ${square_size} -d ${imgs_left} -i "chessboard-L" -o "cam_left.yml" -e "png"

#./calibrate -w ${board_width} -h ${board_height} -n ${num_imgs} -s ${square_size} -d ${imgs_right} -i "chessboard-R" -o "cam_right.yml" -e "png"

#./calibrate_stereo -n ${num_imgs} -u "cam_left.yml" -v "cam_right.yml" -L ${imgs_left} -R ${imgs_right} -l chessboard-L -r chessboard-R -o cam_stereo.yml -e "png"

./undistort_rectify -l "./images/left/chessboard-L0.png" -r "./images/right/chessboard-R0.png" -c "./cam_stereo.yml" -L "./rechessboard-L0.png" -R "./rechessboard-R0.png"

