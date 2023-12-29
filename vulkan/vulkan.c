#include "vulkan.h"

struct vulkan *vulkan_create() {
  struct vulkan *vulkan = malloc(sizeof(struct vulkan));
  vulkan_window_create(vulkan);
  vulkan_instance_create(vulkan);
  vulkan_surface_create(vulkan);
  vulkan_device_select(vulkan);
  vulkan_device_create(vulkan);
  vkGetDeviceQueue(vulkan->device, vulkan->queueFamilyIndex, 0, &vulkan->queue);
  vulkan_render_pass_create(vulkan);

  vulkan_command_pool_create(vulkan);
  vulkan_command_buffers_create(vulkan);
  vulkan_fence_create(vulkan);
  vulkan_semaphore_create(vulkan);

  vulkan_swap_chain_create(vulkan);
  vulkan_pipeline_layout_create(vulkan);
  return vulkan;
}

void vulkan_destroy(struct vulkan *vulkan) {
    vulkan_swap_chain_destroy(vulkan);

  for (int n = 0; n < vulkan->pipelineCount; n++) {
    vkDestroyPipeline(vulkan->device, vulkan->pipelines[n], NULL);
  }
  vkDestroyPipelineLayout(vulkan->device, vulkan->pipelineLayout, NULL);

  for (int n = 0; n < MAX_FRAMES_IN_FLIGHT; n++) {
    vkDestroySemaphore(vulkan->device, vulkan->imageAvailableSemaphores[n], NULL);
    vkDestroySemaphore(vulkan->device, vulkan->renderFinishedSemaphores[n], NULL);
    vkDestroyFence(vulkan->device, vulkan->inFlightFences[n], NULL);
  }

  vkDestroyCommandPool(vulkan->device, vulkan->commandPool, NULL);
  vkDestroyRenderPass(vulkan->device, vulkan->renderPass, NULL);
  vkDestroyDevice(vulkan->device, NULL);
  vkDestroySurfaceKHR(vulkan->instance, vulkan->surface, NULL);
  vkDestroyInstance(vulkan->instance, NULL);

  free(vulkan);
}