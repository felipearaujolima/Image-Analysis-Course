# Description  
In this file you will find every function that i used to test the theory behind image analysis. Given the importance of such, i was sure that i had to document it somehow. This is where this README comes to play, down below i describe what every function is doing and how (note that the *why* is answered in the theory file).  

# Topics 

- [Common's functions](#Common)

- [Image's functions](#Image)

- [Adjacency's functions](#Adjacency)

- [MathMorph's functions](#MathMorph)

- [Kernel's functions](#Kernel)

# Common
These funcions are common to all library, mainly used as a aux for the others, or to myself. 

### *(struct)* Voxel
Voxel is short for volume pixel, is a unit of graphic information that defines a point in three-dimensional space. Given that, i need x (height), y (widht) and z (depth) - i also have t as time, but i don't use that. So this structure is mainly because of that, a way to store the coordinates of a volume pixel. 

### *(struct)* IntArray
This one is for the background. It's only a structure that has the array (int*) and n. 

### *(struct)* Color 
Color can be see as RGB plus a alpha channel. What this means is: We have three luminosity channels (Red, Green, Blue) with color intensity given by 0 to 255. With that, we may also have a alpha channel which give us the opacity the pixel. See, every struct Voxel will have a struct color attachted to it. 

### *(function)* applylog
This one is pretty straightforward, is a mathamatical way of doing a log of a number, given certain base, using natural log. This one will come in hand when we work with bit depth.

### *(function)* CreateIntArray
Creates a struct of type IntArray. 

### *(function)* IntRange
This function creates an array of integers within a specified range and incremente. E.g. IntArray(0, 10, 2) -> [0, 2, 4, 6, 8, 10]

### *(function)* DestroyIntArray
Destroy a struct of type IntArray

### *(function)* applyFree
It's just free(pointer) guys... with the spice of working with NULL pointers as well. 

### *(function)* applyAlloc
It's just a calloc of n, and size size. So, we alloc memory for an array of n elements, each of size size bytes (all initialized as zero).

### *(function)* applyAllocIntArray
Using applyAlloc in a way that my n elements has size int

### *(function)* applyAllocFloatArray
Using applyAlloc in a way that my n elements has size float

### *(function)* applyAllocUShortArray
Using applyAlloc in a way that my n elements has size ushort

### *(function)* applyRGBtoYCbCr
This function helps the convertion of a image with color space RGB to YCbCr, remember that:  
* RGB: This is an additive color model in which the red, green and blue primary colors of light are added together in various ways to reproduce a broad array of colors. Each pixel in a image is represented as a combination of these three colors.  
* YCbCr: This is a family of color space used as a part of the image pipeline in video and digital photography systems. YCbCr is sometimes described as a "decorreleted" version of the RGB color space, since the three components have less redundancy and are more amenable to efficient coding.
    - Y stands for 'luma' which is the brightness in an image.
    - Cb stands for 'blue-difference choroma' component.
    - Cr stands for 'red difference choroma' component.
The YCbCr color model is derived from the RGB model and is widely used in digital imaging systems, digital video, and JPEG and MPEG compression schemes. The main advantange of YCbCr is that it separates the brightness (luminance) information (Y) from the color (chorominance) information (Cb and Cr), which allows for efficent compression. It's also designed to be compatible with black and white television and to provide consistent color representation across different devices.
Given all that, we always convert from RGB to YCbCr the images that we are working on. Even if the image is gray-level we can also to this convertion, with the Cb and Cr values being NULL.
It's important to say that if the image is a 8-bit image, normalization_value would be 255, and all output values would be in the range [0, 255].

### *(function)* applyRGBtoYCbCrBT2020
This function also convert RGB to YCbCr, however, this one is a more advanced and flexible version of the RGB to YCbCr convertion. It takes into account the bit depth of the input RGB values and the desired output YCbCr values. The bit depth is the number of bits used to represent each color component. Higher bits depths allows for more precise color representation. 

### *(function)* applyYCbCrBT2020toRGB
This one is to apply a convertion in a image, to go from YCbCr to RGB.  

# Image
These functions are for working with image, and by that i mean: Reading, Writing, Copying and Destroing a image. In here you will also find the structure of a image, and some global aux functions.

### *(struct)* Image
This structure is the key to all of what we are doing, it's where the image is gonna be allocated.  
* val -> is a array of values from 0 to n, having the luminance of the image (the image in here is a flat 1D array).
* Cb -> is a array of values from 0 to n, having the blue-difference choroma component of the image.
* Cr -> the same as the above, but for the red-difference choroma.
* Alpha -> the same as the above, but for the alpha component (opacity).
* Xsize -> the xsize of the image (here, we are treating the image as a 2D or 3D concept).
* Ysize -> the same as the above, for y.
* Zsize -> the same as the above, for z.
* dx, dy, dz -> the size of each voxel along the x, y and z dimensions.
* tby, tbz -> This two are used to speed up voxel access by storing pre-calculated values for the y and z parts of the 3D to 1D index conversion formula.

### *(define)* applyGetVoxelIndex
It's used to calculate the 1D index of the voxel in the 3D array.

### *(define)* isValidVoxel
Given a voxel index, we see if the index is bounded by the image dimensions.

### *(define)* CopyVoxelSize
Given two image, we copy the sizes of each voxel in image 1 to image 2.

### *(function)* applyNormalizationValue
This one is to apply the normalization in a value. So, we are clipping the value.

### *(function)* applyMinMaxValues
This one is to find the min and max values (luminance) of a image. 

### *(function)* applyImageDepth
This one is used to find the bit depth of a image. We first find the range of values in the image (255 if 8-bit), than we clip that to be inside certain known bits range (255, 4095..), and then we find the bit depth by applying the log. 

### *(function)* applyIsColorImage
This is a aux function to find out if the image is color.

### *(function)* applySetCbCr
Allocing a value into the Cb and Cr components.

### *(function)* applySetAlpha
Allocing a value into the alpha component.

### *(function)* MaxImageRange
This is the same as 2^bitdepth, to find the max image range.

### *(function)* applyGetVoxelCoord
Used to convert a 1D index into 3D coordinates in a voxel grid.

### *(function)* CopyCbCr
Used to copy the Cb and Cr components of a image into another.

### *(function)* CreateColorImage
Used to create an standard color image where the Cb and Cr components will have a standard value aswell.

### *(function)* CreateImageFromImage
Create a image with nothing in it, but with the same specs as another one.

### *(function)* CreateImage
This function first alloc memory for the voxels values, and calls another applyCreateImageFromBuffer. 

### *(function)* applyCreateImageFromBuffer
Given the memory for the pixels, this function allocs memory and values for the struct image.

### *(function)* ReadPngImageAux
This function uses the libpng strategy of reading png's images. The main goal of this function, is to read and have the row_pointers (pointers for each row of pixels in the image), for the input image.

### *(function)* ReadImage
ReadImage is one of the most important function in this library. First we get the row_pointers out of the ReadPngImageAux, then we get some infos about the image, such as height, color_type an depth, which is then used to create the image. Then, we have the major block code in the function:  
* If the image is gray_level: For each x and y, we get the pixel value using row_pointers (see that we have to add the number of channels and byteshift in here); then we alloc in the 1D array the value of this pixel (inside the val int* of the struct image). Note that if the depth is 16, then we have 16 bits for pixel, so we have to add the first 8 bit part and the second one.
* If the image is gray_level with alpha channel: In here, the only difference is that we have to alloc the values of the channel alpha aswell.
* If the image is RGB: In here, we first get the RGB componentes of each pixel inside the image, and then we convert this to YCbCr, which is then allocated inside the val, Cb and Cr arrays of the image.
* If the image is RGB with alpha channel: Reading RGB pixels with the alpha channel.  To end it all, we free the memory allocated in the row_pointers, and we destroy the png struct of libpng.  

### *(function)* DestroyImage
This is a fun one... freeing the memory of each component of the image, and the pointer of the image at the end.  

### *(function)* WritePngImageAux
In here, we first create the libpng structs necessary to write a png image. then we use all the info about the image, with the row_pointers, to write a png image. 

### *(function)* WriteImagePNG
The main goal of this function is to find the row_pointers of the image again, given the struct image. First we find the numbers of channels in the image, and then we alloc memomry and info in each row_pointer.  
* If the image is gray-level: We do the inverse of what we did to read the row_pointers into img -> val.
If the image is not gray-level, we do the inverse of each possibility, given the type of image, based on ReadImage. 

# Adjacency
In here, you find the functions necessary to work with adjacency in images. Not only a function to create a adjacency, but a rectangular and circular one. This block of functions is extremely important given the many uses that adjacency has on many image analysis's applications.

### *(struct)* AdjRel 
This is the major structure when working with adjacency. Here we have arrays of displacement for each dimension (x, y, z, t), and the number of adjacency voxels. The way we are working with adjacency, is by first creating the displacements, in a way that e.g. we have a rectangular adjacency with h = w = 3, then we will need to go from x = y = z = t = 0 (the center of the adjacency) to x = -2, x = -1, x = 1, x = 2 (the same for the others dimensions). So, we only alloc the displacements for the adjacency, and use that for every pixel, in a way that i can easily find his adjacents pixels.  

### *(function)* applyGetAdjacentVoxel
Given a voxel, a adjacency and the index of the adjacency (such as: If we have a rectangular adjacency of w = h = 3, then we have {0, 1, 2... 8} possibles adjacents voxels index), we find the adjacency voxel.  

### *(function)* CreateAdjRel
Here we are only allocating memory for the adjacency and its arrays.

### *(function)* DestroyAdjRel
Destroing the adjacency.

### *(function)* adjRectangular
Creating a rectangular adjacency. First finding n and then allocating the displacements given this shape. 

### *(function)* adjCircular
Creating a circular adjacency. First finding n and then allocating the displacements given this shape. 

# MathMorph

# Kernel
