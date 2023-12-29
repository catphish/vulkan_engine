#include "vulkan.h"

void vulkan_image_create(struct vulkan *vulkan, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage *image, VkDeviceMemory *imageMemory) {
  VkImageCreateInfo imageInfo;
  memset(&imageInfo, 0, sizeof(VkImageCreateInfo));
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
  imageInfo.flags = 0;  // Optional
  if (vkCreateImage(vulkan->device, &imageInfo, NULL, image) != VK_SUCCESS) {
    ERROR("Failed to create image!\n");
  }

  VkMemoryRequirements memRequirements;
  vkGetImageMemoryRequirements(vulkan->device, *image, &memRequirements);

  VkMemoryAllocateInfo allocInfo;
  memset(&allocInfo, 0, sizeof(VkMemoryAllocateInfo));
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = vulkan_find_memory_type(vulkan->physicalDevice, memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(vulkan->device, &allocInfo, NULL, imageMemory) != VK_SUCCESS) {
    ERROR("Failed to allocate image memory!\n");
  }

  if (vkBindImageMemory(vulkan->device, *image, *imageMemory, 0) != VK_SUCCESS) {
    ERROR("Failed to bind image memory!\n");
  }
}
