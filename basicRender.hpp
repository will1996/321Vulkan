//basicRender.hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include<string>
#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <vector>



class BasicRenderer{
  public: 

struct UniformBufferObject{
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);


        return attributeDescriptions;
    }
};
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
    struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    };
    BasicRenderer();
    BasicRenderer(std::vector<Vertex> verticies, std::vector<uint32_t> indicies);
    ~BasicRenderer(); 
    void initialize();
    void shutdown();
    void update(std::vector<Vertex> verticies, std::vector<uint16_t> indicies);
    void draw();//publicly exposed draw frame method
    GLFWwindow* getWindow();
    void run();
    bool d_framebufferResized = false;
    VkShaderModule createShaderModule(const std::vector<char>& code);
    void setTexturePath(std::string texturePath);
    void setModelPath(std::string modelPath);
  private:
    std::string d_texturePath;
    std::string d_modelPath;
    std::vector<uint32_t> d_indicies;
    std::vector<Vertex> d_verticies; 
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    bool d_enableValidationLayers;
    GLFWwindow* d_window;

    VkInstance d_instance;
    VkDebugUtilsMessengerEXT d_debugMessenger;
    VkSurfaceKHR d_surface;

    VkPhysicalDevice d_physicalDevice = VK_NULL_HANDLE;
    VkDevice d_device;

    VkQueue d_graphicsQueue;
    VkQueue d_presentQueue;

    VkSwapchainKHR d_swapChain;
    std::vector<VkImage> d_swapChainImages;
    VkFormat d_swapChainImageFormat;
    VkExtent2D d_swapChainExtent;
    std::vector<VkImageView> d_swapChainImageViews;
    std::vector<VkFramebuffer> d_swapChainFramebuffers;

    VkRenderPass d_renderPass;
    VkDescriptorSetLayout d_descriptorSetLayout;
    VkPipelineLayout d_pipelineLayout;
    VkPipeline d_graphicsPipeline;

    VkCommandPool d_commandPool;

    VkBuffer d_vertexBuffer;
    VkDeviceMemory d_vertexBufferMemory;
    VkBuffer d_indexBuffer;
    VkDeviceMemory d_indexBufferMemory;
    
    std::vector<VkBuffer> d_uniformBuffers;
    std::vector<VkDeviceMemory> d_uniformBuffersMemory;
    VkDescriptorPool d_descriptorPool;
    std::vector<VkDescriptorSet> d_descriptorSets;

    VkImage d_textureImage;
    VkDeviceMemory d_textureImageMemory;
    VkImageView d_textureImageView;
    VkSampler   d_textureSampler;
    
    VkImage d_depthImage;
    VkDeviceMemory d_depthImageMemory;
    VkImageView d_depthImageView;


    std::vector<VkCommandBuffer> d_commandBuffers;
  
    std::vector<VkSemaphore> d_imageAvailableSemaphores;
    std::vector<VkSemaphore> d_renderFinishedSemaphores;
    std::vector<VkFence> d_inFlightFences;
    std::vector<VkFence> d_imagesInFlight;

    size_t d_currentFrame = 0;

    
    void initVulkan();
      void initWindow();
      void createInstance();
      void setupDebugMessenger(); 
      void createSurface();
      void pickPhysicalDevice();
      void createLogicalDevice();
      void createSwapChain();
      void createImageViews();
      void createRenderPass();
      void createDescriptorSetLayout();
      void createGraphicsPipeline();
      void createFramebuffers();
      void createCommandPool();
      void createDepthResources();
      void createTextureImage();
      void createTextureImageView();
      void createTextureSampler();
      void loadModel();
      void createVertexBuffer();
      void createIndexBuffer();
      void createUniformBuffers();
      void createDescriptorPool();
      void createDescriptorSets();
      void createCommandBuffers();
      void createSyncObjects();

    void mainLoop();
      void drawFrame();

    void cleanup();
      void cleanupSwapChain();
    


    void updateVertexBuffer();
    void updateIndexBuffer();
    void updateUniformBuffer(uint32_t currentImage);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void createImage(uint32_t width, uint32_t height,VkFormat format, VkImageTiling tiling,
        VkImageUsageFlags usage,VkMemoryPropertyFlags properties,VkImage & image, VkDeviceMemory &imageMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void recreateSwapChain();
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer );


    VkFormat findSupportedFormat(const std::vector<VkFormat> & ,VkImageTiling ,VkFormatFeatureFlags );
    VkFormat findDepthFormat();
    void transitionImageLayout(VkImage,VkFormat,VkImageLayout,VkImageLayout);
    void copyBufferToImage(VkBuffer , VkImage,uint32_t width,uint32_t height);
    VkImageView createImageView(VkImage, VkFormat,VkImageAspectFlags);
    
    
    
    

};
