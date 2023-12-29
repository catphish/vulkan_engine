#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include "../system.h"

#define MAX_FRAMES_IN_FLIGHT 2
#define MAX_PIPELINES 32

struct vulkan {
  xcb_connection_t    *connection;
  xcb_screen_t        *screen;
  xcb_window_t         window;
  xcb_gcontext_t       gcontext;
  VkInstance           instance;
  VkSurfaceKHR         surface;
  VkPhysicalDevice     physicalDevice;
  int                  queueFamilyIndex;
  uint32_t             xcb_delete_window;
  VkDevice device;
  VkQueue queue;
  VkExtent2D           extent;

  VkSwapchainKHR swapChain;
  uint32_t swapChainImageCount;
  VkImage* swapChainImages;
  VkImageView* swapChainImageViews;
  VkFramebuffer* swapChainFramebuffers;

  VkRenderPass renderPass;

  VkCommandPool commandPool;
  VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];
  VkSemaphore imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
  VkSemaphore renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
  VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];

  VkImage depthImage;
  VkDeviceMemory depthImageMemory;
  VkImageView depthImageView;

  int currentFrame;

  VkPipelineLayout pipelineLayout;
  int pipelineCount;
  VkPipeline pipelines[MAX_PIPELINES];
};

struct vulkan *vulkan_create();
void vulkan_destroy(struct vulkan *vulkan);

void vulkan_surface_create(struct vulkan *vulkan);
void vulkan_window_create(struct vulkan *vulkan);
void vulkan_instance_create(struct vulkan *vulkan);
void vulkan_device_select(struct vulkan *vulkan);
uint8_t vulkan_window_poll(struct vulkan *vulkan);
void vulkan_device_create(struct vulkan *vulkan);
void vulkan_swap_chain_create(struct vulkan *vulkan);
void vulkan_render_pass_create(struct vulkan *vulkan);
void vulkan_command_pool_create(struct vulkan *vulkan);
void vulkan_command_buffers_create(struct vulkan *vulkan);
void vulkan_semaphore_create(struct vulkan *vulkan);
void vulkan_fence_create(struct vulkan *vulkan);
void vulkan_pipeline_layout_create(struct vulkan *vulkan);
void vulkan_framebuffer_create(struct vulkan *vulkan);
void vulkan_swap_chain_image_views_create(struct vulkan *vulkan);
void vulkan_swap_chain_depth_resources_create(struct vulkan *vulkan);
uint32_t vulkan_find_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void vulkan_image_view_create(struct vulkan *vulkan, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView *imageView);
void vulkan_image_create(struct vulkan *vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage *image, VkDeviceMemory *imageMemory);

VkShaderModule vulkan_shader_module_create(struct vulkan* vulkan, char* path);
VkPipeline vulkan_pipeline_create(struct vulkan* vulkan, char* vertPath, char* fragPath);
void vulkan_pipeline_add(struct vulkan* vulkan, VkPipeline pipeline);
void vulkan_draw(struct vulkan *vulkan);

void vulkan_swap_chain_destroy(struct vulkan *vulkan);
