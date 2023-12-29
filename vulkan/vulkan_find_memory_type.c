#include "vulkan.h"

uint32_t vulkan_find_memory_type(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t n = 0; n < memProperties.memoryTypeCount; n++) {
    if ((typeFilter & (1 << n)) && (memProperties.memoryTypes[n].propertyFlags & properties) == properties) {
      return n;
    }
  }
  ERROR("Failed to find suitable memory type!\n");
}
