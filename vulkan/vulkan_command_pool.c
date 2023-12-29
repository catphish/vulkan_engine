#include "vulkan.h"

void vulkan_command_pool_create(struct vulkan *vulkan) {
  VkCommandPoolCreateInfo poolInfo;
  memset(&poolInfo, 0, sizeof(VkCommandPoolCreateInfo));
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = vulkan->queueFamilyIndex;
  if (vkCreateCommandPool(vulkan->device, &poolInfo, NULL, &vulkan->commandPool) != VK_SUCCESS) {
    ERROR("Command pool creation failed!\n");
  }
}
