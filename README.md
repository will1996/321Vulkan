# 321Vulkan

## Overview:

321 Vulkan is the tool I wish I had when I got into rendering. It is a portable version of the Vulkan Tutorial, extended with an example for run-time vertex buffer modification, which can be extended more naturally into a first project. 

This project only really makes sense as a companion to the Vulkan tutorial, they did an awesome job, and I'm not here to re-invent the wheel, so if you're ever lost look there! 

The implementation is all in one file, and the structure is kept as close to the original tutorial as possible. That way, you can follow along with them, but use my project. 





## Setup:

The project structure, build files, and implementation are all set up for you. To use it simply:

Clone this repository to a local directory

Create a new sub-directory in that folder called "build"

enter the build folder on command line

type: 
cmake ..
then:
make

This will populate the build directory with an executable called "practice" if all goes well.

run ./practice, if a window pops up with a red rectangle, that then turns green and grows, then everything is set up properly! 

## Usage:
I've included a sample file called "basicVulkan.cpp" 
It is a working example of how to include the "basicRender" class, which contains the entire implementation. As well as examples of how to put shapes into the vertex buffer, and how to use run-time vertex buffer updating. 
Start there. 

Once you understand what it does, simply include basic render in any application that generates verticies, and you can have well-behaved real-time rendering in anything you want! 


## future changes:

I want to keep the original 321 Vulkan the same, but I plan on adding more complete renderers in the future with more features in the following order:

Uniform Buffers, 
Camera Support, 
Texture support, 
Lighting

With all that it will be a fairly complete Vulakn renderer, that can be dropped into any application easily! 

Happy rendering! 




