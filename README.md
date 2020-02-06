###321 Vulkan###
This is a learning project for graphics programming, it is also a complete version of "The Vulkan Tutorial" (https://vulkan-tutorial.com) and as such 
has many code similarities to that project. 

In addition to those similarities there are the following differences:

I wrote the example into a class called "basicRenderer" which is separated into a single header- implementation pair. Doing this allows the renderer to be 
included anywhere, and used to draw whatever models you want. 

Additionally, basicRenderer exposes the "update vertex buffer" and "update index buffer" methods, which allow dynamice modification of the vertex and index buffers. 
a topic that is not covered in the Vulkan Tutorial. 

If you're curious about how to do that, this is a decent place to look, and "basicVulkan.cpp " is a decent example. 


This project Demonstrates the following ideas from graphics programming:
  Indexed Drawing
  Staging Buffers
  The Graphics Pipeline
  Shader programming
  Uniform buffers
  Texture sampling/mapping
  Depth testing

If you're just starting out with graphics programming as I was, I would reccomend Dave Mount from UMD's notes on the topic, his lectures cover openGL, but the 
concepts transfer, and make the whole prospect much less overwhelming. Eight months after starting with the vulkan tutorial I think I'm finally starting to get it!




