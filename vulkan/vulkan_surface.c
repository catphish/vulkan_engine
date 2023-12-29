#include "vulkan.h"

void vulkan_surface_create(struct vulkan *vulkan) {
  VkXcbSurfaceCreateInfoKHR createInfo;
  createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  createInfo.pNext = NULL;
  createInfo.flags = 0;
  createInfo.connection = vulkan->connection;
  createInfo.window = vulkan->window;
  VkResult err = vkCreateXcbSurfaceKHR(vulkan->instance, &createInfo, NULL, &vulkan->surface);
  if (err != VK_SUCCESS) {
    ERROR("Failed to create surface!\n");
  }
}
