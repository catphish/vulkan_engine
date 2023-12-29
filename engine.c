#include "vulkan/vulkan.h"

int main() {
  struct vulkan *vulkan;
  vulkan = vulkan_create();
  vulkan_pipeline_add(vulkan, vulkan_pipeline_create(vulkan, "shaders/triangle.vert.spv", "shaders/triangle.frag.spv"));

  while(vulkan_window_poll(vulkan)) {
    vulkan_draw(vulkan);
  }

  vulkan_destroy(vulkan);

  return 0;
}
