#include "vulkan.h"

void vulkan_draw(struct vulkan *vulkan) {
  vkWaitForFences(vulkan->device, 1, &vulkan->inFlightFences[vulkan->currentFrame], VK_TRUE, UINT64_MAX);
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(vulkan->device, vulkan->swapChain, UINT64_MAX, vulkan->imageAvailableSemaphores[vulkan->currentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    vulkan_swap_chain_destroy(vulkan);
    vulkan_swap_chain_create(vulkan);
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    ERROR("Failed to acquire swap chain image!\n");
  }

  VkCommandBuffer commandBuffer = vulkan->commandBuffers[vulkan->currentFrame];

  vkResetFences(vulkan->device, 1, &vulkan->inFlightFences[vulkan->currentFrame]);
  vkResetCommandBuffer(commandBuffer, 0);

  VkCommandBufferBeginInfo beginInfo;
  memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    ERROR("Begin command buffer failed!\n");
  }

  VkRenderPassBeginInfo renderPassInfo;
  memset(&renderPassInfo, 0, sizeof(VkRenderPassBeginInfo));
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = vulkan->renderPass;
  renderPassInfo.framebuffer = vulkan->swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset.x = 0;
  renderPassInfo.renderArea.offset.y = 0;
  renderPassInfo.renderArea.extent = vulkan->extent;

  VkClearValue clearValues[2];
  memset(&clearValues, 0, sizeof(clearValues));
  clearValues[0].color = (VkClearColorValue){{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = (VkClearDepthStencilValue){1.0f, 0};
  renderPassInfo.clearValueCount = 2;
  renderPassInfo.pClearValues = clearValues;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport;
  memset(&viewport, 0, sizeof(VkViewport));
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)vulkan->extent.width;
  viewport.height = (float)vulkan->extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor;
  memset(&scissor, 0, sizeof(VkRect2D));
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent = vulkan->extent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  for (int n = 0; n < vulkan->pipelineCount; n++) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->pipelines[n]);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
  }

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    ERROR("Failed to record command buffer!\n");
  }

  VkSubmitInfo submitInfo;
  memset(&submitInfo, 0, sizeof(VkSubmitInfo));
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {vulkan->imageAvailableSemaphores[vulkan->currentFrame]};
  VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  VkSemaphore signalSemaphores[] = {vulkan->renderFinishedSemaphores[vulkan->currentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;
  if (vkQueueSubmit(vulkan->queue, 1, &submitInfo, vulkan->inFlightFences[vulkan->currentFrame]) != VK_SUCCESS) {
    ERROR("Failed to submit draw command buffer!\n");
  }

  VkPresentInfoKHR presentInfo;
  memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  VkSwapchainKHR swapChains[] = {vulkan->swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  result = vkQueuePresentKHR(vulkan->queue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    vulkan_swap_chain_destroy(vulkan);
    vulkan_swap_chain_create(vulkan);
  } else if (result != VK_SUCCESS) {
    ERROR("Failed to present swap chain image!\n");
  }

  vulkan->currentFrame++;
  vulkan->currentFrame %= MAX_FRAMES_IN_FLIGHT;
}
