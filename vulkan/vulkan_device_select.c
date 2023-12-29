#include "vulkan.h"

void vulkan_device_select(struct vulkan *vulkan) {
  uint32_t deviceCount;
  vkEnumeratePhysicalDevices(vulkan->instance, &deviceCount, NULL);
  if (deviceCount == 0) {
    ERROR("Failed to find GPUs with Vulkan support!\n");
  }
  VkPhysicalDevice devices[deviceCount];
  vkEnumeratePhysicalDevices(vulkan->instance, &deviceCount, devices);

  for (int n = 0; n < deviceCount; n++) {
    VkPhysicalDevice device = devices[n];
    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    if (queueFamilyCount == 0) continue;
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);
    for (int n = 0; n < queueFamilyCount; n++) {
      VkQueueFamilyProperties queueFamily = queueFamilies[n];
      int required_queue_flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
      if ((queueFamily.queueFlags & required_queue_flags) == required_queue_flags) {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, n, vulkan->surface, &presentSupport);
        if (presentSupport) {
          vulkan->physicalDevice = device;
          vulkan->queueFamilyIndex = n;
          VkPhysicalDeviceProperties physicalProperties = {};
          vkGetPhysicalDeviceProperties(device, &physicalProperties);
          LOG("Found suitable GPU: %s\n", physicalProperties.deviceName);

          return;
        }
      }
    }
    ERROR("No suitable GPU found!\n");
  }
}
