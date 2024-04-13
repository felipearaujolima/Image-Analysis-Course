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

### *(function)* applyRGBtoYCbCrBT2020

### *(function)* applyYCbCrBT2020toRGB




# Image

# Adjacency

# MathMorph

# Kernel
