#include "vulkan.h"

VkShaderModule vulkan_shader_module_create(struct vulkan* vulkan, char* path) {
  VkShaderModule shaderModule;
  FileData fileData = file_read(path);
  VkShaderModuleCreateInfo createInfo;
  memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = fileData.size;
  createInfo.pCode = (uint32_t*)fileData.data;
  if (vkCreateShaderModule(vulkan->device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) {
    ERROR("Shader creation failed!\n");
  }
  free(fileData.data);
  return shaderModule;
}
