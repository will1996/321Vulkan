//basicRender.cpp
#include "basicRender.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

#include <chrono>

using QueueFamilyIndices =  BasicRenderer::QueueFamilyIndices;
using SwapChainSupportDetails = BasicRenderer::SwapChainSupportDetails;

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VKAPI_ATTR VkBool32 VKAPI_CALL BasicRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


BasicRenderer::QueueFamilyIndices BasicRenderer::findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, d_surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

  bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
          requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
  }




BasicRenderer::BasicRenderer(std::vector<BasicRenderer::Vertex> verticies, 
        std::vector<uint32_t> indicies){
 #ifdef NDEBUG
 d_enableValidationLayers = false;
#else
 d_enableValidationLayers = true;
 std::cout<<"validation layers enabled" <<std::endl;
#endif

 d_verticies = verticies;
 d_indicies = indicies;
}

//default_constructor
BasicRenderer::BasicRenderer(){
  
#ifdef NDEBUG
 d_enableValidationLayers = false;
#else
 d_enableValidationLayers = true;
#endif

 d_verticies ={
    {{-0.5f, -0.5f,0.0f}, {1.0f, 0.0f, 0.0f},{1.0f,0.0f}},
    {{0.5f, -0.5f,0.0f}, {0.0f, 1.0f, 0.0f},{0.0f,0.0f}},
    {{0.5f, 0.5f,0.0f}, {0.0f, 0.0f, 1.0f},{0.0f,1.0f}},
    {{-0.5f, 0.5f,0.0f}, {1.0f, 1.0f, 1.0f},{1.0f,1.0f}},

    {{-0.5f, -0.5f,-0.5f}, {1.0f, 0.0f, 0.0f},{1.0f,0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f},{0.0f,0.0f}},
    {{0.5f, 0.5f,  -0.5f}, {0.0f, 0.0f, 1.0f},{0.0f,1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f},{1.0f,1.0f}}
};

d_indicies = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};
std::string rootDir = "/Users/willchambers/Projects/321Vulkan";

setTexturePath(rootDir+"/textures/chalet.jpg");
setModelPath(rootDir+"/models/chalet.obj");
}

BasicRenderer::~BasicRenderer(){

}

//publicly exposed setup and teardown functions for the renderer
void BasicRenderer::initialize(){
    initWindow();
    initVulkan();
}
void BasicRenderer::shutdown(){
  vkDeviceWaitIdle(d_device);
  cleanup();
}

GLFWwindow* BasicRenderer::getWindow(){
    return d_window;
}
void BasicRenderer::draw(){
    drawFrame();
}

void BasicRenderer::loadModel(){
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

     if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, d_modelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }
  d_verticies.clear();
  d_indicies.clear();
  for (const auto& shape : shapes){
    for (const auto& index : shape.mesh.indices){
      Vertex vertex = {};
      vertex.pos = {
        attrib.vertices[3*index.vertex_index +0],
        attrib.vertices[3*index.vertex_index +1],
        attrib.vertices[3*index.vertex_index +2] 
      };

      vertex.texCoord = {
        attrib.texcoords[2*index.texcoord_index +0],
        1.0f - attrib.texcoords[2*index.texcoord_index +1]
      };

      vertex.color = {1.0,1.0,1.0};

      d_verticies.push_back(vertex);
      d_indicies.push_back(d_indicies.size());
    } 
  }

}
void BasicRenderer::update(std::vector<Vertex> verticies, std::vector<uint16_t> indicies){
    if(verticies.size()!=d_verticies.size()) throw std::logic_error("cannot resize vertex buffer after creation");
    if(indicies.size()!=d_indicies.size()) throw std::logic_error("cannot resize index buffer after creation");

    for(size_t i=0;i<verticies.size();i++){
      d_verticies[i]= verticies[i]; 
    }
  updateVertexBuffer();
  updateIndexBuffer();
  }

void BasicRenderer::setTexturePath(std::string texturePath){
  d_texturePath = texturePath;
}
void BasicRenderer::setModelPath(std::string modelPath){
  d_modelPath = modelPath;
}


void BasicRenderer::createImage(uint32_t width, uint32_t height,VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage,VkMemoryPropertyFlags properties,VkImage & image, VkDeviceMemory &imageMemory){

  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = width; 
  imageInfo.extent.height = height;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.flags = 0;

  if(vkCreateImage(d_device, &imageInfo,nullptr,&image)!=VK_SUCCESS){
    throw std::runtime_error("failed to create exture image");
  }
  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(d_device, image, &memRequirements);

  VkMemoryAllocateInfo allocInfo = {};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,properties);
  if(vkAllocateMemory(d_device, &allocInfo,nullptr, &imageMemory)!=VK_SUCCESS){
      throw std::runtime_error("failed to allocate image memory") ;
  } 
   vkBindImageMemory(d_device, image, imageMemory,0);  


};

bool hasStencilComponent(VkFormat format){
  return format==VK_FORMAT_D32_SFLOAT_S8_UINT||format ==VK_FORMAT_D24_UNORM_S8_UINT;
}
void BasicRenderer::transitionImageLayout(VkImage image, VkFormat format,
        VkImageLayout oldLayout, VkImageLayout newLayout){
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    
    VkImageMemoryBarrier barrier = {};

    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    if(newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      if(hasStencilComponent(format)){
          barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;  
       }
    }
    else{
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }



    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer =0;
    barrier.subresourceRange.layerCount = 1;
    
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if(oldLayout== VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }else if(oldLayout== VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
        newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
              newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL){
    
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT|
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else{
    throw std::runtime_error("Unsupported image Layout transition");
    }

    vkCmdPipelineBarrier
      (commandBuffer,
      sourceStage,destinationStage,
      0,
      0,nullptr,
      0,nullptr,
      1,&barrier
       );


    endSingleTimeCommands(commandBuffer);

}
void BasicRenderer::copyBufferToImage(VkBuffer buffer, VkImage image,uint32_t width,uint32_t height){
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    VkBufferImageCopy region = {};

    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0,0,0};
    region.imageExtent = {
      width,
      height,
      1
    };
    vkCmdCopyBufferToImage(
       commandBuffer,
       buffer,
       image,
       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
       1,
       &region
        ); 
   endSingleTimeCommands(commandBuffer);

}
void BasicRenderer::createTextureImage(){
  int texWidth, texHeight, texChannels;
  std::string filepath = d_texturePath; 
  stbi_uc* pixels = stbi_load(filepath.c_str(),&texWidth, &texHeight, &texChannels,
      STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth*texHeight*4;//1 dimension for r, g, b, a
  if(!pixels){
    throw std::runtime_error("failed to load image from filepath "+filepath);
  }

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;

  void * data;
  createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,stagingBuffer, stagingBufferMemory);
  vkMapMemory(d_device, stagingBufferMemory, 0, imageSize, 0, &data);
      memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(d_device, stagingBufferMemory);
  stbi_image_free(pixels);
createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    d_textureImage, d_textureImageMemory);
  transitionImageLayout(d_textureImage,VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(stagingBuffer,d_textureImage,static_cast<uint32_t>(texWidth),
          static_cast<uint32_t>(texHeight));
  transitionImageLayout(d_textureImage,VK_FORMAT_R8G8B8A8_UNORM,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  vkDestroyBuffer(d_device,stagingBuffer,nullptr);
  vkFreeMemory(d_device,stagingBufferMemory,nullptr);
}

void BasicRenderer::createTextureImageView(){
  d_textureImageView = createImageView(d_textureImage,VK_FORMAT_R8G8B8A8_UNORM,VK_IMAGE_ASPECT_COLOR_BIT);
}
void BasicRenderer::createTextureSampler(){
  VkSamplerCreateInfo samplerInfo ={};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO; 
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = 16;

  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates =VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;


  if(vkCreateSampler(d_device,&samplerInfo,nullptr,&d_textureSampler)!=VK_SUCCESS){
    throw std::runtime_error("failed to create texture sampler");
  
  } 

}
void BasicRenderer::run(){

        initWindow();
        initVulkan();
        mainLoop();
        cleanup();

}
      


VkFormat BasicRenderer::findSupportedFormat(const std::vector<VkFormat> & candidates ,
    VkImageTiling tiling,VkFormatFeatureFlags features ){
  for(VkFormat format: candidates){
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(d_physicalDevice,format,&props);

    if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features)==features){
        return format;
    
    }else if(tiling==VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features)==features){
        return format; 
    }
  }
      throw std::runtime_error("failed to find depth buffer format");
}

VkFormat BasicRenderer::findDepthFormat(){
  return findSupportedFormat(
 {VK_FORMAT_D32_SFLOAT,VK_FORMAT_D32_SFLOAT_S8_UINT,VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

void BasicRenderer::createDepthResources(){
  VkFormat depthFormat = findDepthFormat();
  createImage(d_swapChainExtent.width, d_swapChainExtent.height,depthFormat,
        VK_IMAGE_TILING_OPTIMAL,VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,d_depthImage,d_depthImageMemory);
  d_depthImageView = createImageView(d_depthImage,depthFormat,VK_IMAGE_ASPECT_DEPTH_BIT);
  transitionImageLayout(d_depthImage,depthFormat,VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

}

void BasicRenderer::initVulkan(){

        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createDescriptorSetLayout();
        createGraphicsPipeline();
        createCommandPool();
        createDepthResources();
        createFramebuffers();
        createTextureImage();
        createTextureImageView();
        createTextureSampler();
        loadModel();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();

}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<BasicRenderer*>(glfwGetWindowUserPointer(window));
    app->d_framebufferResized = true;
}

void BasicRenderer::initWindow(){
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        d_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(d_window, this);
        glfwSetFramebufferSizeCallback(d_window, framebufferResizeCallback);
}

void BasicRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void BasicRenderer::createInstance(){
      if (d_enableValidationLayers && !checkValidationLayerSupport()) {
          throw std::runtime_error("validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo = {};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "Hello Triangle";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "No Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions = getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (d_enableValidationLayers) {
          createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
          createInfo.ppEnabledLayerNames = validationLayers.data();

          populateDebugMessengerCreateInfo(debugCreateInfo);
          createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
      } else {
          createInfo.enabledLayerCount = 0;

          createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &d_instance) != VK_SUCCESS) {
          throw std::runtime_error("failed to create instance!");
      }
      std::cout<<"Instance created, party on ted"<<std::endl;
}
void BasicRenderer::setupDebugMessenger(){

if (!d_enableValidationLayers) return;

VkDebugUtilsMessengerCreateInfoEXT createInfo;
populateDebugMessengerCreateInfo(createInfo);
VkResult result = CreateDebugUtilsMessengerEXT(d_instance, &createInfo, nullptr, &d_debugMessenger);
if (result != VK_SUCCESS) {
    std::cout<< result << std::endl;
    throw std::runtime_error("failed to set up debug messenger!");
}
}

void BasicRenderer::createSurface(){

if (glfwCreateWindowSurface(d_instance, d_window, nullptr, &d_surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
}

}


bool BasicRenderer::isDeviceSuitable(VkPhysicalDevice device) {
  QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device,&deviceFeatures);
    return indices.isComplete() && extensionsSupported && swapChainAdequate
      &&deviceFeatures.samplerAnisotropy;
}
void BasicRenderer::pickPhysicalDevice(){

uint32_t deviceCount = 0;
vkEnumeratePhysicalDevices(d_instance, &deviceCount, nullptr);

if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
}

std::vector<VkPhysicalDevice> devices(deviceCount);
vkEnumeratePhysicalDevices(d_instance, &deviceCount, devices.data());

for (const auto& device : devices) {
    if (isDeviceSuitable(device)) {
        d_physicalDevice = device;
        break;
    }
}

if (d_physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
}
}


void BasicRenderer::createLogicalDevice(){

        QueueFamilyIndices indices = findQueueFamilies(d_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (d_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(d_physicalDevice, &createInfo, nullptr, &d_device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(d_device, indices.graphicsFamily.value(), 0, &d_graphicsQueue);
        vkGetDeviceQueue(d_device, indices.presentFamily.value(), 0, &d_presentQueue);
    }


    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D BasicRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(d_window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    SwapChainSupportDetails BasicRenderer::querySwapChainSupport(VkPhysicalDevice device) { SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, d_surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, d_surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, d_surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, d_surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, d_surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

void BasicRenderer::createSwapChain(){
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(d_physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = d_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(d_physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(d_device, &createInfo, nullptr, &d_swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(d_device, d_swapChain, &imageCount, nullptr);
        d_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(d_device, d_swapChain, &imageCount, d_swapChainImages.data());

        d_swapChainImageFormat = surfaceFormat.format;
        d_swapChainExtent = extent;
    
}


VkImageView BasicRenderer::createImageView(VkImage image, VkFormat format,
    VkImageAspectFlags aspectFlags){
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;

        VkImageView imageView;
        if(vkCreateImageView(d_device,&viewInfo,nullptr,&imageView)!=VK_SUCCESS){
            throw std::runtime_error("failed to create image view ");
        }
       return imageView; 
}


void BasicRenderer::createImageViews(){

        d_swapChainImageViews.resize(d_swapChainImages.size());

        for (size_t i = 0; i < d_swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = d_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = d_swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(d_device, &createInfo, nullptr, &d_swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }

}

void BasicRenderer::createRenderPass(){
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = findDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = d_swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        
        std::array<VkAttachmentDescription,2> attachments = {colorAttachment,depthAttachment};
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(d_device, &renderPassInfo, nullptr, &d_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass!");
        }
}
    static std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }
    VkShaderModule BasicRenderer::createShaderModule(const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(d_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

void BasicRenderer::createDescriptorSetLayout(){
  VkDescriptorSetLayoutBinding uboLayoutBinding = {};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.descriptorCount =1;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding,2> bindings = {uboLayoutBinding,
    samplerLayoutBinding};
  VkDescriptorSetLayoutCreateInfo layoutInfo = {};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data(); 

  if(vkCreateDescriptorSetLayout(d_device, &layoutInfo,nullptr,&d_descriptorSetLayout)!=VK_SUCCESS){
    throw std::runtime_error("failed to create descriptor set layout");
  }
}
void BasicRenderer::createUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  d_uniformBuffers.resize(d_swapChainImages.size());
  d_uniformBuffersMemory.resize(d_swapChainImages.size());

  for (size_t i=0; i< d_swapChainImages.size();i++){
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        d_uniformBuffers[i],d_uniformBuffersMemory[i]);
  }

}
void BasicRenderer::createGraphicsPipeline(){

        auto vertShaderCode = readFile("../shaders/vert.spv");
        auto fragShaderCode = readFile("../shaders/frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) d_swapChainExtent.width;
        viewport.height = (float) d_swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = d_swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &d_descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        if (vkCreatePipelineLayout(d_device, &pipelineLayoutInfo, nullptr, &d_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
        
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = d_pipelineLayout;
        pipelineInfo.renderPass = d_renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.pDepthStencilState = &depthStencil;

        if (vkCreateGraphicsPipelines(d_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &d_graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(d_device, fragShaderModule, nullptr);
        vkDestroyShaderModule(d_device, vertShaderModule, nullptr);
}
void BasicRenderer::createFramebuffers(){

        d_swapChainFramebuffers.resize(d_swapChainImageViews.size());

        for (size_t i = 0; i < d_swapChainImageViews.size(); i++) {
          std::array<VkImageView,2> attachments = {
                d_swapChainImageViews[i],
                d_depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = d_renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = d_swapChainExtent.width;
            framebufferInfo.height = d_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(d_device, &framebufferInfo, nullptr, &d_swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }

}
void BasicRenderer::createCommandPool(){

        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(d_physicalDevice);

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(d_device, &poolInfo, nullptr, &d_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics command pool!");
        }


}
    uint32_t BasicRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(d_physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
    void BasicRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(d_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(d_device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(d_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(d_device, buffer, bufferMemory, 0);
    }
  
    VkCommandBuffer BasicRenderer::beginSingleTimeCommands(){
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = d_commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(d_device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
       
        return commandBuffer; 
    }



    void BasicRenderer::endSingleTimeCommands(VkCommandBuffer commandBuffer){
        vkEndCommandBuffer(commandBuffer);
    
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(d_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(d_graphicsQueue);

        vkFreeCommandBuffers(d_device, d_commandPool, 1, &commandBuffer);
    
    }



    void BasicRenderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

        VkBufferCopy copyRegion = {};
        copyRegion.size = size;
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        endSingleTimeCommands(commandBuffer);


    }


void BasicRenderer::createVertexBuffer(){

        VkDeviceSize bufferSize = sizeof(d_verticies[0]) * d_verticies.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(d_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, d_verticies.data(), (size_t) bufferSize);
        vkUnmapMemory(d_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, d_vertexBuffer, d_vertexBufferMemory);

        copyBuffer(stagingBuffer, d_vertexBuffer, bufferSize);

        vkDestroyBuffer(d_device, stagingBuffer, nullptr);
        vkFreeMemory(d_device, stagingBufferMemory, nullptr);
}

void BasicRenderer::updateVertexBuffer(){
        VkDeviceSize bufferSize = sizeof(d_verticies[0]) * d_verticies.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(d_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, d_verticies.data(), (size_t) bufferSize);
        vkUnmapMemory(d_device, stagingBufferMemory);

        copyBuffer(stagingBuffer, d_vertexBuffer, bufferSize);

        vkDestroyBuffer(d_device, stagingBuffer, nullptr);
        vkFreeMemory(d_device, stagingBufferMemory, nullptr);
}



void BasicRenderer::createIndexBuffer(){
        VkDeviceSize bufferSize = sizeof(d_indicies[0]) * d_indicies.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(d_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, d_indicies.data(), (size_t) bufferSize);
        vkUnmapMemory(d_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, d_indexBuffer, d_indexBufferMemory);

        copyBuffer(stagingBuffer, d_indexBuffer, bufferSize);

        vkDestroyBuffer(d_device, stagingBuffer, nullptr);
        vkFreeMemory(d_device, stagingBufferMemory, nullptr);
}

void BasicRenderer::updateIndexBuffer(){
    //TODO
}

void BasicRenderer::createCommandBuffers(){
        d_commandBuffers.resize(d_swapChainFramebuffers.size());
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = d_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) d_commandBuffers.size();

        if (vkAllocateCommandBuffers(d_device, &allocInfo, d_commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < d_commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(d_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = d_renderPass;
            renderPassInfo.framebuffer = d_swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = d_swapChainExtent;

            std::array<VkClearValue,2> clearValues = {};
              clearValues[0].color =  {0.0f, 0.0f, 0.0f, 1.0f};
              clearValues[1].depthStencil =  {1.0f,0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(d_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                vkCmdBindPipeline(d_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, d_graphicsPipeline);

                VkBuffer vertexBuffers[] = {d_vertexBuffer};
                VkDeviceSize offsets[] = {0};
                vkCmdBindVertexBuffers(d_commandBuffers[i], 0, 1, vertexBuffers, offsets);

                vkCmdBindIndexBuffer(d_commandBuffers[i], d_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
                
                vkCmdBindDescriptorSets(d_commandBuffers[i],VK_PIPELINE_BIND_POINT_GRAPHICS, d_pipelineLayout, 
                    0, 1, &d_descriptorSets[i],0, nullptr);

                vkCmdDrawIndexed(d_commandBuffers[i], static_cast<uint32_t>(d_indicies.size()), 1, 0, 0, 0);

            vkCmdEndRenderPass(d_commandBuffers[i]);

            if (vkEndCommandBuffer(d_commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }

}
void BasicRenderer::createSyncObjects(){

        d_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        d_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        d_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        d_imagesInFlight.resize(d_swapChainImages.size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(d_device, &semaphoreInfo, nullptr, &d_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(d_device, &semaphoreInfo, nullptr, &d_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(d_device, &fenceInfo, nullptr, &d_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
}

void BasicRenderer::drawFrame(){

        vkWaitForFences(d_device, 1, &d_inFlightFences[d_currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(d_device, d_swapChain, UINT64_MAX, d_imageAvailableSemaphores[d_currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        if (d_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(d_device, 1, &d_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        d_imagesInFlight[imageIndex] = d_inFlightFences[d_currentFrame];
        
        //updateVertexBuffer();
        updateUniformBuffer(imageIndex);
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {d_imageAvailableSemaphores[d_currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &d_commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {d_renderFinishedSemaphores[d_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(d_device, 1, &d_inFlightFences[d_currentFrame]);

        if (vkQueueSubmit(d_graphicsQueue, 1, &submitInfo, d_inFlightFences[d_currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR d_swapChains[] = {d_swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = d_swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(d_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || d_framebufferResized) {
            d_framebufferResized = false;
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        d_currentFrame = (d_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
void BasicRenderer::mainLoop(){

        while (!glfwWindowShouldClose(d_window)) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(d_device);
}

void BasicRenderer::cleanup(){
cleanupSwapChain();
        vkDestroySampler(d_device,d_textureSampler,nullptr);
        vkDestroyImageView(d_device,d_textureImageView,nullptr);
        vkDestroyImage(d_device,d_textureImage,nullptr);
        vkFreeMemory(d_device,d_textureImageMemory,nullptr);
        vkDestroyDescriptorSetLayout(d_device, d_descriptorSetLayout, nullptr);
        
        vkDestroyBuffer(d_device, d_indexBuffer, nullptr);
        vkFreeMemory(d_device, d_indexBufferMemory, nullptr);

        vkDestroyBuffer(d_device, d_vertexBuffer, nullptr);
        vkFreeMemory(d_device, d_vertexBufferMemory, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(d_device, d_renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(d_device, d_imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(d_device, d_inFlightFences[i], nullptr);
        }

        vkDestroyCommandPool(d_device, d_commandPool, nullptr);

        vkDestroyDevice(d_device, nullptr);

        if (d_enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(d_instance, d_debugMessenger, nullptr);
        }

        vkDestroySurfaceKHR(d_instance, d_surface, nullptr);
        vkDestroyInstance(d_instance, nullptr);

        glfwDestroyWindow(d_window);

        glfwTerminate();
}

void BasicRenderer::cleanupSwapChain(){

        vkDestroyImageView(d_device, d_depthImageView, nullptr);
        vkDestroyImage(d_device, d_depthImage, nullptr);
        vkFreeMemory(d_device, d_depthImageMemory, nullptr);

        for (auto framebuffer : d_swapChainFramebuffers) {
            vkDestroyFramebuffer(d_device, framebuffer, nullptr);
        }

        vkFreeCommandBuffers(d_device, d_commandPool, static_cast<uint32_t>(d_commandBuffers.size()), d_commandBuffers.data());

 
        vkDestroyPipeline(d_device, d_graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(d_device, d_pipelineLayout, nullptr);
        vkDestroyRenderPass(d_device, d_renderPass, nullptr);

        for (auto imageView : d_swapChainImageViews) {
            vkDestroyImageView(d_device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(d_device, d_swapChain, nullptr);
        for(size_t i=0;i<d_swapChainImages.size();i++){
          vkDestroyBuffer(d_device,d_uniformBuffers[i],nullptr);
          vkFreeMemory(d_device,d_uniformBuffersMemory[i],nullptr);
        }
        vkDestroyDescriptorPool(d_device, d_descriptorPool, nullptr);
}


void BasicRenderer::updateUniformBuffer(uint32_t currentImage){
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                                                    currentTime-startTime).count();
  UniformBufferObject ubo;
  ubo.model = glm::rotate(glm::mat4(1.0f),time*glm::radians(90.0f),
                                  glm::vec3(0.0f,0.0f,1.0f));
  ubo.view = glm::lookAt(glm::vec3(2.0f,2.0f,2.0f),glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,0.0f,1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),d_swapChainExtent.width/(float) d_swapChainExtent.height,0.1f,10.0f);
  
  ubo.proj[1][1] *=-1;

  void* data;
  vkMapMemory(d_device, d_uniformBuffersMemory[currentImage],0,sizeof(ubo),0,&data);
    memcpy(data,&ubo,sizeof(ubo));
  vkUnmapMemory(d_device, d_uniformBuffersMemory[currentImage]);
}

void BasicRenderer::createDescriptorPool(){
    std::array<VkDescriptorPoolSize,2> poolSizes = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(d_swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(d_swapChainImages.size());
    

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(d_swapChainImages.size());


    if(vkCreateDescriptorPool(d_device, &poolInfo,nullptr, &d_descriptorPool)!=VK_SUCCESS){
      throw std::runtime_error("failed to create descriptor pool");
    }
    

}
void BasicRenderer::createDescriptorSets(){
    std::vector<VkDescriptorSetLayout> layouts(d_swapChainImages.size(), d_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = d_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(d_swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    d_descriptorSets.resize(d_swapChainImages.size());

    if(vkAllocateDescriptorSets(d_device, &allocInfo, d_descriptorSets.data())!=VK_SUCCESS){
      throw std::runtime_error("failed to allocate descriptor sets");
    }

    for(size_t i=0;i<d_swapChainImages.size();i++){
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = d_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);
        
        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = d_textureImageView;
        imageInfo.sampler = d_textureSampler;

        std::array<VkWriteDescriptorSet,2> writeInfos = {};
        writeInfos[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfos[0].dstSet = d_descriptorSets[i];
        writeInfos[0].dstBinding = 0;
        writeInfos[0].dstArrayElement = 0;
        writeInfos[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeInfos[0].descriptorCount = 1;
        writeInfos[0].pBufferInfo = &bufferInfo;

        writeInfos[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeInfos[1].dstSet = d_descriptorSets[i];
        writeInfos[1].dstBinding = 1;
        writeInfos[1].dstArrayElement = 0;
        writeInfos[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeInfos[1].descriptorCount = 1;
        writeInfos[1].pImageInfo = &imageInfo;
      
        vkUpdateDescriptorSets(d_device, static_cast<uint32_t>(writeInfos.size()),
            writeInfos.data() , 0, nullptr);

    }


}
bool BasicRenderer::checkValidationLayerSupport(){

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;


}
void BasicRenderer::recreateSwapChain(){
        int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(d_window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(d_device);

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createDepthResources();
        createFramebuffers();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
}
std::vector<const char*> BasicRenderer::getRequiredExtensions(){
 uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (d_enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions; 

}
