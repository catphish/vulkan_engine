#include "vulkan.h"

void vulkan_fence_create(struct vulkan *vulkan) {
  VkFenceCreateInfo fenceInfo;
  memset(&fenceInfo, 0, sizeof(VkFenceCreateInfo));
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  for (int n = 0; n < MAX_FRAMES_IN_FLIGHT; n++) {
    if (vkCreateFence(vulkan->device, &fenceInfo, NULL, &vulkan->inFlightFences[n]) != VK_SUCCESS) {
      ERROR("Failed to create in flight fence!\n");
    }
  }
}
