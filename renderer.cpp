#pragma once
#include "defines.h"
#include "includes.h"


constexpr s64 max_frames_in_flieght = 2;

struct Instance_Functions {
        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
        PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
        PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
        PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
        PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
        PFN_vkCreateDevice vkCreateDevice;
};

struct Ubo {
        glm::mat4 camera;
        glm::mat4 model;
};

struct Triangle_Mesh {
        u64 vertex_count;
        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_buffer_memory;

        u64 index_count;
        VkBuffer index_buffer;
        VkDeviceMemory index_buffer_memory;

        VkBuffer texture_uv_buffer;
        VkDeviceMemory texture_uv_buffer_memory;
};



struct Texture{
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView view = VK_NULL_HANDLE;
};

struct Line_Mesh{};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
        puts(std::format("validation layer {} :{} \n", string_VkDebugUtilsMessageSeverityFlagsEXT(messageSeverity), pCallbackData->pMessage).c_str());
        return VK_FALSE;
}

template <typename T> static T load_vulkan_function(const char *name) { return reinterpret_cast<T>(glfwGetInstanceProcAddress(nullptr, name)); }

template <typename T> static T load_vulkan_function(VkInstance instance, const char *name) { return reinterpret_cast<T>(glfwGetInstanceProcAddress(instance, name)); }


//TODO: this needs to be completly refactored so that the models and textures can be packed together and rendered using minimal graphics pipelines.
struct Render_State {

        GLFWwindow * window;

        std::pmr::polymorphic_allocator<u8> allocator;
        VkAllocationCallbacks *vulkan_allocator = nullptr;
        VkInstance instance;
        VkSurfaceKHR window_surface;
        Instance_Functions instance_functions;
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        VkPhysicalDevice physical_device;
        VkPhysicalDeviceFeatures physical_device_features;
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        char **enabled_device_extensions;
        u32 device_extension_count;
        VkDevice device;
        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = VK_NULL_HANDLE;


        VkSurfaceCapabilitiesKHR surface_capabilities;
        VkSurfaceFormatKHR surface_format;
        VkPresentModeKHR present_mode;
        VkSwapchainCreateInfoKHR swapchain_info;
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
        u32 swapchain_image_count = 0;
        VkImage * swapchain_images = nullptr;
        VkImageView * swapchain_image_views = nullptr;

        VkViewport graphics_pipeline_viewport;
        VkRect2D graphics_pipeline_scissor;

        VkRenderPass render_pass = VK_NULL_HANDLE;
        VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
        VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
        VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
        VkPipeline graphics_pipeline = VK_NULL_HANDLE;
        u32 present_index;
        u32 graphics_index;
        u32 compute_index;
        VkQueue graphics_queue = VK_NULL_HANDLE;
        VkQueue present_queue = VK_NULL_HANDLE ;

        VkFramebuffer *frame_buffers = nullptr;
        VkCommandBuffer *command_buffers = nullptr;
        VkCommandPool command_pool = VK_NULL_HANDLE;
        VkSemaphore *image_available_semaphores = nullptr;
        VkSemaphore *render_finished_semaphores = nullptr;
        VkFence *image_in_flieght_fences = nullptr;


        VkMemoryRequirements vertex_index_test_buffer_memory_requirements;

        VkFormat depth_format;
        VkImage *depth_images = nullptr;
        VkDeviceMemory *depth_images_memory = nullptr;
        VkImageView *depth_images_views = nullptr;
        VkSampler sampler;

        u64 frame_count = 0;
        Ubo player_ubo;
        Ubo terrain_ubo;

        u32 vertex_count;
        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_buffer_memory;

        u32 index_count;
        VkBuffer index_buffer;
        VkDeviceMemory index_buffer_memory;

        VkDeviceMemory memory_block;

        Line_Mesh bounding_sphere_mesh;
        Line_Mesh debug_mesh;
        Line_Mesh selected_object_mesh;
        Texture terrain_texture;

        Triangle_Mesh terrain_mesh;
        Triangle_Mesh player_mesh;

        u32 opaque_mesh_count;
        VkBuffer player_ubo_buffer;
        VkDeviceMemory player_ubo_memory;
        void *player_ubo_mapped_memory;
        VkDescriptorSet player_descriptor_set;
        VkBuffer terrain_ubo_buffer;
        VkDeviceMemory terrain_ubo_memory;
        void *terrain_ubo_mapped_memory;
        VkDescriptorSet terrain_descriptor_set;
        VkBuffer ui_ubo_buffer;

        VkDescriptorSet texture_sampler_descriptor_set;

        struct UI{
                VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
                VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
                VkPipelineLayout pipeline_layout = VK_NULL_HANDLE;
                VkPipeline pipeline = VK_NULL_HANDLE;
                Texture texture;

                VkDeviceSize max_indices = 0;
                VkDeviceSize max_vertices = 0;
                VkDeviceSize index_count = 0;
                VkDeviceSize vertex_count = 0;
                VkBuffer indices = VK_NULL_HANDLE;
                VkBuffer positions = VK_NULL_HANDLE;
                VkBuffer texuvs = VK_NULL_HANDLE;
                VkBuffer colors = VK_NULL_HANDLE;

                VkDeviceMemory memory = VK_NULL_HANDLE;
                //TODO:
        }ui;

        PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR                 = nullptr;
        PFN_vkGetDeviceQueue vkGetDeviceQueue                           = nullptr;
        PFN_vkDestroyBuffer vkDestroyBuffer                             = nullptr;
        PFN_vkFreeMemory vkFreeMemory                                   = nullptr;
        PFN_vkCreateSampler vkCreateSampler                             = nullptr;
        PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR                   = nullptr;
        PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR             = nullptr;
        PFN_vkCreateImageView vkCreateImageView                         = nullptr;
        PFN_vkCreateShaderModule vkCreateShaderModule                   = nullptr;
        PFN_vkCreateDescriptorPool vkCreateDescriptorPool               = nullptr;
        PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout     = nullptr;
        PFN_vkCreatePipelineLayout vkCreatePipelineLayout               = nullptr;
        PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines         = nullptr;
        PFN_vkCreateRenderPass vkCreateRenderPass                       = nullptr;
        PFN_vkCreateImage vkCreateImage                                 = nullptr;
        PFN_vkBindBufferMemory vkBindBufferMemory                       = nullptr;
        PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
        PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements   = nullptr;
        PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets           = nullptr;
        PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets               = nullptr;
        PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers           = nullptr;
        PFN_vkFreeCommandBuffers vkFreeCommandBuffers                   = nullptr;
        PFN_vkBeginCommandBuffer vkBeginCommandBuffer                   = nullptr;
        PFN_vkEndCommandBuffer vkEndCommandBuffer                       = nullptr;
        PFN_vkCmdCopyBuffer vkCmdCopyBuffer                             = nullptr;
        PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier                   = nullptr;
        PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage               = nullptr;
        PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass                   = nullptr;
        PFN_vkCmdEndRenderPass vkCmdEndRenderPass                       = nullptr;
        PFN_vkCmdSetScissor vkCmdSetScissor                             = nullptr;
        PFN_vkCmdSetViewport vkCmdSetViewport                           = nullptr;
        PFN_vkCmdBindPipeline vkCmdBindPipeline                         = nullptr;
        PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets             = nullptr;
        PFN_vkCmdPushConstants vkCmdPushConstants                       = nullptr;
        PFN_vkCmdDispatch vkCmdDispatch                                 = nullptr;
        PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers               = nullptr;
        PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer                   = nullptr;
        PFN_vkCmdDraw vkCmdDraw                                         = nullptr;
        PFN_vkCmdDrawIndexed vkCmdDrawIndexed                           = nullptr;
        PFN_vkQueueSubmit vkQueueSubmit                                 = nullptr;
        PFN_vkDeviceWaitIdle vkDeviceWaitIdle                           = nullptr;
        PFN_vkMapMemory vkMapMemory                                     = nullptr;
        PFN_vkUnmapMemory vkUnmapMemory                                 = nullptr;
        PFN_vkAllocateMemory vkAllocateMemory                           = nullptr;
        PFN_vkBindImageMemory vkBindImageMemory                         = nullptr;
        PFN_vkCreateFramebuffer vkCreateFramebuffer                     = nullptr;
        PFN_vkCreateCommandPool vkCreateCommandPool                     = nullptr;
        PFN_vkCreateBuffer vkCreateBuffer                               = nullptr;
        PFN_vkCreateComputePipelines vkCreateComputePipelines           = nullptr;
        PFN_vkCreateSemaphore vkCreateSemaphore                         = nullptr;
        PFN_vkCreateFence vkCreateFence                                 = nullptr;
        PFN_vkWaitForFences vkWaitForFences                             = nullptr;
        PFN_vkResetFences vkResetFences                                 = nullptr;
        PFN_vkDestroyFence vkDestroyFence                               = nullptr;
        PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR                 = nullptr;
        PFN_vkQueuePresentKHR vkQueuePresentKHR                         = nullptr;
        PFN_vkQueueWaitIdle vkQueueWaitIdle                             = nullptr;
        PFN_vkDestroySemaphore vkDestroySemaphore                       = nullptr;
        PFN_vkDestroyFramebuffer vkDestroyFramebuffer                   = nullptr;
        PFN_vkDestroyImage vkDestroyImage                               = nullptr;
        PFN_vkDestroyImageView vkDestroyImageView                       = nullptr;

        template <typename T> inline T load_instance_function(char const *name) const noexcept { 
                return reinterpret_cast<T>(vkGetInstanceProcAddr(instance, name)); 
        }

        template <typename T> inline T load_device_function(char const *name) const noexcept { 
                return reinterpret_cast<T>(vkGetDeviceProcAddr(device, name)); 
        }

        inline void load_instance_functions() noexcept {
                instance_functions.vkCreateDebugUtilsMessengerEXT = load_instance_function<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
                instance_functions.vkEnumeratePhysicalDevices = load_instance_function<PFN_vkEnumeratePhysicalDevices>("vkEnumeratePhysicalDevices");
                instance_functions.vkGetPhysicalDeviceFeatures = load_instance_function<PFN_vkGetPhysicalDeviceFeatures>("vkGetPhysicalDeviceFeatures");
                instance_functions.vkGetPhysicalDeviceQueueFamilyProperties = load_instance_function<PFN_vkGetPhysicalDeviceQueueFamilyProperties>("vkGetPhysicalDeviceQueueFamilyProperties");
                instance_functions.vkGetPhysicalDeviceFormatProperties = load_instance_function<PFN_vkGetPhysicalDeviceFormatProperties>("vkGetPhysicalDeviceFormatProperties");
                instance_functions.vkGetPhysicalDeviceMemoryProperties = load_instance_function<PFN_vkGetPhysicalDeviceMemoryProperties>("vkGetPhysicalDeviceMemoryProperties");
                instance_functions.vkGetPhysicalDeviceProperties = load_instance_function<PFN_vkGetPhysicalDeviceProperties>("vkGetPhysicalDeviceProperties");
                instance_functions.vkGetPhysicalDeviceSurfaceSupportKHR = load_instance_function<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>("vkGetPhysicalDeviceSurfaceSupportKHR");
                instance_functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR = load_instance_function<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>( "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
                instance_functions.vkGetPhysicalDeviceSurfaceFormatsKHR = load_instance_function<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>("vkGetPhysicalDeviceSurfaceFormatsKHR");
                instance_functions.vkGetPhysicalDeviceSurfacePresentModesKHR = load_instance_function<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>("vkGetPhysicalDeviceSurfacePresentModesKHR");
                instance_functions.vkCreateDevice = load_instance_function<PFN_vkCreateDevice>("vkCreateDevice");
        }

        inline void load_device_functions() noexcept {
                vkGetDeviceProcAddr = load_instance_function<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr");

                vkDestroySwapchainKHR         = load_device_function<PFN_vkDestroySwapchainKHR>("vkDestroySwapchainKHR");
                vkGetDeviceQueue              = load_device_function<PFN_vkGetDeviceQueue>("vkGetDeviceQueue");
                vkDestroyBuffer               = load_device_function<PFN_vkDestroyBuffer>("vkDestroyBuffer");
                vkFreeMemory                  = load_device_function<PFN_vkFreeMemory>("vkFreeMemory");
                vkCreateSampler               = load_device_function<PFN_vkCreateSampler>("vkCreateSampler");
                vkCreateSwapchainKHR          = load_device_function<PFN_vkCreateSwapchainKHR>("vkCreateSwapchainKHR");
                vkGetSwapchainImagesKHR       = load_device_function<PFN_vkGetSwapchainImagesKHR>("vkGetSwapchainImagesKHR");
                vkCreateImageView             = load_device_function<PFN_vkCreateImageView>("vkCreateImageView");
                vkCreateShaderModule          = load_device_function<PFN_vkCreateShaderModule>("vkCreateShaderModule");
                vkCreateDescriptorPool        = load_device_function<PFN_vkCreateDescriptorPool>("vkCreateDescriptorPool");
                vkCreateDescriptorSetLayout   = load_device_function<PFN_vkCreateDescriptorSetLayout>("vkCreateDescriptorSetLayout");
                vkCreatePipelineLayout        = load_device_function<PFN_vkCreatePipelineLayout>("vkCreatePipelineLayout");
                vkCreateGraphicsPipelines     = load_device_function<PFN_vkCreateGraphicsPipelines>("vkCreateGraphicsPipelines");
                vkCreateRenderPass            = load_device_function<PFN_vkCreateRenderPass>("vkCreateRenderPass");
                vkCreateImage                 = load_device_function<PFN_vkCreateImage>("vkCreateImage");
                vkBindBufferMemory            = load_device_function<PFN_vkBindBufferMemory>("vkBindBufferMemory");
                vkGetBufferMemoryRequirements = load_device_function<PFN_vkGetBufferMemoryRequirements>("vkGetBufferMemoryRequirements");
                vkGetImageMemoryRequirements  = load_device_function<PFN_vkGetImageMemoryRequirements>("vkGetImageMemoryRequirements");
                vkAllocateDescriptorSets      = load_device_function<PFN_vkAllocateDescriptorSets>("vkAllocateDescriptorSets");
                vkUpdateDescriptorSets        = load_device_function<PFN_vkUpdateDescriptorSets>("vkUpdateDescriptorSets");
                vkAllocateCommandBuffers      = load_device_function<PFN_vkAllocateCommandBuffers>("vkAllocateCommandBuffers");
                vkFreeCommandBuffers          = load_device_function<PFN_vkFreeCommandBuffers>("vkFreeCommandBuffers");
                vkBeginCommandBuffer          = load_device_function<PFN_vkBeginCommandBuffer>("vkBeginCommandBuffer");
                vkEndCommandBuffer            = load_device_function<PFN_vkEndCommandBuffer>("vkEndCommandBuffer");
                vkCmdCopyBuffer               = load_device_function<PFN_vkCmdCopyBuffer>("vkCmdCopyBuffer");
                vkCmdPipelineBarrier          = load_device_function<PFN_vkCmdPipelineBarrier>("vkCmdPipelineBarrier");
                vkCmdCopyBufferToImage        = load_device_function<PFN_vkCmdCopyBufferToImage>("vkCmdCopyBufferToImage");
                vkCmdBeginRenderPass          = load_device_function<PFN_vkCmdBeginRenderPass>("vkCmdBeginRenderPass");
                vkCmdEndRenderPass            = load_device_function<PFN_vkCmdEndRenderPass>("vkCmdEndRenderPass");
                vkCmdSetScissor               = load_device_function<PFN_vkCmdSetScissor>("vkCmdSetScissor");
                vkCmdSetViewport              = load_device_function<PFN_vkCmdSetViewport>("vkCmdSetViewport");
                vkCmdBindPipeline             = load_device_function<PFN_vkCmdBindPipeline>("vkCmdBindPipeline");
                vkCmdBindDescriptorSets       = load_device_function<PFN_vkCmdBindDescriptorSets>("vkCmdBindDescriptorSets");
                vkCmdDispatch                 = load_device_function<PFN_vkCmdDispatch>("vkCmdDispatch");
                vkCmdBindVertexBuffers        = load_device_function<PFN_vkCmdBindVertexBuffers>("vkCmdBindVertexBuffers");
                vkCmdBindIndexBuffer          = load_device_function<PFN_vkCmdBindIndexBuffer>("vkCmdBindIndexBuffer");
                vkCmdPushConstants            = load_device_function<PFN_vkCmdPushConstants>("vkCmdPushConstants");
                vkCmdDraw                     = load_device_function<PFN_vkCmdDraw>("vkCmdDraw");
                vkCmdDrawIndexed              = load_device_function<PFN_vkCmdDrawIndexed>("vkCmdDrawIndexed");
                vkQueueSubmit                 = load_device_function<PFN_vkQueueSubmit>("vkQueueSubmit");
                vkDeviceWaitIdle              = load_device_function<PFN_vkDeviceWaitIdle>("vkDeviceWaitIdle");
                vkMapMemory                   = load_device_function<PFN_vkMapMemory>("vkMapMemory");
                vkUnmapMemory                 = load_device_function<PFN_vkUnmapMemory>("vkUnmapMemory");
                vkAllocateMemory              = load_device_function<PFN_vkAllocateMemory>("vkAllocateMemory");
                vkGetImageMemoryRequirements  = load_device_function<PFN_vkGetImageMemoryRequirements>("vkGetImageMemoryRequirements");
                vkBindImageMemory             = load_device_function<PFN_vkBindImageMemory>("vkBindImageMemory");
                vkCreateFramebuffer           = load_device_function<PFN_vkCreateFramebuffer>("vkCreateFramebuffer");
                vkCreateCommandPool           = load_device_function<PFN_vkCreateCommandPool>("vkCreateCommandPool");
                vkCreateBuffer                = load_device_function<PFN_vkCreateBuffer>("vkCreateBuffer");
                vkCreateComputePipelines      = load_device_function<PFN_vkCreateComputePipelines>("vkCreateComputePipelines");
                vkCreateSemaphore             = load_device_function<PFN_vkCreateSemaphore>("vkCreateSemaphore");
                vkCreateFence                 = load_device_function<PFN_vkCreateFence>("vkCreateFence");
                vkAcquireNextImageKHR         = load_device_function<PFN_vkAcquireNextImageKHR>("vkAcquireNextImageKHR");
                vkQueuePresentKHR             = load_device_function<PFN_vkQueuePresentKHR>("vkQueuePresentKHR");
                vkQueueWaitIdle               = load_device_function<PFN_vkQueueWaitIdle>("vkQueueWaitIdle");
                vkWaitForFences               = load_device_function<PFN_vkWaitForFences>("vkWaitForFences");
                vkResetFences                 = load_device_function<PFN_vkResetFences>("vkResetFences");
                vkDestroyFence                = load_device_function<PFN_vkDestroyFence>("vkDestroyFence");
                vkDestroySemaphore            = load_device_function<PFN_vkDestroySemaphore>("vkDestroySemaphore");
                vkDestroyFramebuffer          = load_device_function<PFN_vkDestroyFramebuffer>("vkDestroyFramebuffer");
                vkDestroyImage                = load_device_function<PFN_vkDestroyImage>("vkDestroyImage");
                vkDestroyImageView            = load_device_function<PFN_vkDestroyImageView>("vkDestroyImageView");
        }

        inline auto load_shader_module(std::filesystem::path path) noexcept -> VkShaderModule {
                puts("creating shader module");
                auto file = std::ifstream(path.string(), std::ios::ate | std::ios::binary);
                auto const fileSize = (size_t)file.tellg();
                auto buffer = std::vector<char>(fileSize);
                file.seekg(0);
                file.read(buffer.data(), fileSize);
                puts(std::format("loading shader {} with bytecount {}", path.string(), buffer.size()).c_str());
                auto const shader_module_create_info = VkShaderModuleCreateInfo{
                        .sType = vku::GetSType<VkShaderModuleCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .codeSize = buffer.size(),
                        .pCode = reinterpret_cast<uint32_t const *>(buffer.data()),
                };
                VkShaderModule module;
                if (vkCreateShaderModule(device, &shader_module_create_info, vulkan_allocator, &module)) {
                        puts("unable to create shader module");
                        exit(420);
                }
                return module;
        };

        constexpr auto find_memory_type(uint32_t memory_bits_requirement, VkMemoryPropertyFlags properties)const noexcept {
                for (uint32_t memory_type_index = 0; memory_type_index < physical_device_memory_properties.memoryTypeCount; ++memory_type_index) {
                        auto memory_properties = physical_device_memory_properties.memoryTypes[memory_type_index];
                        if (memory_bits_requirement & (1 << memory_type_index) and (memory_properties.propertyFlags & properties) == properties) {
                                return memory_type_index;
                        }
                }

                puts("unable to find suitable memory index.");
                exit(420);
        };

        inline auto create_buffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_properties, VkDeviceSize size, VkBuffer *buffer, VkDeviceMemory *buffer_memory)const noexcept{
                auto const buffer_create_info = VkBufferCreateInfo{
                        .sType = vku::GetSType<VkBufferCreateInfo>(),
                        .size = size,
                        .usage = usage,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                };
                vkCreateBuffer(device, &buffer_create_info, vulkan_allocator, buffer);

                VkMemoryRequirements buffer_memory_requirements;
                vkGetBufferMemoryRequirements(device, *buffer, &buffer_memory_requirements);

                auto const buffer_memory_alloc_info = VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = buffer_memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(buffer_memory_requirements.memoryTypeBits, memory_properties),
                };
                vkAllocateMemory(device, &buffer_memory_alloc_info, vulkan_allocator, buffer_memory);
                vkBindBufferMemory(device, *buffer, *buffer_memory, 0);
        };

        static inline auto settup_descriptors(Render_State *state) noexcept{
                auto const ui_pool_size = VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = 1};
                auto const ui_pool_info = VkDescriptorPoolCreateInfo{
                        .sType = vku::GetSType<VkDescriptorPoolCreateInfo>(),
                        .maxSets = 1,
                        .poolSizeCount = 1,
                        .pPoolSizes = &ui_pool_size,
                };
                VkDescriptorPool ui_pool;
                if (auto const result = state->vkCreateDescriptorPool(state->device, &ui_pool_info, state->vulkan_allocator, &ui_pool); result not_eq VK_SUCCESS) {
                        puts(std::format("unable to create descriptor pool ui {}", string_VkResult(result)).c_str());
                        exit(420);
                }
                auto const pool_set_allocate_info = VkDescriptorSetAllocateInfo{
                        .sType = vku::GetSType<VkDescriptorSetAllocateInfo>(),
                        .descriptorPool = ui_pool,
                        .descriptorSetCount = 1,
                        .pSetLayouts = &state->ui.descriptor_set_layout,
                };
                if (auto const result = state->vkAllocateDescriptorSets(state->device, &pool_set_allocate_info, &state->ui.descriptor_set); result not_eq VK_SUCCESS) {
                        puts("unable to allocate ui descriptor set");
                        exit(420);
                }

                auto const ubo_buffer_size = sizeof(Ubo);
                // auto const terrain_uniform_descriptor_pool_size = ;
                constexpr u8 model_count = 2;
                //TODO: make these descriptors for the ubo a push constant, and decombine the image and sampler.
                auto const descriptors_pool_sizes = std::array{
                        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, .descriptorCount = model_count },
                        VkDescriptorPoolSize{.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, .descriptorCount = model_count},
                };

                auto const descriptor_pool_create_info = VkDescriptorPoolCreateInfo{
                        .sType = vku::GetSType<VkDescriptorPoolCreateInfo>(),
                        .maxSets = model_count,
                        .poolSizeCount = descriptors_pool_sizes.size(),
                        .pPoolSizes = descriptors_pool_sizes.data(),
                };

                if (auto const result = state->vkCreateDescriptorPool(state->device, &descriptor_pool_create_info, state->vulkan_allocator, &state->descriptor_pool); result not_eq VK_SUCCESS) {
                        puts(std::format("unable to create descriptor pool {}", string_VkResult(result)).c_str());
                        exit(429);
                }

                auto const descriptor_set_allocation_info = VkDescriptorSetAllocateInfo{
                        .sType = vku::GetSType<VkDescriptorSetAllocateInfo>(),
                        .descriptorPool = state->descriptor_pool,
                        .descriptorSetCount = 1,
                        .pSetLayouts = &state->descriptor_set_layout,
                };

                VkDescriptorSet * descriptor_sets[] = {
                        &state->player_descriptor_set,
                        &state->terrain_descriptor_set,
                };

                for(auto i = 0; i < model_count;++i){
                        if (auto const result = state->vkAllocateDescriptorSets(state->device, &descriptor_set_allocation_info, descriptor_sets[i]); result not_eq VK_SUCCESS) {
                                puts(std::format("unable to allocate descriptor set[{}] {}", i, string_VkResult(result)).c_str());
                                exit(420);
                        }
                }

                VkBuffer* ubo_buffers[] = {
                        &state->player_ubo_buffer,
                        &state->terrain_ubo_buffer,
                        &state->ui_ubo_buffer,
                };

                VkDeviceMemory * ubo_buffer_memory[] = {
                        &state->player_ubo_memory,
                        &state->terrain_ubo_memory,
                };

                void ** ubo_mapped_buffer_memory[] = {
                        &state->player_ubo_mapped_memory,
                        &state->terrain_ubo_mapped_memory,
                };

                for(auto i = 0; i < model_count;++i){
                        state->create_buffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, ubo_buffer_size, ubo_buffers[i], ubo_buffer_memory[i]);
                        state->vkMapMemory(state->device, *ubo_buffer_memory[i], 0, ubo_buffer_size, 0, ubo_mapped_buffer_memory[i]);
                        memcpy(*ubo_mapped_buffer_memory[i], ubo_buffers[i], ubo_buffer_size);
                }


                for(auto i = 0; i < model_count;++i){
                        auto const info = VkDescriptorBufferInfo{
                                .buffer = *ubo_buffers[i],
                                .offset = 0,
                                .range = ubo_buffer_size,
                        };
                        auto const write = VkWriteDescriptorSet{
                                .sType = vku::GetSType<VkWriteDescriptorSet>(),
                                .dstSet = *descriptor_sets[i],
                                .dstBinding = 0,
                                .dstArrayElement = 0,
                                .descriptorCount = 1,
                                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                .pImageInfo = nullptr,
                                .pBufferInfo = &info,
                                .pTexelBufferView = nullptr,
                        };
                        state->vkUpdateDescriptorSets(state->device, 1, &write, 0, nullptr);
                }
        }

        inline auto create_shader_module(size_t size, u32 const * data) noexcept{
                auto info = VkShaderModuleCreateInfo{
                        .sType = vku::GetSType<VkShaderModuleCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .codeSize = size,
                        .pCode = data,
                };
                VkShaderModule module;
                if (vkCreateShaderModule(device, &info, vulkan_allocator, &module)) {
                        puts("unable to create shader module");
                        exit(420);
                }
                return module;
        }

        inline void get_physical_device_surface_capabilities() noexcept{
                if (instance_functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, window_surface, &surface_capabilities) not_eq VK_SUCCESS) {
                        puts("unable to get surface capabilities");
                        exit(39393);
                }
        }

        //TODO: find the correct image format for our aplication
        inline void get_physical_device_surface_format() noexcept{
                VkSurfaceFormatKHR format;
                u32 format_count = 0;
                if (instance_functions.vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, window_surface, &format_count, nullptr) not_eq VK_SUCCESS) {
                        exit(420);
                }
                VkSurfaceFormatKHR * formats = allocator.allocate_object<VkSurfaceFormatKHR>(format_count);
                instance_functions.vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, window_surface, &format_count, formats);
                surface_format = formats[0];
                allocator.deallocate_object(formats, format_count);
        }

        //TODO: find correct present mode for application.
        inline void get_physical_device_surface_present_mode(){
                u32 present_mode_count = 0;
                if (instance_functions.vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, window_surface, &present_mode_count, nullptr) not_eq VK_SUCCESS) {
                        exit(39393);
                }
                auto present_modes = allocator.allocate_object<VkPresentModeKHR>(present_mode_count);
                instance_functions.vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, window_surface, &present_mode_count, present_modes);
                // present_mode = present_modes[0];
                present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
                allocator.deallocate_object(present_modes, present_mode_count);
        }

        //if the swapchain alread exists it puts it as an old swapchain and creates a new one.
        inline void create_swapchain() noexcept{
                //TODO: clamp current extent in surface capabilities.
                get_physical_device_surface_capabilities();
                get_physical_device_surface_format();
                get_physical_device_surface_present_mode();

                auto sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
                auto queue_family_indices = std::vector<uint32_t>();
                queue_family_indices.push_back(graphics_index);
                if (graphics_index not_eq present_index) {
                        queue_family_indices.push_back(present_index);
                        sharing_mode = VK_SHARING_MODE_CONCURRENT;
                }

                swapchain_info = VkSwapchainCreateInfoKHR{
                        .sType = vku::GetSType<VkSwapchainCreateInfoKHR>(),
                        .pNext = nullptr,
                        .flags = {},
                        .surface = window_surface,
                        .minImageCount =  surface_capabilities.minImageCount + 1,
                        .imageFormat = surface_format.format,
                        .imageColorSpace = surface_format.colorSpace,
                        .imageExtent = surface_capabilities.currentExtent,
                        .imageArrayLayers = 1,
                        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                        .imageSharingMode = sharing_mode,
                        .queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices.size()),
                        .pQueueFamilyIndices = queue_family_indices.data(),
                        .preTransform = surface_capabilities.currentTransform,
                        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                        .presentMode = present_mode,
                        .clipped = VK_TRUE,
                        .oldSwapchain = swapchain,
                };

                if (auto result = vkCreateSwapchainKHR(device, &swapchain_info, vulkan_allocator, &swapchain); result not_eq VK_SUCCESS) {
                        puts(std::format("unable to create swapchain:{}", string_VkResult(result)).c_str());
                        exit(2323232);
                }

                graphics_pipeline_viewport = VkViewport{
                        .x = 0,
                        .y = 0,
                        .width = static_cast<float>(swapchain_info.imageExtent.width),
                        .height = static_cast<float>(swapchain_info.imageExtent.height),
                        .minDepth = 0.001,
                        .maxDepth = 1,
                };

                graphics_pipeline_scissor = VkRect2D{
                        .offset = {0, 0},
                        .extent = swapchain_info.imageExtent,
                };
        }

        inline void create_swapchain_images() noexcept{
                if (vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, nullptr) not_eq VK_SUCCESS) exit(2424242);
                swapchain_images = allocator.allocate_object<VkImage>(swapchain_image_count);
                vkGetSwapchainImagesKHR(device, swapchain, &swapchain_image_count, swapchain_images);
                swapchain_image_views = allocator.allocate_object<VkImageView>(swapchain_image_count);
                for (auto i = 0; i < swapchain_image_count; ++i) {
                        auto const subresource_range = VkImageSubresourceRange{
                                .aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        };
                        auto const image_view_create_info = VkImageViewCreateInfo{
                                .sType = vku::GetSType<VkImageViewCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .image = swapchain_images[i],
                                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                .format = surface_format.format,
                                .components = {},
                                .subresourceRange = subresource_range,
                        };
                        if (vkCreateImageView(device, &image_view_create_info, vulkan_allocator, &swapchain_image_views[i]) not_eq VK_SUCCESS) {
                                puts("unable to create swapchain image views");
                                exit(52);
                        }
                }
        }

        auto const create_device_local_image(VkFormat format, uint32_t mip_levels, VkImageTiling tiling, VkImageUsageFlags usage) {
                auto const swapchain_image_extent = swapchain_info.imageExtent;
                auto const image_info = VkImageCreateInfo{
                        .sType = vku::GetSType<VkImageCreateInfo>(),
                        .imageType = VK_IMAGE_TYPE_2D,
                        .format = format,
                        .extent = VkExtent3D{swapchain_image_extent.width, swapchain_image_extent.height, 1},
                        .mipLevels = mip_levels,
                        .arrayLayers = 1,
                        .samples = VK_SAMPLE_COUNT_1_BIT,
                        .tiling = tiling,
                        .usage = usage,
                        .queueFamilyIndexCount = 0,
                        .pQueueFamilyIndices = nullptr,
                        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                };
                VkImage image;
                if (vkCreateImage(device, &image_info, vulkan_allocator, &image) not_eq VK_SUCCESS) {
                        puts("unable to create vulkan image");
                        exit(420);
                }
                VkMemoryRequirements image_memory_requirements;
                vkGetImageMemoryRequirements(device, image, &image_memory_requirements);

                VkMemoryAllocateInfo memory_allocate_info{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = image_memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(image_memory_requirements.memoryTypeBits, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) 
                };

                VkDeviceMemory image_memory;
                if (vkAllocateMemory(device, &memory_allocate_info, vulkan_allocator, &image_memory) not_eq VK_SUCCESS) {
                        puts("unable to allocate memory for an image");
                        exit(420);
                }

                if (vkBindImageMemory(device, image, image_memory, 0) not_eq VK_SUCCESS) {
                        exit(420);
                }

                struct {
                        VkImage image;
                        VkDeviceMemory memory;
                } image_stuff{image, image_memory};
                return image_stuff;
        }

        inline auto create_depth_images() noexcept{
                depth_images = allocator.allocate_object<VkImage>(swapchain_image_count);
                depth_images_memory = allocator.allocate_object<VkDeviceMemory>(swapchain_image_count);
                depth_images_views = allocator.allocate_object<VkImageView>(swapchain_image_count); 

                for(auto i = 0; i < swapchain_image_count; ++i){
                        auto [depth_image, depth_image_memory] = create_device_local_image(depth_format, 1, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
                        depth_images[i] = depth_image;
                        depth_images_memory[i] = depth_image_memory;
                        auto const depth_subresource_range = VkImageSubresourceRange{
                                .aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        };
                        auto const depth_image_view_create_info = VkImageViewCreateInfo{
                                .sType = vku::GetSType<VkImageViewCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .image = depth_image,
                                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                                .format = depth_format,
                                .components = {},
                                .subresourceRange = depth_subresource_range,
                        };
                        if (vkCreateImageView(device, &depth_image_view_create_info, vulkan_allocator, &depth_images_views[i]) not_eq VK_SUCCESS) {
                                exit(520);
                        }
                }
        }

        inline auto destroy_depth_images() noexcept{
                assert(depth_images != nullptr and depth_images_memory != nullptr and depth_images_views != nullptr);
                if(depth_images){
                        for(auto i = 0; i < swapchain_image_count; ++i){
                                vkDestroyImage(device, depth_images[i], vulkan_allocator);
                        }
                        allocator.deallocate_object(depth_images, swapchain_image_count);
                        depth_images = nullptr;
                }
                if(depth_images_memory){
                        for(auto i = 0; i < swapchain_image_count; ++i){
                                vkFreeMemory(device, depth_images_memory[i], vulkan_allocator);
                        }
                        allocator.deallocate_object(depth_images_memory, swapchain_image_count);
                        depth_images_memory = nullptr;
                }
                if(depth_images_views){
                        for(auto i = 0; i < swapchain_image_count; ++i){
                                vkDestroyImageView(device, depth_images_views[i], vulkan_allocator);
                        }
                        allocator.deallocate_object(depth_images_views, swapchain_image_count);
                        depth_images_views = nullptr;
                }
        }

        inline auto create_swapchain_frame_buffers() noexcept{
                frame_buffers = allocator.allocate_object<VkFramebuffer>(swapchain_image_count);
                for (auto i = 0; i < swapchain_image_count; ++i) {
                        VkImageView attachments[]{
                                swapchain_image_views[i],
                                depth_images_views[i],
                        };

                        auto const creat_info = VkFramebufferCreateInfo{
                                .sType = vku::GetSType<VkFramebufferCreateInfo>(),
                                .flags = {},
                                .renderPass = render_pass,
                                .attachmentCount = 2,
                                .pAttachments = attachments,
                                .width = swapchain_info.imageExtent.width,
                                .height = swapchain_info.imageExtent.height,
                                .layers = 1,
                        };
                        if (vkCreateFramebuffer(device, &creat_info, vulkan_allocator, &frame_buffers[i]) not_eq VK_SUCCESS) {
                                exit(630);
                        }
                }
        }

        inline auto destroy_swapchain_frame_buffers() noexcept{
                if(not frame_buffers) return;
                for(auto i = 0; i < swapchain_image_count; ++i){
                        vkDestroyFramebuffer(device, frame_buffers[i], vulkan_allocator);
                }
                allocator.deallocate_object(frame_buffers, swapchain_image_count);
                frame_buffers = nullptr;
        }

        static inline auto find_queue_family_indices(Render_State * state){
                u32 property_count = 0;
                state->instance_functions.vkGetPhysicalDeviceQueueFamilyProperties(state->physical_device, &property_count, nullptr);
                auto properties = std::vector<VkQueueFamilyProperties>(property_count);
                state->instance_functions.vkGetPhysicalDeviceQueueFamilyProperties(state->physical_device, &property_count, properties.data());

                struct {
                        s32 graphics_index;
                        s32 present_index;
                        s32 compute_index;
                } indices{-1, -1, -1};

                for (auto i = 0; i < property_count; ++i) {
                        auto const &property = properties[i];
                        if (indices.graphics_index < 0 and property.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
                                indices.graphics_index = i;
                        }
                        if (indices.compute_index < 0 and property.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                                indices.compute_index = i;
                        }
                        VkBool32 surface_is_supported = VK_FALSE;
                        if (state->instance_functions.vkGetPhysicalDeviceSurfaceSupportKHR(state->physical_device, i, state->window_surface, &surface_is_supported) not_eq VK_SUCCESS) {
                                puts(std::format("device surface support for queue index {} is not supported", i).c_str());
                                continue;
                        }
                        if (indices.present_index < 0 and surface_is_supported) {
                                indices.present_index = i;
                        }

                        if (indices.graphics_index >= 0 and indices.present_index >= 0 and indices.compute_index >= 0) {
                                break;
                        }
                }
                return indices;
        }

        //TODO: create semaphores as we render images.
        inline void settup_sync(){
                if(not image_available_semaphores)image_available_semaphores = allocator.allocate_object<VkSemaphore>(max_frames_in_flieght);
                if(not render_finished_semaphores)render_finished_semaphores = allocator.allocate_object<VkSemaphore>(max_frames_in_flieght);
                if(not image_in_flieght_fences)image_in_flieght_fences = allocator.allocate_object<VkFence>(max_frames_in_flieght);
                for (auto i = 0; i < max_frames_in_flieght; ++i) {
                        auto empty_semaphore_create_info = VkSemaphoreCreateInfo{
                                .sType = vku::GetSType<VkSemaphoreCreateInfo>(),
                        };
                        if (vkCreateSemaphore(device, &empty_semaphore_create_info, vulkan_allocator, &image_available_semaphores[i]) not_eq VK_SUCCESS) exit(3939);
                        if (vkCreateSemaphore(device, &empty_semaphore_create_info, vulkan_allocator, &render_finished_semaphores[i]) not_eq VK_SUCCESS) exit(3939);
                        auto fence_create_info = VkFenceCreateInfo{
                                .sType = vku::GetSType<VkFenceCreateInfo>(),
                                .flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT,
                        };
                        if (vkCreateFence(device, &fence_create_info, vulkan_allocator, &image_in_flieght_fences[i]) not_eq VK_SUCCESS) exit(765987);
                }
        }

        inline void destroy_sync(){
                vkDeviceWaitIdle(device);
                for(auto i = 0; i < max_frames_in_flieght; ++i){
                        vkDestroySemaphore(device, image_available_semaphores[i], vulkan_allocator);
                        vkDestroySemaphore(device, render_finished_semaphores[i], vulkan_allocator);
                        vkDestroyFence(device, image_in_flieght_fences[i], vulkan_allocator);
                }
        }

        inline void recreate_swapchain() noexcept{
                destroy_sync();
                destroy_swapchain_frame_buffers();
                // destroy_depth_images();

                create_swapchain();
                create_swapchain_images();

                // create_depth_images();
                create_swapchain_frame_buffers();
                settup_sync();
        }

        static inline auto initalize(Render_State *state, GLFWwindow * window) noexcept {
                state->window = window;
                puts("initalizeing vulkan render state");
                auto app_info = VkApplicationInfo{
                        .sType = vku::GetSType<VkApplicationInfo>(),
                        .pNext = nullptr,
                        .pApplicationName = "plastic",
                        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                        .pEngineName = "plastic",
                        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                        .apiVersion = VK_API_VERSION_1_3,
                };

                u32 glfw_extension_count;
                auto const glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
                auto extension_names = std::vector(glfw_extensions, glfw_extensions + glfw_extension_count);
                #ifdef DEBUG
                extension_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                #endif
                
                u32 extension_count = UINT8_MAX;
                VkExtensionProperties extension_properties[UINT8_MAX];
                load_vulkan_function<PFN_vkEnumerateInstanceExtensionProperties>("vkEnumerateInstanceExtensionProperties")(nullptr, &extension_count, extension_properties);
                printf("extension count %d\n", extension_count);
                for (auto &&name : extension_names) {
                        for(auto i = 0; i < extension_count; ++i){
                                if(0 == strcmp(name, extension_properties[i].extensionName)) goto has_extension;
                        }
                        puts(std::format("unable to find require vulkan extension: {}", name).c_str());
                        exit(420);
                has_extension:continue;
                }

                //TODO: make these functions that loop until all required layers and extensions have been checked.
                u32 layer_count = UINT8_MAX;
                VkLayerProperties layer_properties[UINT8_MAX];
                load_vulkan_function<PFN_vkEnumerateInstanceLayerProperties>("vkEnumerateInstanceLayerProperties")(&layer_count, layer_properties);
                printf("layer_count %d \n", layer_count);

                auto layers = std::vector<char const *>(); 
                #ifdef DEBUG
                layers.push_back("VK_LAYER_KHRONOS_validation");
                #endif

                for(auto && layer: layers){
                        for(auto i = 0; i < layer_count; ++i){
                                if(0 == strcmp(layer, layer_properties[i].layerName)) goto has_layer;
                        }
                        puts(std::format("unable to find require vulkan layer: {}", layer).c_str());
                        exit(420);
                has_layer:continue;
                }


                auto instantce_info = VkInstanceCreateInfo{
                        .sType = vku::GetSType<VkInstanceCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .pApplicationInfo = &app_info,
                        .enabledLayerCount = (u32)layers.size(),
                        .ppEnabledLayerNames = layers.data(),
                        .enabledExtensionCount = static_cast<uint32_t>(extension_names.size()),
                        .ppEnabledExtensionNames = extension_names.data(),
                };

                auto const vkCreateInstance = load_vulkan_function<PFN_vkCreateInstance>("vkCreateInstance");

                if (auto result = vkCreateInstance(&instantce_info, state->vulkan_allocator, &state->instance); result not_eq VK_SUCCESS) {
                        puts(std::format("Unable to create vulkan instance: {}", string_VkResult(result)).c_str());
			exit(69);
                }
                state->vkGetInstanceProcAddr = load_vulkan_function<PFN_vkGetInstanceProcAddr>(state->instance, "vkGetInstanceProcAddr");
                state->load_instance_functions();
                auto debug_messenger_info = VkDebugUtilsMessengerCreateInfoEXT{
                        .sType = vku::GetSType<VkDebugUtilsMessengerCreateInfoEXT>(),
                        .pNext = nullptr,
                        .flags = {},
                        .messageSeverity = /* VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | */ VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                        .pfnUserCallback = debug_callback,
                };

                #ifdef DEBUG
                VkDebugUtilsMessengerEXT debug_utils_messenger;
                if (not state->instance_functions.vkCreateDebugUtilsMessengerEXT) {
                        puts("unable to load debug utils messenger.\n");
                } else if (state->instance_functions.vkCreateDebugUtilsMessengerEXT(state->instance, &debug_messenger_info, state->vulkan_allocator, &debug_utils_messenger) not_eq VK_SUCCESS) {
                        puts("unable to create debug utils messenger.\n");
                }
                #endif

                if (glfwCreateWindowSurface(state->instance, window, state->vulkan_allocator, &state->window_surface) not_eq VK_SUCCESS) {
                        puts("Unable to get surface");
                        exit(420);
                }

                u32 device_count = 0;
                if (state->instance_functions.vkEnumeratePhysicalDevices(state->instance, &device_count, nullptr) not_eq VK_SUCCESS or not device_count) {
                        puts("no physical device.");
                        exit(420);
                }
                auto physical_devices = std::vector<VkPhysicalDevice>(device_count);
                state->instance_functions.vkEnumeratePhysicalDevices(state->instance, &device_count, physical_devices.data());

                // TODO: choose a proper rendering device.
                state->physical_device = physical_devices[0];

                auto queue_family_indices = find_queue_family_indices(state);
                state->graphics_index = queue_family_indices.graphics_index;
                state->present_index = queue_family_indices.present_index;
                state->compute_index = queue_family_indices.compute_index;

                f32 graphics_queue_prioraties[] = {1.0f};

                state->instance_functions.vkGetPhysicalDeviceFeatures(state->physical_device, &state->physical_device_features);
                state->instance_functions.vkGetPhysicalDeviceMemoryProperties(state->physical_device, &state->physical_device_memory_properties);

                auto const queue_create_infos = std::array{
                        VkDeviceQueueCreateInfo{
                                .sType = vku::GetSType<VkDeviceQueueCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .queueFamilyIndex = static_cast<uint32_t>(state->graphics_index),
                                .queueCount = 1,
                                .pQueuePriorities = graphics_queue_prioraties,
                        },
                };

                char const *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
                auto const device_create_info = VkDeviceCreateInfo{
                        .sType = vku::GetSType<VkDeviceCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .queueCreateInfoCount = queue_create_infos.size(),
                        .pQueueCreateInfos = queue_create_infos.data(),
                        .enabledLayerCount = (u32)layers.size(),
                        .ppEnabledLayerNames = layers.data(),
                        .enabledExtensionCount = 1,
                        .ppEnabledExtensionNames = device_extensions,
                        .pEnabledFeatures = &state->physical_device_features,
                };

                if (auto result = state->instance_functions.vkCreateDevice(state->physical_device, &device_create_info, state->vulkan_allocator, &state->device); result not_eq VK_SUCCESS) {
                        puts(std::format("unable to create device: {}", string_VkResult(result)).c_str());
                        exit(42);
                }
                puts("loading device functions.");
                state->load_device_functions();

                state->vkGetDeviceQueue(state->device, state->graphics_index, 0, &state->graphics_queue);

                state->create_swapchain();
                state->create_swapchain_images();

                auto const color_attachment = VkAttachmentDescription{
                        .flags = {},
                        .format = state->surface_format.format,
                        .samples = VK_SAMPLE_COUNT_1_BIT,
                        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                };

                auto const color_attachment_refrence = VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                };

                auto depth_format = std::optional<VkFormat>(std::nullopt);
                for (auto format : {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}) {
                        VkFormatProperties physical_device_format_properties;
                        state->instance_functions.vkGetPhysicalDeviceFormatProperties(state->physical_device, format, &physical_device_format_properties); 
                        if ((physical_device_format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) > 0) {
                                depth_format = format;
                                break;
                        }
                }
                if (not depth_format) {
                        puts("unable to find depth format");
                        exit(7894238);
                }

                state->depth_format = depth_format.value();

                auto const depth_attachment = VkAttachmentDescription{
                        .flags = {},
                        .format = state->depth_format,
                        .samples = VK_SAMPLE_COUNT_1_BIT,
                        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
                };

                auto const depth_attachement_refrence = VkAttachmentReference{
                        .attachment = 1,
                        .layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
                };

                auto const subpass = VkSubpassDescription{
                        .flags = {},
                        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                        .colorAttachmentCount = 1,
                        .pColorAttachments = &color_attachment_refrence,
                        .pDepthStencilAttachment =&depth_attachement_refrence,
                };

                VkPipelineStageFlags stage_mask_bits = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

                auto const subpass_dependency = VkSubpassDependency{
                        .srcSubpass = VK_SUBPASS_EXTERNAL,
                        .dstSubpass = 0,
                        .srcStageMask = stage_mask_bits,
                        .dstStageMask = stage_mask_bits,
                        .srcAccessMask = {},
                        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                        .dependencyFlags = {},
                };

                auto const attachemnts = std::array{
                        color_attachment,
                        depth_attachment
                };

                auto const render_pass_create_info = VkRenderPassCreateInfo{
                        .sType = vku::GetSType<VkRenderPassCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .attachmentCount = attachemnts.size(),
                        .pAttachments = attachemnts.data(),
                        .subpassCount = 1,
                        .pSubpasses = &subpass,
                        .dependencyCount = 1,
                        .pDependencies = &subpass_dependency,
                };

                if (state->vkCreateRenderPass(state->device, &render_pass_create_info, state->vulkan_allocator, &state->render_pass) not_eq VK_SUCCESS) {
                        puts("unable to create render pass");
                        exit(1);
                }


                auto const vertex_shader_module = state->load_shader_module("shader.vert.spv");
                auto const vertex_shader_stage_create_info = VkPipelineShaderStageCreateInfo{
                        .sType = vku::GetSType<VkPipelineShaderStageCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .stage = VK_SHADER_STAGE_VERTEX_BIT,
                        .module = vertex_shader_module,
                        .pName = "main",
                };

                auto const fragment_shader_module = state->load_shader_module("shader.frag.spv");
                auto const fragment_shader_stage_create_info = VkPipelineShaderStageCreateInfo{
                        .sType = vku::GetSType<VkPipelineShaderStageCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .module = fragment_shader_module,
                        .pName = "main",
                };

                auto const shader_stages = std::array{
                        vertex_shader_stage_create_info,
                        fragment_shader_stage_create_info,
                };

                auto const vertex_position_binding_description = VkVertexInputBindingDescription{
                        .binding = 0,
                        .stride = sizeof(Pos),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };
                auto const vertex_position_attribute = VkVertexInputAttributeDescription{
                        .location = 0,
                        .binding = vertex_position_binding_description.binding,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = 0,
                };

                auto const vertex_texuv_binding_description = VkVertexInputBindingDescription{
                        .binding = 1,
                        .stride = sizeof(Texuv),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };
                auto const texture_uv_attribute = VkVertexInputAttributeDescription{
                        .location = 1,
                        .binding = vertex_texuv_binding_description.binding,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = 0,
                };

                auto const vertex_color_binding_description = VkVertexInputBindingDescription{
                        .binding = 2,
                        .stride = sizeof(Color),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };

                auto const vertex_color_attribute = VkVertexInputAttributeDescription{
                        .location = 2,
                        .binding = vertex_color_binding_description.binding,
                        .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                        .offset = 0,
                };

                auto const vertex_binding_dexcriptions = std::array{
                        vertex_position_binding_description,
                        vertex_texuv_binding_description,
                        // vertex_color_binding_description,
                };
                auto const vertex_attribute_descritions = std::array{
                        vertex_position_attribute,
                        texture_uv_attribute,
                        // vertex_color_attribute,
                };

                auto const vertex_input_info = VkPipelineVertexInputStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineVertexInputStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .vertexBindingDescriptionCount = vertex_binding_dexcriptions.size(),
                        .pVertexBindingDescriptions = vertex_binding_dexcriptions.data(),
                        .vertexAttributeDescriptionCount = vertex_attribute_descritions.size(),
                        .pVertexAttributeDescriptions = vertex_attribute_descritions.data(),
                };

                auto const input_assembly_info = VkPipelineInputAssemblyStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineInputAssemblyStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                        .primitiveRestartEnable = VK_FALSE,
                };

                auto const viewport_state_info = VkPipelineViewportStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineViewportStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .viewportCount = 1,
                        .pViewports = &state->graphics_pipeline_viewport,
                        .scissorCount = 1,
                        .pScissors = &state->graphics_pipeline_scissor,
                };

                auto const rasterizer = VkPipelineRasterizationStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineRasterizationStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .depthClampEnable = VK_FALSE,
                        .rasterizerDiscardEnable = VK_FALSE,
                        .polygonMode = VK_POLYGON_MODE_FILL,
                        .cullMode = VK_CULL_MODE_BACK_BIT,
                        .frontFace = VK_FRONT_FACE_CLOCKWISE,
                        .depthBiasEnable = VK_FALSE,
                        .lineWidth = 1,
                };

                auto const multisampling = VkPipelineMultisampleStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineMultisampleStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                        .sampleShadingEnable = false,
                        .minSampleShading = 1,
                        .pSampleMask = nullptr,
                        .alphaToCoverageEnable = VK_FALSE,
                        .alphaToOneEnable = VK_FALSE,
                };

                auto const color_blend_attachment = VkPipelineColorBlendAttachmentState{
                        .blendEnable = VK_FALSE,
                        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                        .colorBlendOp = VK_BLEND_OP_ADD,
                        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                        .alphaBlendOp = VK_BLEND_OP_ADD,
                        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
                };

                auto const color_blending = VkPipelineColorBlendStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineColorBlendStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .logicOpEnable = VK_FALSE,
                        .attachmentCount = 1,
                        .pAttachments = &color_blend_attachment,
                        .blendConstants = {0.0f},
                };

                auto const dynamic_states = std::array{
                        VK_DYNAMIC_STATE_VIEWPORT,
                        VK_DYNAMIC_STATE_SCISSOR,
                        VK_DYNAMIC_STATE_LINE_WIDTH,
                };

                auto const dynamic_state = VkPipelineDynamicStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineDynamicStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .dynamicStateCount = dynamic_states.size(),
                        .pDynamicStates = dynamic_states.data(),
                };

                auto const depth_stencil = VkPipelineDepthStencilStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineDepthStencilStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .depthTestEnable = VK_TRUE,
                        .depthWriteEnable = VK_TRUE,
                        .depthCompareOp = VK_COMPARE_OP_LESS,
                        .depthBoundsTestEnable = VK_FALSE,
                        .stencilTestEnable = VK_TRUE,
                        .front = {},
                        .back = {},
                        .minDepthBounds = 0,
                        .maxDepthBounds = MAXFLOAT,
                };

                auto const ubo_binding = VkDescriptorSetLayoutBinding{
                        .binding = 0,
                        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                        .pImmutableSamplers = nullptr,
                };

                auto const sampler_binding = VkDescriptorSetLayoutBinding{
                        .binding = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .pImmutableSamplers = &state->sampler,
                };
                
                auto const sampler_create_info = VkSamplerCreateInfo{
                        .sType = vku::GetSType<VkSamplerCreateInfo>(),
                        .magFilter = VK_FILTER_LINEAR,
                        .minFilter = VK_FILTER_LINEAR,
                        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
                        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
                        .mipLodBias = 0.0f,
                        .anisotropyEnable = VK_FALSE,
                        //TODO: get max anisotropy from the limits of the physical device properties.
                        .maxAnisotropy = 0,
                        .compareEnable = VK_FALSE,
                        .compareOp = VK_COMPARE_OP_ALWAYS,
                        .minLod = 0.0f,
                        .maxLod = 0.0f,
                        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
                        .unnormalizedCoordinates = VK_FALSE,
                };

                if(auto result = state->vkCreateSampler(state->device, &sampler_create_info, state->vulkan_allocator, &state->sampler); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to create sampler:{}", string_VkResult(result)).c_str());
                }

                auto const descriptor_set_bindings = std::array{
                        ubo_binding,
                        sampler_binding,
                };

                auto const descriptor_set_info = VkDescriptorSetLayoutCreateInfo{
                        .sType = vku::GetSType<VkDescriptorSetLayoutCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .bindingCount = descriptor_set_bindings.size(),
                        .pBindings = descriptor_set_bindings.data(),
                };

                if (state->vkCreateDescriptorSetLayout(state->device, &descriptor_set_info, state->vulkan_allocator, &state->descriptor_set_layout) not_eq VK_SUCCESS) {
                        puts("unable to create descriptor set layout");
                        exit(1);
                }

                auto const ubo_push_constant_range = VkPushConstantRange{
                        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                        .offset = 0,
                        .size = sizeof(Ubo),
                };

                auto const pipeline_layout_info = VkPipelineLayoutCreateInfo{
                        .sType = vku::GetSType<VkPipelineLayoutCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .setLayoutCount = 1,
                        .pSetLayouts = &state->descriptor_set_layout,
                        .pushConstantRangeCount = 1,
                        .pPushConstantRanges = &ubo_push_constant_range,
                };

                if (state->vkCreatePipelineLayout(state->device, &pipeline_layout_info, state->vulkan_allocator, &state->pipeline_layout) not_eq VK_SUCCESS) {
                        puts("unable to create pipeline layout");
                        exit(1);
                }

                auto const pipeline_create_info = VkGraphicsPipelineCreateInfo{
                        .sType = vku::GetSType<VkGraphicsPipelineCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .stageCount = shader_stages.size(),
                        .pStages = shader_stages.data(),
                        .pVertexInputState = &vertex_input_info,
                        .pInputAssemblyState = &input_assembly_info,
                        .pViewportState = &viewport_state_info,
                        .pRasterizationState = &rasterizer,
                        .pMultisampleState = &multisampling,
                        .pDepthStencilState = &depth_stencil,
                        .pColorBlendState = &color_blending,
                        .pDynamicState = &dynamic_state,
                        .layout = state->pipeline_layout,
                        .renderPass = state->render_pass,
                };

                #include "ui_shaders.h"
                auto const ui_shader_stages = std::array{
                        VkPipelineShaderStageCreateInfo{
                                .sType = vku::GetSType<VkPipelineShaderStageCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                                .module = state->create_shader_module(ui_vert_spv_len, reinterpret_cast<u32 const *>(ui_vert_spv)),
                                .pName = "main",
                        },
                        VkPipelineShaderStageCreateInfo{
                                .sType = vku::GetSType<VkPipelineShaderStageCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                                .module =  state->create_shader_module(ui_frag_spv_len, reinterpret_cast<u32 const *>(ui_frag_spv)),
                                .pName = "main",
                        },
                };
                auto const ui_vertex_binding_dexcriptions = std::array{
                        VkVertexInputBindingDescription{
                                .binding = 0,
                                .stride = sizeof(glm::vec2),
                                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                        },
                        VkVertexInputBindingDescription{
                                .binding = 1,
                                .stride = sizeof(Texuv),
                                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                        },
                                VkVertexInputBindingDescription{
                                .binding = 2,
                                .stride = sizeof(Color),
                                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                        },
                };
                auto const ui_vertex_attribute_descritions = std::array{
                        VkVertexInputAttributeDescription{
                                .location = 0,
                                .binding = vertex_position_binding_description.binding,
                                .format = VK_FORMAT_R32G32_SFLOAT,
                                .offset = 0,
                        },
                        VkVertexInputAttributeDescription{
                                .location = 1,
                                .binding = vertex_texuv_binding_description.binding,
                                .format = VK_FORMAT_R32G32_SFLOAT,
                                .offset = 0,
                        },
                        VkVertexInputAttributeDescription{
                                .location = 2,
                                .binding = vertex_color_binding_description.binding,
                                .format = VK_FORMAT_R32G32B32A32_SFLOAT,
                                .offset = 0,
                        },
                };

                auto const ui_vertex_input_info = VkPipelineVertexInputStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineVertexInputStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .vertexBindingDescriptionCount = ui_vertex_binding_dexcriptions.size(),
                        .pVertexBindingDescriptions = ui_vertex_binding_dexcriptions.data(),
                        .vertexAttributeDescriptionCount = ui_vertex_attribute_descritions.size(),
                        .pVertexAttributeDescriptions = ui_vertex_attribute_descritions.data(),
                };

                auto const ui_sampler_binding = VkDescriptorSetLayoutBinding{
                        .binding = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        .descriptorCount = 1,
                        .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .pImmutableSamplers = &state->sampler,
                };
                auto const ui_descriptor_set_info = VkDescriptorSetLayoutCreateInfo{
                        .sType = vku::GetSType<VkDescriptorSetLayoutCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .bindingCount = 1,
                        .pBindings = &ui_sampler_binding,
                };
                if (state->vkCreateDescriptorSetLayout(state->device, &ui_descriptor_set_info, state->vulkan_allocator, &state->ui.descriptor_set_layout) not_eq VK_SUCCESS) {
                        puts("unable to create descriptor set layout");
                        exit(1);
                }
                auto const ui_pipeline_layout_info = VkPipelineLayoutCreateInfo{
                        .sType = vku::GetSType<VkPipelineLayoutCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .setLayoutCount = 1,
                        .pSetLayouts = &state->ui.descriptor_set_layout,
                        .pushConstantRangeCount = 0,
                        .pPushConstantRanges = nullptr,
                };
                if (state->vkCreatePipelineLayout(state->device, &ui_pipeline_layout_info, state->vulkan_allocator, &state->ui.pipeline_layout) not_eq VK_SUCCESS) {
                        puts("unable to create pipeline layout");
                        exit(1);
                }

                auto const ui_pipeline_create_info = VkGraphicsPipelineCreateInfo{
                        .sType = vku::GetSType<VkGraphicsPipelineCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .stageCount = ui_shader_stages.size(),
                        .pStages = ui_shader_stages.data(),
                        .pVertexInputState = &ui_vertex_input_info,
                        .pInputAssemblyState = &input_assembly_info,
                        .pViewportState = &viewport_state_info,
                        .pRasterizationState = &rasterizer,
                        .pMultisampleState = &multisampling,
                        .pDepthStencilState = &depth_stencil,
                        .pColorBlendState = &color_blending,
                        .pDynamicState = &dynamic_state,
                        .layout = state->ui.pipeline_layout,
                        .renderPass = state->render_pass,
                };

                VkGraphicsPipelineCreateInfo pipeline_infos[] = {pipeline_create_info, ui_pipeline_create_info};
                VkPipeline pipelines[2];
                puts("making graphics pipelines");
                if (state->vkCreateGraphicsPipelines(state->device, nullptr, 2, pipeline_infos, state->vulkan_allocator, pipelines) not_eq VK_SUCCESS) {
                        puts("unable to create graphics pipelines.");
                        exit(89888);
                }

                state->graphics_pipeline = pipelines[0];
                state->ui.pipeline = pipelines[1];

                VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
                state->instance_functions.vkGetPhysicalDeviceMemoryProperties(state->physical_device, &physical_device_memory_properties);

                state->create_depth_images();
                state->create_swapchain_frame_buffers();

                if (not state->vkCreateCommandPool) {
                        exit(30);
                }
                auto const command_pool_create_info = VkCommandPoolCreateInfo{
                        .sType = vku::GetSType<VkCommandPoolCreateInfo>(),
                        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                        .queueFamilyIndex = static_cast<uint32_t>(state->graphics_index),
                };
                if (state->vkCreateCommandPool(state->device, &command_pool_create_info, state->vulkan_allocator, &state->command_pool) not_eq VK_SUCCESS) {
                        puts("unable to create command pool.");
                        exit(40202);
                }

                settup_descriptors(state);

                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = state->command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = static_cast<uint32_t>(state->swapchain_image_count),
                };

                state->command_buffers = state->allocator.allocate_object<VkCommandBuffer>(state->swapchain_image_count);
                if (state->vkAllocateCommandBuffers(state->device, &command_buffer_allocate_info, state->command_buffers) not_eq VK_SUCCESS) {
                        puts("unable to allocate command buffers");
                        exit(420);
                }

                state->vkGetDeviceQueue(state->device, state->present_index, 0, &state->present_queue);
                state->vkDeviceWaitIdle(state->device);

                state->settup_sync();

                //Figure out memory requirements for an vertex and index buffer for the rest of the program.
                auto const test_buffer_create_info = VkBufferCreateInfo{
                        .sType = vku::GetSType<VkBufferCreateInfo>(),
                        .size = 0,
                        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                };
                VkBuffer test_buffer;
                puts("Ignore validation error");
                state->vkCreateBuffer(state->device, &test_buffer_create_info, state->vulkan_allocator, &test_buffer);
                state->vkGetBufferMemoryRequirements(state->device, test_buffer, &state->vertex_index_test_buffer_memory_requirements);
        }

        inline auto buffer_copy(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)const noexcept {
                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = 1,
                };
                VkCommandBuffer copy_command_buffer;
                if (vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &copy_command_buffer) not_eq VK_SUCCESS) {
                        puts("unable to allocate command buffers");
                        exit(420);
                }

                auto const begin_info = VkCommandBufferBeginInfo{.sType = vku::GetSType<VkCommandBufferBeginInfo>()};

                vkBeginCommandBuffer(copy_command_buffer, &begin_info);

                auto region = VkBufferCopy{.size = size};
                vkCmdCopyBuffer(copy_command_buffer, src_buffer, dst_buffer, 1, &region);

                vkEndCommandBuffer(copy_command_buffer);

                auto submit_info = VkSubmitInfo{
                        .sType = vku::GetSType<VkSubmitInfo>(),
                        .commandBufferCount = 1,
                        .pCommandBuffers = &copy_command_buffer,
                };

                vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
                vkQueueWaitIdle(graphics_queue);
                vkFreeCommandBuffers(device, command_pool, 1, &copy_command_buffer);
        };

        template <typename T> constexpr auto stage_and_copy_buffer(T *data, u64 size, VkBufferUsageFlags usage) noexcept{
                struct Buffer_Handle_and_Memory { VkDeviceMemory memory; VkBuffer buffer; }; 
                if(data == nullptr or size == 0){
                        return Buffer_Handle_and_Memory{VK_NULL_HANDLE, VK_NULL_HANDLE};
                }

                auto const buffer_size = size * sizeof(T);
                VkDeviceMemory staging_memory;
                VkBuffer staging_buffer;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | usage, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size, &staging_buffer, &staging_memory);

                void *buffer_data_staging_memory;
                vkMapMemory(device, staging_memory, 0, buffer_size, 0, &buffer_data_staging_memory);
                std::memcpy(buffer_data_staging_memory, data, buffer_size);
                vkUnmapMemory(device, staging_memory);

                VkDeviceMemory buffer_memory;
                VkBuffer buffer;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer_size, &buffer, &buffer_memory);
                buffer_copy(staging_buffer, buffer, buffer_size);

                // if(staging_buffer) device_functions.vkDestroyBuffer(device, staging_buffer, vulkan_allocator);
                // if(staging_memory) device_functions.vkFreeMemory(device, staging_memory, vulkan_allocator);

                return Buffer_Handle_and_Memory{buffer_memory, buffer};
        };

        constexpr auto load_static_mesh32( glm::vec3 const * vertices, u32 vertex_count, u32 const * indices, u32 index_count, glm::vec2 const * texuvs) noexcept{
                /* TODO: just put these all into one buffer and store the offsets of each thing so we aren't allocated tones of buffers when we have more things. */
                auto [position_memory, position_buffer] = stage_and_copy_buffer(vertices, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                auto [texuv_memory, texuv_buffer] = stage_and_copy_buffer(texuvs, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                auto [texcol_memory, texcol_buffer] = stage_and_copy_buffer(texuvs, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                auto [index_memory, index_buffer] = stage_and_copy_buffer(indices, index_count, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
                return Triangle_Mesh{
                        .vertex_count = vertex_count,
                        .vertex_buffer = position_buffer,
                        .vertex_buffer_memory = position_memory,
                        .index_count = index_count,
                        .index_buffer = index_buffer,
                        .index_buffer_memory = index_memory,
                        .texture_uv_buffer = texuv_buffer,
                        .texture_uv_buffer_memory = texuv_memory,
                };
        }

        inline auto create_exclusive_vertex_buffer(VkDeviceSize size){
                auto const info = VkBufferCreateInfo{
                        .sType = vku::GetSType<VkBufferCreateInfo>(),
                        .size = size,
                        .usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                };
                VkBuffer buffer;
                if(auto result = vkCreateBuffer(device, &info, vulkan_allocator, &buffer); result not_eq VK_SUCCESS){
                        puts(std::format("could not create exclusive vertex buffer {}", string_VkResult(result)).c_str());
                }
                return buffer;
        }

        inline auto create_device_local_buffers_memory_alloc_info(VkDeviceSize buffer_count, VkBuffer * buffers){
                VkMemoryRequirements buffer_memory_requirements[buffer_count];
                for(auto i = 0; i < buffer_count; ++i){
                        vkGetBufferMemoryRequirements(device, buffers[i], &buffer_memory_requirements[i]);
                }
                //TODO: make sure all the memry type bits and allignment and other stuff is the same.
                u32 memory_type_bits = 0;
                VkDeviceSize allocation_size = 0;
                for(auto i = 0; i < buffer_count; ++i){
                        allocation_size += calculate_buffer_offset(buffer_memory_requirements[i].size, vertex_index_test_buffer_memory_requirements.alignment);
                        memory_type_bits |= buffer_memory_requirements[i].memoryTypeBits;
                }
                return VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = allocation_size,
                        .memoryTypeIndex = find_memory_type(memory_type_bits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                };
        }

        constexpr auto calculate_buffer_offset(VkDeviceSize preceding_array_size, VkDeviceSize allignment) noexcept -> VkDeviceSize{
                auto const remainder = (preceding_array_size % allignment);
                return preceding_array_size + ((remainder > 0) * (allignment - remainder)); 
        }

        auto allocate_ui(u32 max_vertices = 0, u32 max_indices = 0) noexcept{
                ui.max_indices = max_indices;
                ui.max_vertices = max_vertices;
                ui.indices = create_exclusive_vertex_buffer(max_indices * sizeof(u32));
                ui.positions = create_exclusive_vertex_buffer(max_vertices * sizeof(glm::vec2));
                ui.texuvs = create_exclusive_vertex_buffer(max_vertices * sizeof(Texuv));
                ui.colors = create_exclusive_vertex_buffer(max_vertices * sizeof(Color));
                VkBuffer buffers[] = {ui.indices, ui.positions, ui.texuvs, ui.colors};
                auto const buffer_memory_alloc_info = create_device_local_buffers_memory_alloc_info(4, buffers);
                vkAllocateMemory(device, &buffer_memory_alloc_info, vulkan_allocator, &ui.memory);
                auto const allignment = vertex_index_test_buffer_memory_requirements.alignment;
                //TODO: this allocaiton + allignment stuff is certantly wrong but I can't be damned to fix it right now.
                auto const postion_offset = calculate_buffer_offset(max_indices * sizeof(u32), allignment);
                auto const texuvs_offset = calculate_buffer_offset(postion_offset + (max_vertices * sizeof(glm::vec2)), allignment);
                auto const color_offset = calculate_buffer_offset(texuvs_offset + (max_vertices * sizeof(Texuv)), allignment);
                vkBindBufferMemory(device, ui.indices, ui.memory, 0);
                vkBindBufferMemory(device, ui.positions, ui.memory, postion_offset);
                vkBindBufferMemory(device, ui.texuvs, ui.memory, texuvs_offset);
                vkBindBufferMemory(device, ui.colors, ui.memory, color_offset);
        }

        template<typename T>
        inline auto copy_data_to_device_buffer(VkDeviceSize size, T * data, VkBuffer device_buffer)noexcept{
                VkBuffer buffer;
                VkDeviceMemory memory;
                auto const buffer_size = size * sizeof(T);
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size, &buffer, &memory);
                void *buffer_data_staging_memory;
                vkMapMemory(device, memory, 0, buffer_size, 0, &buffer_data_staging_memory);
                std::memcpy(buffer_data_staging_memory, data, buffer_size);
                vkUnmapMemory(device, memory);
                buffer_copy(buffer, device_buffer, buffer_size);
                if(buffer)vkDestroyBuffer(device, buffer, vulkan_allocator);
                if(memory)vkFreeMemory(device, memory,vulkan_allocator);
        }

        auto load_ui_data( u32 index_count, u32 * indices, u32 vertex_count, glm::vec2 * positions, Texuv * texuvs, Color * colors){
                if(index_count > ui.max_indices){
                        puts(std::format("index count too high for ui data. max indices:{}, index count:{}", ui.max_indices, index_count).c_str());
                        return;
                }

                if(vertex_count > ui.max_vertices){
                        puts(std::format("vertex count too high for ui data. max vertices:{}, vertex count:{}", ui.max_vertices, vertex_count).c_str());
                        return;
                }

                ui.index_count = index_count;
                ui.vertex_count = vertex_count;
                if(indices) copy_data_to_device_buffer<u32>(index_count, indices, ui.indices);
                if(positions) copy_data_to_device_buffer<glm::vec2>(vertex_count, positions, ui.positions);
                if(texuvs) copy_data_to_device_buffer<Texuv>(vertex_count, texuvs, ui.texuvs);
                if(colors) copy_data_to_device_buffer<Color>(vertex_count, colors, ui.colors);
        }

        //Expects grey bitmap rn.
        inline void load_ui_texture(int width, int height, u8 * const data) noexcept{
                VkDeviceSize image_size = width * height * 4;
                VkBuffer staging_buffer;
                VkDeviceMemory staging_memory;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, image_size, &staging_buffer, &staging_memory);
                void * mapped_memory;
                vkMapMemory(device, staging_memory, 0, image_size, 0, &mapped_memory);
                //TODO: dont do this
                for(auto i = 0; i < width * height; ++i){
                        reinterpret_cast<u8 *>(mapped_memory)[i*4+0] = data[i];
                        reinterpret_cast<u8 *>(mapped_memory)[i*4+1] = data[i];
                        reinterpret_cast<u8 *>(mapped_memory)[i*4+2] = data[i];
                        reinterpret_cast<u8 *>(mapped_memory)[i*4+3] = data[i];
                }
                vkUnmapMemory(device, staging_memory);

                auto const image_extent = VkExtent3D{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

                auto const image_create_info = VkImageCreateInfo{
                        .sType = vku::GetSType<VkImageCreateInfo>(),
                        .imageType = VK_IMAGE_TYPE_2D,
                        .format = VK_FORMAT_R8G8B8A8_SRGB, 
                        .extent = image_extent,
                        .mipLevels = 1,
                        .arrayLayers = 1,
                        .samples = VK_SAMPLE_COUNT_1_BIT,
                        .tiling = VK_IMAGE_TILING_OPTIMAL,
                        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                };

                VkImage image;
                if(auto result = vkCreateImage(device, &image_create_info, vulkan_allocator, &image); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to create image:{}", string_VkResult(result)).c_str());
                }

                VkMemoryRequirements memory_requirements;
                vkGetImageMemoryRequirements(device, image, &memory_requirements); 
                
                auto const image_alloc_info = VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                };
                VkDeviceMemory memory;
                if(auto result = vkAllocateMemory(device, &image_alloc_info, vulkan_allocator, &memory); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to allocate memory for image:{}", string_VkResult(result)).c_str());
                        exit(420);
                }

                if(auto result = vkBindImageMemory(device, image, memory, 0); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to bind memory for image:{}", string_VkResult(result)).c_str());
                        exit(420);
                }

                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = 1,
                };
                VkCommandBuffer copy_command_buffer;
                if (vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &copy_command_buffer) not_eq VK_SUCCESS) {
                        puts("unable to allocate command buffers to load texture");
                }

                auto const begin_info = VkCommandBufferBeginInfo{.sType = vku::GetSType<VkCommandBufferBeginInfo>()};

                vkBeginCommandBuffer(copy_command_buffer, &begin_info);
                auto const region = VkBufferImageCopy{
                        .bufferRowLength = 0,
                        .bufferImageHeight = 0,
                        .imageSubresource = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .mipLevel = 0,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                        .imageOffset = {0,0,0},
                        .imageExtent = image_extent,
                };

                auto const barrier = VkImageMemoryBarrier{
                        .sType = vku::GetSType<VkImageMemoryBarrier>(),
                        .srcAccessMask = 0,
                        .dstAccessMask = 0,
                        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = image,
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                };
                vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,0, nullptr, 0, nullptr, 1, &barrier);
                vkCmdCopyBufferToImage(copy_command_buffer, staging_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
                auto submit_info = VkSubmitInfo{
                        .sType = vku::GetSType<VkSubmitInfo>(),
                        .commandBufferCount = 1,
                        .pCommandBuffers = &copy_command_buffer,
                };

                auto const image_to_sampled_barrier = VkImageMemoryBarrier{
                        .sType = vku::GetSType<VkImageMemoryBarrier>(),
                        .srcAccessMask = 0,
                        .dstAccessMask = 0,
                        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = image,
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                };
                vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,0, nullptr, 0, nullptr, 1, &image_to_sampled_barrier);
                vkEndCommandBuffer(copy_command_buffer);

                vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
                vkQueueWaitIdle(graphics_queue);
                vkFreeCommandBuffers(device, command_pool, 1, &copy_command_buffer);

                auto const view_create_info = VkImageViewCreateInfo{
                        .sType = vku::GetSType<VkImageViewCreateInfo>(),
                        .image = image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D,
                        .format = image_create_info.format,
                        .subresourceRange = image_to_sampled_barrier.subresourceRange,
                };

                VkImageView view; 
                if (auto result = vkCreateImageView(device, &view_create_info, vulkan_allocator, &view);result not_eq VK_SUCCESS) {
                        puts(std::format("Unable to create image views:{}", string_VkResult(result)).c_str());
                }


                auto const image_descriptor_info = VkDescriptorImageInfo{
                        .sampler = sampler,
                        .imageView = view,
                        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                };

                auto const write_sampler_descriptor = VkWriteDescriptorSet{
                        .sType = vku::GetSType<VkWriteDescriptorSet>(),
                        .dstSet = ui.descriptor_set,
                        .dstBinding = 1,
                        .dstArrayElement = 0,
                        .descriptorCount = 1,
                        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        .pImageInfo = &image_descriptor_info,
                        .pBufferInfo = nullptr,
                        .pTexelBufferView = nullptr,
                };
                vkUpdateDescriptorSets(device, 1, &write_sampler_descriptor, 0, nullptr);

                ui.texture = Texture{
                        .image = image,
                        .memory = memory,
                        .view = view,
                };
        }

        inline void load_terrain_texture(int width, int height, u8 * const data) noexcept{
                VkDeviceSize image_size = width * height * 4;
                VkBuffer staging_buffer;
                VkDeviceMemory staging_memory;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, image_size, &staging_buffer, &staging_memory);
                void * mapped_memory;
                vkMapMemory(device, staging_memory, 0, image_size, 0, &mapped_memory);
                memcpy(mapped_memory, data, image_size);
                vkUnmapMemory(device, staging_memory);

                auto const image_extent = VkExtent3D{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};

                auto const image_create_info = VkImageCreateInfo{
                        .sType = vku::GetSType<VkImageCreateInfo>(),
                        .imageType = VK_IMAGE_TYPE_2D,
                        .format = VK_FORMAT_R8G8B8A8_SRGB, 
                        .extent = image_extent,
                        .mipLevels = 1,
                        .arrayLayers = 1,
                        .samples = VK_SAMPLE_COUNT_1_BIT,
                        .tiling = VK_IMAGE_TILING_OPTIMAL,
                        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                };

                VkImage image;
                if(auto result = vkCreateImage(device, &image_create_info, vulkan_allocator, &image); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to create image:{}", string_VkResult(result)).c_str());
                }

                VkMemoryRequirements memory_requirements;
                vkGetImageMemoryRequirements(device, image, &memory_requirements); 
                
                auto const image_alloc_info = VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                };
                VkDeviceMemory memory;
                if(auto result = vkAllocateMemory(device, &image_alloc_info, vulkan_allocator, &memory); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to allocate memory for image:{}", string_VkResult(result)).c_str());
                        exit(420);
                }

                if(auto result = vkBindImageMemory(device, image, memory, 0); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to bind memory for image:{}", string_VkResult(result)).c_str());
                        exit(420);
                }

                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = 1,
                };
                VkCommandBuffer copy_command_buffer;
                if (vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &copy_command_buffer) not_eq VK_SUCCESS) {
                        puts("unable to allocate command buffers to load texture");
                }

                auto const begin_info = VkCommandBufferBeginInfo{.sType = vku::GetSType<VkCommandBufferBeginInfo>()};

                vkBeginCommandBuffer(copy_command_buffer, &begin_info);
                auto const region = VkBufferImageCopy{
                        .bufferRowLength = 0,
                        .bufferImageHeight = 0,
                        .imageSubresource = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .mipLevel = 0,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                        .imageOffset = {0,0,0},
                        .imageExtent = image_extent,
                };

                auto const barrier = VkImageMemoryBarrier{
                        .sType = vku::GetSType<VkImageMemoryBarrier>(),
                        .srcAccessMask = 0,
                        .dstAccessMask = 0,
                        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = image,
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                };
                vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,0, nullptr, 0, nullptr, 1, &barrier);
                vkCmdCopyBufferToImage(copy_command_buffer, staging_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
                auto submit_info = VkSubmitInfo{
                        .sType = vku::GetSType<VkSubmitInfo>(),
                        .commandBufferCount = 1,
                        .pCommandBuffers = &copy_command_buffer,
                };

                auto const image_to_sampled_barrier = VkImageMemoryBarrier{
                        .sType = vku::GetSType<VkImageMemoryBarrier>(),
                        .srcAccessMask = 0,
                        .dstAccessMask = 0,
                        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                        .image = image,
                        .subresourceRange = {
                                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                .baseMipLevel = 0,
                                .levelCount = 1,
                                .baseArrayLayer = 0,
                                .layerCount = 1,
                        },
                };
                vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,0, nullptr, 0, nullptr, 1, &image_to_sampled_barrier);
                vkEndCommandBuffer(copy_command_buffer);

                vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
                vkQueueWaitIdle(graphics_queue);
                vkFreeCommandBuffers(device, command_pool, 1, &copy_command_buffer);

                auto const view_create_info = VkImageViewCreateInfo{
                        .sType = vku::GetSType<VkImageViewCreateInfo>(),
                        .image = image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D,
                        .format = image_create_info.format,
                        .subresourceRange = image_to_sampled_barrier.subresourceRange,
                };

                VkImageView view; if (auto result = vkCreateImageView(device, &view_create_info, vulkan_allocator, &view);result not_eq VK_SUCCESS) {
                        puts(std::format("Unable to create image views:{}", string_VkResult(result)).c_str());
                }

                VkDescriptorSet const descriptor_sets[] = {
                        terrain_descriptor_set,
                        player_descriptor_set,
                };

                auto const image_descriptor_info = VkDescriptorImageInfo{
                        .sampler = sampler,
                        .imageView = view,
                        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                };

                for(auto && set : descriptor_sets){
                        auto const write_sampler_descriptor = VkWriteDescriptorSet{
                                .sType = vku::GetSType<VkWriteDescriptorSet>(),
                                .dstSet = set,
                                .dstBinding = 1,
                                .dstArrayElement = 0,
                                .descriptorCount = 1,
                                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                .pImageInfo = &image_descriptor_info,
                                .pBufferInfo = nullptr,
                                .pTexelBufferView = nullptr,
                        };
                        vkUpdateDescriptorSets(device, 1, &write_sampler_descriptor, 0, nullptr);
                }

                terrain_texture = Texture{
                        .image = image,
                        .memory = memory,
                        .view = view,
                };
        }

        inline void record_command_buffers(u32 swapchain_image_index) const noexcept {
                VkClearValue const clear_values[] = { 
                        VkClearValue{.color = VkClearColorValue{.float32 = {1, 0, 1, 0}}},
                        VkClearValue{.depthStencil = VkClearDepthStencilValue{.depth = 1, .stencil = 0}},
                };

                auto &command_buffer = command_buffers[swapchain_image_index];
                auto &framebuffer = frame_buffers[swapchain_image_index];

                auto const command_buffer_begin_info = VkCommandBufferBeginInfo{
                        .sType = vku::GetSType<VkCommandBufferBeginInfo>(),
                };

                vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
                
                auto const render_pass_begin_info = VkRenderPassBeginInfo{
                        .sType = vku::GetSType<VkRenderPassBeginInfo>(),
                        .renderPass = render_pass,
                        .framebuffer = framebuffer,
                        .renderArea = VkRect2D{.offset{0, 0}, .extent = swapchain_info.imageExtent},
                        .clearValueCount = 2,
                        .pClearValues = clear_values,
                };
                vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);
                auto scisor  = graphics_pipeline_scissor;

                vkCmdSetViewport(command_buffer, 0, 1, &graphics_pipeline_viewport);
                vkCmdSetScissor(command_buffer, 0, 1, &scisor);


                VkDeviceSize offsets[3] = {0};

                if(ui.index_count > 0){
                        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ui.pipeline);
                        VkBuffer const ui_mesh_buffers[] = {ui.positions, ui.texuvs, ui.colors};
                        vkCmdBindVertexBuffers(command_buffer, 0, 3, ui_mesh_buffers, offsets);
                        vkCmdBindIndexBuffer(command_buffer, ui.indices, 0, VK_INDEX_TYPE_UINT32);
                        vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ui.pipeline_layout, 0, 1, &ui.descriptor_set, 0, nullptr);
                        vkCmdDrawIndexed(command_buffer, ui.index_count, 1, 0, 0, 0);
                }

                vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);
                VkBuffer const player_mesh_buffers[] = {player_mesh.vertex_buffer, player_mesh.texture_uv_buffer};
                vkCmdBindVertexBuffers(command_buffer, 0, 2, player_mesh_buffers, offsets);
                vkCmdBindIndexBuffer(command_buffer, player_mesh.index_buffer, 0, VK_INDEX_TYPE_UINT32);
                memcpy(player_ubo_mapped_memory, &player_ubo, sizeof(Ubo));
                vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &player_descriptor_set, 0, nullptr);
                vkCmdDrawIndexed(command_buffer, player_mesh.index_count, 1, 0, 0, 0);


                VkBuffer const terrain_mesh_buffers[] = {terrain_mesh.vertex_buffer, terrain_mesh.texture_uv_buffer};
                vkCmdBindVertexBuffers(command_buffer, 0, 2, terrain_mesh_buffers, offsets);
                vkCmdBindIndexBuffer(command_buffer, terrain_mesh.index_buffer, 0, VK_INDEX_TYPE_UINT32);
                memcpy(terrain_ubo_mapped_memory, &terrain_ubo, sizeof(Ubo));
                vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &terrain_descriptor_set, 0, nullptr);
                vkCmdDrawIndexed(command_buffer, terrain_mesh.index_count, 1, 0, 0, 0);

                vkCmdEndRenderPass(command_buffer);
                vkEndCommandBuffer(command_buffer);
        }

        inline auto draw_frame() noexcept {
                static bool should_recreate_swapchain = false;
                auto current_frame = frame_count % max_frames_in_flieght;

                // vkDeviceWaitIdle(device);
                auto &image_in_flieght_fence = image_in_flieght_fences[current_frame];

                if (vkWaitForFences(device, 1, &image_in_flieght_fence, VK_TRUE, 1000) not_eq VK_SUCCESS) {
                        puts("unable to wait for frame fence");
                }
                if (vkResetFences(device, 1, &image_in_flieght_fence) not_eq VK_SUCCESS) {
                        puts("unable to reset fence");
                }

                auto &image_available_semaphore = image_available_semaphores[current_frame];
                u32 swapchain_image_index;
                if (auto result = vkAcquireNextImageKHR(device, swapchain, 1000, image_available_semaphore, image_in_flieght_fence, &swapchain_image_index); result not_eq VK_SUCCESS) {
                        if(result == VK_SUBOPTIMAL_KHR){
                                puts("sub");
                        }else{
                                puts(std::format("problems aquireing next swapchian image index: {}", string_VkResult(result)).c_str());
                        }
                }
                record_command_buffers(swapchain_image_index);

                if (vkWaitForFences(device, 1, &image_in_flieght_fence, VK_TRUE, 1000) not_eq VK_SUCCESS) {
                        puts("unable to wait for frame fence");
                }
                if (vkResetFences(device, 1, &image_in_flieght_fence) not_eq VK_SUCCESS) {
                        puts("unable to reset fence");
                }

                auto &render_finished_semaphore = render_finished_semaphores[current_frame];
                VkPipelineStageFlags wait_dst_stage_mask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                auto const submit_info = VkSubmitInfo{
                        .sType = vku::GetSType<VkSubmitInfo>(),
                        .waitSemaphoreCount = 1,
                        .pWaitSemaphores = &image_available_semaphore,
                        .pWaitDstStageMask = &wait_dst_stage_mask,
                        .commandBufferCount = 1,
                        .pCommandBuffers = &command_buffers[swapchain_image_index],
                        .signalSemaphoreCount = 1,
                        .pSignalSemaphores = &render_finished_semaphore,
                };

                vkQueueSubmit(graphics_queue, 1, &submit_info, image_in_flieght_fence);

                auto const present_info = VkPresentInfoKHR{
                        .sType = vku::GetSType<VkPresentInfoKHR>(),
                        .pNext = nullptr,
                        .waitSemaphoreCount = 1,
                        .pWaitSemaphores = &render_finished_semaphore,
                        .swapchainCount = 1,
                        .pSwapchains = &swapchain,
                        .pImageIndices = &swapchain_image_index,
                };
                if (auto result = vkQueuePresentKHR(present_queue, &present_info);result not_eq VK_SUCCESS) {
                        if(result == VK_SUBOPTIMAL_KHR){

                        }else{
                                puts(std::format("problems on vkQueeuPresentKHR:{}", string_VkResult(result)).c_str());
                        }
                }

                if (vkQueueWaitIdle(present_queue) not_eq VK_SUCCESS) {
                        puts("could not wait for some reason");
                }

                ++frame_count;
        };
};

