#include "vulkan.h"

void vulkan_pipeline_layout_create(struct vulkan *vulkan) {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo;
  memset(&pipelineLayoutInfo, 0, sizeof(VkPipelineLayoutCreateInfo));
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  if (vkCreatePipelineLayout(vulkan->device, &pipelineLayoutInfo, NULL, &vulkan->pipelineLayout) != VK_SUCCESS) {
    ERROR("Failed to create pipeline layout!\n");
  }
}