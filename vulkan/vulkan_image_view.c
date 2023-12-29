#include "vulkan.h"

void vulkan_image_view_create(struct vulkan *vulkan, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView *imageView) {
  VkImageViewCreateInfo viewInfo;
  memset(&viewInfo, 0, sizeof(VkImageViewCreateInfo));
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;

  if (vkCreateImageView(vulkan->device, &viewInfo, NULL, imageView) != VK_SUCCESS) {
    ERROR("Failed to create image view!\n");
  }
}
