#include "vulkan.h"

void vulkan_instance_create(struct vulkan *vulkan) {
  const char *validationLayers = "VK_LAYER_KHRONOS_validation";

  VkApplicationInfo appInfo;
  memset(&appInfo, 0, sizeof(appInfo));
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo;
  memset(&createInfo, 0, sizeof(createInfo));
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  const char *extensions[] = {
    "VK_KHR_surface",
    "VK_KHR_xcb_surface"
  };
  createInfo.enabledExtensionCount = 2;
  createInfo.ppEnabledExtensionNames = extensions;

  createInfo.enabledLayerCount = 1;
  createInfo.ppEnabledLayerNames = &validationLayers;

  if (vkCreateInstance(&createInfo, NULL, &vulkan->instance) != VK_SUCCESS) {
    ERROR("Failed to create instance!\n");
  }
}
