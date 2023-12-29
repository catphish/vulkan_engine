#include "vulkan.h"

void vulkan_swap_chain_image_views_create(struct vulkan *vulkan) {
  vulkan->swapChainImageViews = malloc(vulkan->swapChainImageCount * sizeof(VkImageView));
  for (int n = 0; n < vulkan->swapChainImageCount; n++) {
    vulkan_image_view_create(vulkan, vulkan->swapChainImages[n], VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, vulkan->swapChainImageViews + n);
  }
}

void vulkan_swap_chain_depth_resources_create(struct vulkan *vulkan) {
  VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
  vulkan_image_create(vulkan, vulkan->extent.width, vulkan->extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vulkan->depthImage, &vulkan->depthImageMemory);
  vulkan_image_view_create(vulkan, vulkan->depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &vulkan->depthImageView);
}

void vulkan_swap_chain_create(struct vulkan *vulkan) {
  xcb_get_geometry_cookie_t cookie;
  xcb_get_geometry_reply_t *reply;
  cookie = xcb_get_geometry(vulkan->connection, vulkan->window);
  if ((reply = xcb_get_geometry_reply(vulkan->connection, cookie, NULL))) {
    vulkan->extent.width = reply->width;
    vulkan->extent.height = reply->height;
  } else {
    ERROR("Failed to get window geometry!\n");
  }
  free(reply);

  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->physicalDevice, vulkan->surface, &capabilities);

  VkSwapchainCreateInfoKHR createInfo;
  memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = vulkan->surface;
  createInfo.minImageCount = 4;
  createInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
  createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  createInfo.imageExtent = vulkan->extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.preTransform = capabilities.currentTransform;
  createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;
  if (vkCreateSwapchainKHR(vulkan->device, &createInfo, NULL, &vulkan->swapChain) != VK_SUCCESS) {
    ERROR("Swapchain creation failed!\n");
  }
  vkGetSwapchainImagesKHR(vulkan->device, vulkan->swapChain, &vulkan->swapChainImageCount, NULL);
  vulkan->swapChainImages = malloc(vulkan->swapChainImageCount * sizeof(VkImage));
  vkGetSwapchainImagesKHR(vulkan->device, vulkan->swapChain, &vulkan->swapChainImageCount, vulkan->swapChainImages);

  vulkan_swap_chain_image_views_create(vulkan);
  vulkan_swap_chain_depth_resources_create(vulkan);
  vulkan_framebuffer_create(vulkan);
}

void vulkan_swap_chain_destroy(struct vulkan *vulkan) {
  vkDeviceWaitIdle(vulkan->device);

  vkDestroyImageView(vulkan->device, vulkan->depthImageView, NULL);
  vkDestroyImage(vulkan->device, vulkan->depthImage, NULL);
  vkFreeMemory(vulkan->device, vulkan->depthImageMemory, NULL);
  for (int n = 0; n < vulkan->swapChainImageCount; n++) vkDestroyFramebuffer(vulkan->device, vulkan->swapChainFramebuffers[n], NULL);
  for (int n = 0; n < vulkan->swapChainImageCount; n++) vkDestroyImageView(vulkan->device, vulkan->swapChainImageViews[n], NULL);
  vkDestroySwapchainKHR(vulkan->device, vulkan->swapChain, NULL);
}
