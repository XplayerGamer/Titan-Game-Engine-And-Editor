#pragma once

#include "Renderer.hpp"
#include <string>

// Vulkan renderer scaffold. Provides a class compatible with the project's
// `Renderer` interface. The implementation is intentionally minimal and
// compiles without the Vulkan SDK; when `VULKAN_SDK` is defined the
// implementation will attempt to include Vulkan headers.

namespace Titan {

class VulkanRenderer : public Renderer {
public:
    VulkanRenderer() = default;
    ~VulkanRenderer() override = default;

    void Initialize() override;
    void Shutdown() override;
    void BeginFrame() override;
    void EndFrame() override;
    void Present() override;

    // Helper: set optional GPU device name
    void SetPreferredDevice(const std::string& name) { preferredDevice = name; }

private:
    std::string preferredDevice;
    bool initialized{false};
    // Internal handles are opaque here; real implementation will hold
    // VkInstance, VkDevice, VkSwapchainKHR, VkCommandBuffers, etc.
    void* vkInternal{nullptr};
};

} // namespace Titan
