#include "vulkan.h"

void vulkan_framebuffer_create(struct vulkan *vulkan) {
  vulkan->swapChainFramebuffers = malloc(vulkan->swapChainImageCount * sizeof(VkFramebuffer));
  for (int n = 0; n < vulkan->swapChainImageCount; n++) {
    VkImageView attachments[] = {vulkan->swapChainImageViews[n], vulkan->depthImageView};
    VkFramebufferCreateInfo framebufferInfo;
    memset(&framebufferInfo, 0, sizeof(VkFramebufferCreateInfo));
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = vulkan->renderPass;
    framebufferInfo.attachmentCount = sizeof(attachments) / sizeof(VkImageView);
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = vulkan->extent.width;
    framebufferInfo.height = vulkan->extent.height;
    framebufferInfo.layers = 1;
    if (vkCreateFramebuffer(vulkan->device, &framebufferInfo, NULL, vulkan->swapChainFramebuffers + n) != VK_SUCCESS) {
      ERROR("Framebuffer creation failed!\n");
    }
  }
}
