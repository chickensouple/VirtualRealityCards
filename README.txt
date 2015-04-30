Dependencies
______________
This project depends on two external libraries
opencv 2.4
boost 1.54 (program_options)

On Ubuntu 14.04, these can both easily be installed through a package manager such as synaptics. The packages to download are
libopencv-dev
libboost-all-dev

This project also uses c++11, and was tested with g++ version 4.8.2


How to build
______________
run make to make binaries
the binaries will be in the build/ folder
there is only one currently that does anything (main)


How to run program
_______________
to test, you can run build/main with no arguments and it should pop out two windows. One that shows the tag image with circles on the corners, and one that shows it overlayed with a projected image and box

You can change the tag image used by using the -t option to specify a path to another tag image

You can change the projected image used by using the -i option to specify a path to another image

Images are in the images/ directory
all tag images are imX.jpg
there are two test display images: chicken.jpg and endeavor.jpg

im2.jpg and im4.jpg have not correct blobs detected so they will mess up
im1, im3, and im5 will work fine



If you want to take pictures of the tag to test, a pdf of the tag is included as tag.pdf
Note: hsv values may need to be tuned for different lighting conditions.


