#include "vulkan.h"

void vulkan_semaphore_create(struct vulkan *vulkan) {
  VkSemaphoreCreateInfo semaphoreInfo;
  memset(&semaphoreInfo, 0, sizeof(VkSemaphoreCreateInfo));
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  for (int n = 0; n < MAX_FRAMES_IN_FLIGHT; n++) {
    if (vkCreateSemaphore(vulkan->device, &semaphoreInfo, NULL, &vulkan->imageAvailableSemaphores[n]) != VK_SUCCESS) {
      ERROR("Failed to create image available semaphore!\n");
    }
    if (vkCreateSemaphore(vulkan->device, &semaphoreInfo, NULL, &vulkan->renderFinishedSemaphores[n]) != VK_SUCCESS) {
      ERROR("Failed to create render finished semaphore!\n");
    }
  }
}
