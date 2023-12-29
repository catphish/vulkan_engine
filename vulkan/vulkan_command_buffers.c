#include "vulkan.h"

void vulkan_command_buffers_create(struct vulkan *vulkan) {
  VkCommandBufferAllocateInfo allocInfo;
  memset(&allocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = vulkan->commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;
  if (vkAllocateCommandBuffers(vulkan->device, &allocInfo, vulkan->commandBuffers) != VK_SUCCESS) {
    ERROR("Command buffer creation failed!\n");
  }
}
