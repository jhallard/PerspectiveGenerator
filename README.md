PerspectiveGenerator
====================

This program was developed in aid of the goal of implementing Monte Carlo Localization for robots inside of a 3D space using image data. To complete this goal, one of the things we needed to do was render images from thousands of different perspectives within a 3D model that was made with a 3D imaging camera (from Matterport Inc.). These images would be run through various feature detection algorithms and the results catalogued for use by future programs. 

This program works as follows.

1.) Takes in an input file specifying a few parameters

    a.) Path to directory containing 3D model and texture files

    b.) Name of .obj file containing 3D model

    c.) A List of [x, y, z, viewx, viewy, viewz] coordinates from which images should be rendered

        * viewx/y/z are component of the vector that the camera is looking along from the [x,y,z] coordinates.

2.) This program goes along the list and renders an image from each perspective listed.

    a.) Each image is grouped in a simple data structure that contains the image and the [x,y,z,viewx,viewy,viewz] coordinates of the perspective.

3.) These data structures are saved to a local directory for future processing.

Example .obj and .mtl files have been included with the project. Input.txt and Input2.txt are two sample input files that should compile right from download.
