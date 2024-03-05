#pragma once

#include "defines.h"
#include "includes.h"
#include "mesh.cpp"
#include "vulkan/vulkan_core.h"


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

struct Device_Functions {
        PFN_vkGetDeviceQueue vkGetDeviceQueue;
        PFN_vkCreateSampler vkCreateSampler;
        PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
        PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
        PFN_vkCreateImageView vkCreateImageView;
        PFN_vkCreateShaderModule vkCreateShaderModule;
        PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
        PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
        PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
        PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
        PFN_vkCreateRenderPass vkCreateRenderPass;
        PFN_vkCreateImage vkCreateImage;
        PFN_vkBindBufferMemory vkBindBufferMemory;
        PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
        PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
        PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
        PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
        PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
        PFN_vkFreeCommandBuffers vkFreeCommandBuffers; 
        PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
        PFN_vkEndCommandBuffer vkEndCommandBuffer;
        PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
        PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
        PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
        PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
        PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
        PFN_vkCmdSetScissor vkCmdSetScissor;
        PFN_vkCmdSetViewport vkCmdSetViewport;
        PFN_vkCmdBindPipeline vkCmdBindPipeline;
        PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
        PFN_vkCmdPushConstants vkCmdPushConstants;
        PFN_vkCmdDispatch vkCmdDispatch;
        PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
        PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
        PFN_vkCmdDraw vkCmdDraw;
        PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
        PFN_vkQueueSubmit vkQueueSubmit;
        PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
        PFN_vkMapMemory vkMapMemory;
        PFN_vkUnmapMemory vkUnmapMemory;
        PFN_vkAllocateMemory vkAllocateMemory;
        PFN_vkBindImageMemory vkBindImageMemory;
        PFN_vkCreateFramebuffer vkCreateFramebuffer;
        PFN_vkCreateCommandPool vkCreateCommandPool;
        PFN_vkCreateBuffer vkCreateBuffer;
        PFN_vkCreateComputePipelines vkCreateComputePipelines;
        PFN_vkCreateSemaphore vkCreateSemaphore;
        PFN_vkCreateFence vkCreateFence;
        PFN_vkWaitForFences vkWaitForFences;
        PFN_vkResetFences vkResetFences;
        PFN_vkDestroyFence vkDestroyFence;
        PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
        PFN_vkQueuePresentKHR vkQueuePresentKHR;
        PFN_vkQueueWaitIdle vkQueueWaitIdle;
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
        std::puts(std::format("validation layer {} :{} \n", string_VkDebugUtilsMessageSeverityFlagsEXT(messageSeverity), pCallbackData->pMessage).c_str());
        return VK_FALSE;
}

template <typename T> static T load_vulkan_function(const char *name) { return reinterpret_cast<T>(glfwGetInstanceProcAddress(nullptr, name)); }

template <typename T> static T load_vulkan_function(VkInstance instance, const char *name) { return reinterpret_cast<T>(glfwGetInstanceProcAddress(instance, name)); }


//TODO: this needs to be completly refactored so that the models and textures can be packed together and rendered using minimal graphics pipelines.
struct Render_State {
        std::pmr::polymorphic_allocator<u8> allocator;
        VkAllocationCallbacks *vulkan_allocator = nullptr;
        char **enabled_layer_names;
        u32 layer_count;
        char **enabled_extension_names;
        u32 extension_count;
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
        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
        Device_Functions device_functions;
        VkSwapchainKHR swapchain;
        VkExtent2D swapchain_extent;
        u32 swapchain_image_count;
        VkRenderPass render_pass;
        VkViewport viewport;
        VkDescriptorSetLayout descriptor_set_layout;
        VkDescriptorPool descriptor_pool;
        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;
        VkQueue graphics_queue;
        VkQueue present_queue;
        VkFramebuffer *frame_buffers;
        VkCommandBuffer *command_buffers;
        VkCommandPool command_pool;
        VkSemaphore *image_available_semaphores;
        VkSemaphore *render_finished_semaphores;
        VkFence *image_in_flieght_fences;
        VkImage *depth_images;
        VkDeviceMemory *depth_images_memory;
        VkImageView *depth_images_views;
        VkSampler sampler;

        s64 current_frame = 0;
        Ubo player_ubo;
        Ubo terrain_ubo;
        Ubo gui_ubo;

        u32 vertex_count;
        VkBuffer vertex_buffer;
        VkDeviceMemory vertex_buffer_memory;
        u32 index_count;
        VkBuffer index_buffer;
        VkDeviceMemory index_buffer_memory;
        Line_Mesh bounding_sphere_mesh;
        Line_Mesh debug_mesh;
        Line_Mesh selected_object_mesh;
        Texture terrain_texture;
        Triangle_Mesh terrain_mesh;
        Triangle_Mesh player_mesh;
        Triangle_Mesh ui_mesh;
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
        VkDeviceMemory ui_ubo_memory;
        void *ui_ubo_mapped_memory;
        VkDescriptorSet ui_descriptor_set;

        VkDescriptorSet texture_sampler_descriptor_set;

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
                device_functions.vkGetDeviceQueue = load_device_function<PFN_vkGetDeviceQueue>("vkGetDeviceQueue");
                device_functions.vkCreateSampler = load_device_function<PFN_vkCreateSampler>("vkCreateSampler");
                device_functions.vkCreateSwapchainKHR = load_device_function<PFN_vkCreateSwapchainKHR>("vkCreateSwapchainKHR");
                device_functions.vkGetSwapchainImagesKHR = load_device_function<PFN_vkGetSwapchainImagesKHR>("vkGetSwapchainImagesKHR");
                device_functions.vkCreateImageView = load_device_function<PFN_vkCreateImageView>("vkCreateImageView");
                device_functions.vkCreateShaderModule = load_device_function<PFN_vkCreateShaderModule>("vkCreateShaderModule");
                device_functions.vkCreateDescriptorPool = load_device_function<PFN_vkCreateDescriptorPool>("vkCreateDescriptorPool");
                device_functions.vkCreateDescriptorSetLayout = load_device_function<PFN_vkCreateDescriptorSetLayout>("vkCreateDescriptorSetLayout");
                device_functions.vkCreatePipelineLayout = load_device_function<PFN_vkCreatePipelineLayout>("vkCreatePipelineLayout");
                device_functions.vkCreateGraphicsPipelines = load_device_function<PFN_vkCreateGraphicsPipelines>("vkCreateGraphicsPipelines");
                device_functions.vkCreateRenderPass = load_device_function<PFN_vkCreateRenderPass>("vkCreateRenderPass");
                device_functions.vkCreateImage = load_device_function<PFN_vkCreateImage>("vkCreateImage");
                device_functions.vkBindBufferMemory = load_device_function<PFN_vkBindBufferMemory>("vkBindBufferMemory");
                device_functions.vkGetBufferMemoryRequirements = load_device_function<PFN_vkGetBufferMemoryRequirements>("vkGetBufferMemoryRequirements");
                device_functions.vkGetImageMemoryRequirements = load_device_function<PFN_vkGetImageMemoryRequirements>("vkGetImageMemoryRequirements");
                device_functions.vkAllocateDescriptorSets = load_device_function<PFN_vkAllocateDescriptorSets>("vkAllocateDescriptorSets");
                device_functions.vkUpdateDescriptorSets = load_device_function<PFN_vkUpdateDescriptorSets>("vkUpdateDescriptorSets");
                device_functions.vkAllocateCommandBuffers = load_device_function<PFN_vkAllocateCommandBuffers>("vkAllocateCommandBuffers");
                device_functions.vkFreeCommandBuffers = load_device_function<PFN_vkFreeCommandBuffers>("vkFreeCommandBuffers");
                device_functions.vkBeginCommandBuffer = load_device_function<PFN_vkBeginCommandBuffer>("vkBeginCommandBuffer");
                device_functions.vkEndCommandBuffer = load_device_function<PFN_vkEndCommandBuffer>("vkEndCommandBuffer");
                device_functions.vkCmdCopyBuffer = load_device_function<PFN_vkCmdCopyBuffer>("vkCmdCopyBuffer");
                device_functions.vkCmdPipelineBarrier = load_device_function<PFN_vkCmdPipelineBarrier>("vkCmdPipelineBarrier");
                device_functions.vkCmdCopyBufferToImage = load_device_function<PFN_vkCmdCopyBufferToImage>("vkCmdCopyBufferToImage");
                device_functions.vkCmdBeginRenderPass = load_device_function<PFN_vkCmdBeginRenderPass>("vkCmdBeginRenderPass");
                device_functions.vkCmdEndRenderPass = load_device_function<PFN_vkCmdEndRenderPass>("vkCmdEndRenderPass");
                device_functions.vkCmdSetScissor = load_device_function<PFN_vkCmdSetScissor>("vkCmdSetScissor");
                device_functions.vkCmdSetViewport = load_device_function<PFN_vkCmdSetViewport>("vkCmdSetViewport");
                device_functions.vkCmdBindPipeline = load_device_function<PFN_vkCmdBindPipeline>("vkCmdBindPipeline");
                device_functions.vkCmdBindDescriptorSets = load_device_function<PFN_vkCmdBindDescriptorSets>("vkCmdBindDescriptorSets");
                device_functions.vkCmdDispatch = load_device_function<PFN_vkCmdDispatch>("vkCmdDispatch");
                device_functions.vkCmdBindVertexBuffers = load_device_function<PFN_vkCmdBindVertexBuffers>("vkCmdBindVertexBuffers");
                device_functions.vkCmdBindIndexBuffer = load_device_function<PFN_vkCmdBindIndexBuffer>("vkCmdBindIndexBuffer");
                device_functions.vkCmdPushConstants = load_device_function<PFN_vkCmdPushConstants>("vkCmdPushConstants");
                device_functions.vkCmdDraw = load_device_function<PFN_vkCmdDraw>("vkCmdDraw");
                device_functions.vkCmdDrawIndexed = load_device_function<PFN_vkCmdDrawIndexed>("vkCmdDrawIndexed");
                device_functions.vkQueueSubmit = load_device_function<PFN_vkQueueSubmit>("vkQueueSubmit");
                device_functions.vkDeviceWaitIdle = load_device_function<PFN_vkDeviceWaitIdle>("vkDeviceWaitIdle");
                device_functions.vkMapMemory = load_device_function<PFN_vkMapMemory>("vkMapMemory");
                device_functions.vkUnmapMemory = load_device_function<PFN_vkUnmapMemory>("vkUnmapMemory");
                device_functions.vkAllocateMemory = load_device_function<PFN_vkAllocateMemory>("vkAllocateMemory");
                device_functions.vkGetImageMemoryRequirements = load_device_function<PFN_vkGetImageMemoryRequirements>("vkGetImageMemoryRequirements");
                device_functions.vkBindImageMemory = load_device_function<PFN_vkBindImageMemory>("vkBindImageMemory");
                device_functions.vkCreateFramebuffer = load_device_function<PFN_vkCreateFramebuffer>("vkCreateFramebuffer");
                device_functions.vkCreateCommandPool = load_device_function<PFN_vkCreateCommandPool>("vkCreateCommandPool");
                device_functions.vkCreateBuffer = load_device_function<PFN_vkCreateBuffer>("vkCreateBuffer");
                device_functions.vkCreateComputePipelines = load_device_function<PFN_vkCreateComputePipelines>("vkCreateComputePipelines");
                device_functions.vkCreateSemaphore = load_device_function<PFN_vkCreateSemaphore>("vkCreateSemaphore");
                device_functions.vkCreateFence = load_device_function<PFN_vkCreateFence>("vkCreateFence");
                device_functions.vkAcquireNextImageKHR = load_device_function<PFN_vkAcquireNextImageKHR>("vkAcquireNextImageKHR");
                device_functions.vkQueuePresentKHR = load_device_function<PFN_vkQueuePresentKHR>("vkQueuePresentKHR");
                device_functions.vkQueueWaitIdle = load_device_function<PFN_vkQueueWaitIdle>("vkQueueWaitIdle");
                device_functions.vkWaitForFences = load_device_function<PFN_vkWaitForFences>("vkWaitForFences");
                device_functions.vkResetFences = load_device_function<PFN_vkResetFences>("vkResetFences");
                device_functions.vkDestroyFence = load_device_function<PFN_vkDestroyFence>("vkDestroyFence");
        }

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
                device_functions.vkCreateBuffer(device, &buffer_create_info, vulkan_allocator, buffer);

                VkMemoryRequirements buffer_memory_requirements;
                device_functions.vkGetBufferMemoryRequirements(device, *buffer, &buffer_memory_requirements);

                auto const buffer_memory_alloc_info = VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = buffer_memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(buffer_memory_requirements.memoryTypeBits, memory_properties),
                };
                device_functions.vkAllocateMemory(device, &buffer_memory_alloc_info, vulkan_allocator, buffer_memory);
                device_functions.vkBindBufferMemory(device, *buffer, *buffer_memory, 0);
        };

        static inline auto settup_descriptors(Render_State *state) noexcept{
                auto const ubo_buffer_size = sizeof(Ubo);

                // auto const terrain_uniform_descriptor_pool_size = ;
                constexpr u8 model_count = 3;
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

                if (auto const result = state->device_functions.vkCreateDescriptorPool(state->device, &descriptor_pool_create_info, state->vulkan_allocator, &state->descriptor_pool); result not_eq VK_SUCCESS) {
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
                        &state->terrain_descriptor_set,
                        &state->player_descriptor_set,
                        &state->ui_descriptor_set,
                };

                for(auto i = 0; i < model_count;++i){
                        if (auto const result = state->device_functions.vkAllocateDescriptorSets(state->device, &descriptor_set_allocation_info, descriptor_sets[i]); result not_eq VK_SUCCESS) {
                                puts(std::format("unable to allocate descriptor set[{}] {}", i, string_VkResult(result)).c_str());
                                exit(420);
                        }
                }

                VkBuffer* ubo_buffers[] = {
                        &state->terrain_ubo_buffer,
                        &state->player_ubo_buffer,
                        &state->ui_ubo_buffer,
                };

                VkDeviceMemory * ubo_buffer_memory[] = {
                        &state->terrain_ubo_memory,
                        &state->player_ubo_memory,
                        &state->ui_ubo_memory,
                };

                void ** ubo_mapped_buffer_memory[] = {
                        &state->terrain_ubo_mapped_memory,
                        &state->player_ubo_mapped_memory,
                        &state->ui_ubo_mapped_memory,
                };

                for(auto i = 0; i < model_count;++i){
                        state->create_buffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, ubo_buffer_size, ubo_buffers[i], ubo_buffer_memory[i]);
                        state->device_functions.vkMapMemory(state->device, *ubo_buffer_memory[i], 0, ubo_buffer_size, 0, ubo_mapped_buffer_memory[i]);
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
                        state->device_functions.vkUpdateDescriptorSets(state->device, 1, &write, 0, nullptr);
                }
        }

        static inline auto initalize(Render_State *state, GLFWwindow *window) noexcept {
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

                std::vector<char const *> extension_names = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

                u32 glfw_extension_count;
                auto const glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
                extension_names.insert(extension_names.end(), glfw_extensions, glfw_extensions + glfw_extension_count);

                for (auto &&name : extension_names) {
                        std::puts(name);
                }

                std::vector<char const *> layer_names = {"VK_LAYER_KHRONOS_validation"};
                auto instantce_info = VkInstanceCreateInfo{
                        .sType = vku::GetSType<VkInstanceCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .pApplicationInfo = &app_info,
                        .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
                        .ppEnabledLayerNames = layer_names.data(),
                        .enabledExtensionCount = static_cast<uint32_t>(extension_names.size()),
                        .ppEnabledExtensionNames = extension_names.data(),
                };

                auto const vkCreateInstance = load_vulkan_function<PFN_vkCreateInstance>("vkCreateInstance");

                if (vkCreateInstance(&instantce_info, state->vulkan_allocator, &state->instance) not_eq VK_SUCCESS) {
                        std::puts("Unable to create vulkan instance.\n");
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

                VkDebugUtilsMessengerEXT debug_utils_messenger;
                if (not state->instance_functions.vkCreateDebugUtilsMessengerEXT) {
                        std::puts("unable to load debug utils messenger.\n");
                } else if (state->instance_functions.vkCreateDebugUtilsMessengerEXT(state->instance, &debug_messenger_info, state->vulkan_allocator, &debug_utils_messenger) not_eq VK_SUCCESS) {
                        std::puts("unable to create debug utils messenger.\n");
                }

                if (glfwCreateWindowSurface(state->instance, window, state->vulkan_allocator, &state->window_surface) not_eq VK_SUCCESS) {
                        std::puts("Unable to get surface");
                        std::terminate();
                }

                uint32_t device_count = 0;
                if (state->instance_functions.vkEnumeratePhysicalDevices(state->instance, &device_count, nullptr) not_eq VK_SUCCESS or device_count == 0) {
                        std::puts("no physical device.");
                        std::terminate();
                }
                auto physical_devices = std::vector<VkPhysicalDevice>(device_count);
                state->instance_functions.vkEnumeratePhysicalDevices(state->instance, &device_count, physical_devices.data());

                // TODO: choose a proper rendering device.
                state->physical_device = physical_devices[0];

                auto const [graphics_index, present_index, compute_index] = std::invoke([&] {
                        uint32_t property_count = 0;
                        state->instance_functions.vkGetPhysicalDeviceQueueFamilyProperties(state->physical_device, &property_count, nullptr);
                        VkQueueFamilyProperties properties[property_count];
                        state->instance_functions.vkGetPhysicalDeviceQueueFamilyProperties(state->physical_device, &property_count, properties);

                        struct {
                                int32_t graphics_index;
                                int32_t present_index;
                                int32_t compute_index;
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
                                        std::puts(std::format("device surface support for queue index {} is not supported", i).c_str());
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
                });

                float graphics_queue_prioraties[] = {1.0f};

                char const *device_extensions[1] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

                state->instance_functions.vkGetPhysicalDeviceFeatures(state->physical_device, &state->physical_device_features);
                state->instance_functions.vkGetPhysicalDeviceMemoryProperties(state->physical_device, &state->physical_device_memory_properties);

                auto const queue_create_infos = std::array{
                        VkDeviceQueueCreateInfo{
                                .sType = vku::GetSType<VkDeviceQueueCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .queueFamilyIndex = static_cast<uint32_t>(graphics_index),
                                .queueCount = 1,
                                .pQueuePriorities = graphics_queue_prioraties,
                        },
                };

                auto const device_create_info = VkDeviceCreateInfo{
                        .sType = vku::GetSType<VkDeviceCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .queueCreateInfoCount = queue_create_infos.size(),
                        .pQueueCreateInfos = queue_create_infos.data(),
                        .enabledLayerCount = static_cast<uint32_t>(layer_names.size()),
                        .ppEnabledLayerNames = layer_names.data(),
                        .enabledExtensionCount = 1,
                        .ppEnabledExtensionNames = device_extensions,
                        .pEnabledFeatures = &state->physical_device_features,
                };

                if (state->instance_functions.vkCreateDevice(state->physical_device, &device_create_info, state->vulkan_allocator, &state->device) not_eq VK_SUCCESS) {
                        puts("unable to create device");
                        exit(42);
                }
                puts("loading device functions.");
                state->load_device_functions();
                auto const & device_functions = state->device_functions;

                device_functions.vkGetDeviceQueue(state->device, graphics_index, 0, &state->graphics_queue);

                VkSurfaceCapabilitiesKHR surface_capabilities;
                if (state->instance_functions.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(state->physical_device, state->window_surface, &surface_capabilities) not_eq VK_SUCCESS) {
                        puts("unable to get surface capabilities");
                        std::exit(39393);
                }

                uint32_t format_count = 0;
                if (state->instance_functions.vkGetPhysicalDeviceSurfaceFormatsKHR(state->physical_device, state->window_surface, &format_count, nullptr) not_eq VK_SUCCESS) {
                        std::exit(39393);
                }
                VkSurfaceFormatKHR formats[format_count];
                state->instance_functions.vkGetPhysicalDeviceSurfaceFormatsKHR(state->physical_device, state->window_surface, &format_count, formats);
                auto const surface_format = formats[0];

                uint32_t present_mode_count = 0;
                if (state->instance_functions.vkGetPhysicalDeviceSurfacePresentModesKHR(state->physical_device, state->window_surface, &present_mode_count, nullptr) not_eq VK_SUCCESS) {
                        std::exit(39393);
                }
                VkPresentModeKHR present_modes[present_mode_count];
                state->instance_functions.vkGetPhysicalDeviceSurfacePresentModesKHR(state->physical_device, state->window_surface, &format_count, present_modes);
                auto const surface_present_mode = present_modes[0];

                int32_t window_width, window_height;
                glfwGetFramebufferSize(window, &window_width, &window_height);

                auto min_image_extent = surface_capabilities.minImageExtent;
                auto max_image_extent = surface_capabilities.maxImageExtent;
                state->swapchain_extent = VkExtent2D{
                        .width = std::clamp<uint32_t>(static_cast<uint32_t>(window_width), min_image_extent.width, max_image_extent.width),
                        .height = std::clamp<uint32_t>(static_cast<uint32_t>(window_height), min_image_extent.height, max_image_extent.height),
                };

                uint32_t image_array_layers = 0;

                auto sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
                auto queue_family_indices = std::vector<uint32_t>();
                queue_family_indices.push_back(graphics_index);
                queue_family_indices.push_back(compute_index);
                if (graphics_index not_eq present_index) {
                        queue_family_indices.push_back(present_index);
                        sharing_mode = VK_SHARING_MODE_CONCURRENT;
                }

                auto const swapchain_create_info = VkSwapchainCreateInfoKHR{
                        .sType = vku::GetSType<VkSwapchainCreateInfoKHR>(),
                        .pNext = nullptr,
                        .flags = {},
                        .surface = state->window_surface,
                        .minImageCount = surface_capabilities.minImageCount + 1,
                        .imageFormat = surface_format.format,
                        .imageColorSpace = surface_format.colorSpace,
                        .imageExtent = state->swapchain_extent,
                        .imageArrayLayers = 1,
                        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                        .imageSharingMode = sharing_mode,
                        .queueFamilyIndexCount = static_cast<uint32_t>(queue_family_indices.size()),
                        .pQueueFamilyIndices = queue_family_indices.data(),
                        .preTransform = surface_capabilities.currentTransform,
                        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                        .presentMode = surface_present_mode,
                        .clipped = VK_TRUE,
                        .oldSwapchain = nullptr,
                };

                if (device_functions.vkCreateSwapchainKHR(state->device, &swapchain_create_info, state->vulkan_allocator, &state->swapchain) not_eq VK_SUCCESS) {
                        std::puts("unable to create swapchain");
                        std::exit(2323232);
                }

                if (device_functions.vkGetSwapchainImagesKHR(state->device, state->swapchain, &state->swapchain_image_count, nullptr) not_eq VK_SUCCESS) {
                        std::exit(2424242);
                }

                auto swapchain_images = std::vector<VkImage>(state->swapchain_image_count);
                device_functions.vkGetSwapchainImagesKHR(state->device, state->swapchain, &state->swapchain_image_count, swapchain_images.data());

                auto swapchain_image_views = std::vector<VkImageView>(state->swapchain_image_count);
                for (auto i = 0; i < state->swapchain_image_count; ++i) {
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
                        if (device_functions.vkCreateImageView(state->device, &image_view_create_info, state->vulkan_allocator, &swapchain_image_views[i]) not_eq VK_SUCCESS) {
                                std::puts("unable to create swapchain image views");
                                std::exit(52);
                        }
                }

                auto const color_attachment = VkAttachmentDescription{
                        .flags = {},
                        .format = surface_format.format,
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
                        std::puts("unable to find depth format");
                        exit(7894238);
                }

                auto const depth_attachment = VkAttachmentDescription{
                        .flags = {},
                        .format = depth_format.value(),
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

                if (device_functions.vkCreateRenderPass(state->device, &render_pass_create_info, state->vulkan_allocator, &state->render_pass) not_eq VK_SUCCESS) {
                        std::puts("unable to create render pass");
                        std::exit(1);
                }

                auto create_shader_module = [&device_functions, state](std::filesystem::path path) -> VkShaderModule {
                        std::puts("creating shader module");
                        auto file = std::ifstream(path.string(), std::ios::ate | std::ios::binary);
                        auto const fileSize = (size_t)file.tellg();
                        auto buffer = std::vector<char>(fileSize);
                        file.seekg(0);
                        file.read(buffer.data(), fileSize);
                        std::puts(std::format("loading shader {} with bytecount {}", path.string(), buffer.size()).c_str());
                        auto const shader_module_create_info = VkShaderModuleCreateInfo{
                                .sType = vku::GetSType<VkShaderModuleCreateInfo>(),
                                .pNext = nullptr,
                                .flags = {},
                                .codeSize = buffer.size(),
                                .pCode = reinterpret_cast<uint32_t const *>(buffer.data()),
                        };
                        VkShaderModule module;
                        if (device_functions.vkCreateShaderModule(state->device, &shader_module_create_info, state->vulkan_allocator, &module)) {
                                std::puts("unable to create shader module");
                                std::exit(420);
                        }
                        return module;
                };

                auto const vertex_shader_module = create_shader_module("shader.vert.spv");
                auto const vertex_shader_stage_create_info = VkPipelineShaderStageCreateInfo{
                        .sType = vku::GetSType<VkPipelineShaderStageCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .stage = VK_SHADER_STAGE_VERTEX_BIT,
                        .module = vertex_shader_module,
                        .pName = "main",
                };

                auto const fragment_shader_module = create_shader_module("shader.frag.spv");
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
                        .stride = sizeof(glm::vec3),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };
                auto const vertex_position_attribute = VkVertexInputAttributeDescription{
                        .location = 0,
                        .binding = vertex_position_binding_description.binding,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = 0,
                };

                auto const vertex_texture_uv_binding_description = VkVertexInputBindingDescription{
                        .binding = 1,
                        .stride = sizeof(glm::vec2),
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                };
                auto const texture_uv_attribute = VkVertexInputAttributeDescription{
                        .location = 1,
                        .binding = vertex_texture_uv_binding_description.binding,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = 0,
                };

                auto const vertex_binding_dexcriptions = std::array{
                        vertex_position_binding_description,
                        vertex_texture_uv_binding_description
                };
                auto const vertex_attribute_descritions = std::array{
                        vertex_position_attribute,
                        texture_uv_attribute,
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

                state->viewport = VkViewport{
                        .x = 0,
                        .y = 0,
                        .width = static_cast<float>(state->swapchain_extent.width),
                        .height = static_cast<float>(state->swapchain_extent.height),
                        .minDepth = 0,
                        .maxDepth = 1,
                };

                auto const scissor = VkRect2D{
                        .offset = {0, 0},
                        .extent = state->swapchain_extent,
                };

                auto const viewport_state_info = VkPipelineViewportStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineViewportStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .viewportCount = 1,
                        .pViewports = &state->viewport,
                        .scissorCount = 1,
                        .pScissors = &scissor,
                };

                auto const rasterizer = VkPipelineRasterizationStateCreateInfo{
                        .sType = vku::GetSType<VkPipelineRasterizationStateCreateInfo>(),
                        .pNext = nullptr,
                        .flags = {},
                        .depthClampEnable = VK_FALSE,
                        .rasterizerDiscardEnable = VK_FALSE,
                        // .polygonMode = VK_POLYGON_MODE_POINT,
                        .polygonMode = VK_POLYGON_MODE_FILL,
                        // .polygonMode = VK_POLYGON_MODE_LINE,
                        .cullMode = VK_CULL_MODE_BACK_BIT,
                        .frontFace = VK_FRONT_FACE_CLOCKWISE,
                        .depthBiasEnable = VK_FALSE,
                        .lineWidth = 40,
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
                        .stencilTestEnable = VK_FALSE,
                        .front = {},
                        .back = {},
                        .minDepthBounds = 0,
                        .maxDepthBounds = 1,
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

                if(auto result = device_functions.vkCreateSampler(state->device, &sampler_create_info, state->vulkan_allocator, &state->sampler); result not_eq VK_SUCCESS){
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

                if (device_functions.vkCreateDescriptorSetLayout(state->device, &descriptor_set_info, state->vulkan_allocator, &state->descriptor_set_layout) not_eq VK_SUCCESS) {
                        std::puts("unable to create descriptor set layout");
                        std::exit(1);
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

                if (device_functions.vkCreatePipelineLayout(state->device, &pipeline_layout_info, state->vulkan_allocator, &state->pipeline_layout) not_eq VK_SUCCESS) {
                        std::puts("unable to create pipeline layout");
                        std::exit(1);
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

                std::puts("making graphics pipeline");
                if (device_functions.vkCreateGraphicsPipelines(state->device, nullptr, 1, &pipeline_create_info, state->vulkan_allocator, &state->graphics_pipeline) not_eq VK_SUCCESS) {
                        std::puts("unable to create graphics pipelines.");
                        std::exit(89888);
                }

                VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
                state->instance_functions.vkGetPhysicalDeviceMemoryProperties(state->physical_device, &physical_device_memory_properties);

                auto const create_device_local_image = [&](VkFormat format, uint32_t mip_levels, VkImageTiling tiling, VkImageUsageFlags usage) {
                        auto const image_info = VkImageCreateInfo{
                                .sType = vku::GetSType<VkImageCreateInfo>(),
                                .imageType = VK_IMAGE_TYPE_2D,
                                .format = format,
                                .extent = VkExtent3D{static_cast<uint32_t>(window_width), static_cast<uint32_t>(window_height), 1},
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
                        if (device_functions.vkCreateImage(state->device, &image_info, state->vulkan_allocator, &image) not_eq VK_SUCCESS) {
                                std::puts("unable to create vulkan image");
                                std::exit(420);
                        }
                        VkMemoryRequirements image_memory_requirements;
                        device_functions.vkGetImageMemoryRequirements(state->device, image, &image_memory_requirements);

                        VkMemoryAllocateInfo memory_allocate_info{
                                .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                                .allocationSize = image_memory_requirements.size,
                                .memoryTypeIndex = state->find_memory_type(image_memory_requirements.memoryTypeBits, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) 
                        };

                        VkDeviceMemory image_memory;
                        if (device_functions.vkAllocateMemory(state->device, &memory_allocate_info, state->vulkan_allocator, &image_memory) not_eq VK_SUCCESS) {
                                puts("unable to allocate memory for an image");
                                exit(420);
                        }

                        if (device_functions.vkBindImageMemory(state->device, image, image_memory, 0) not_eq VK_SUCCESS) {
                                exit(420);
                        }

                        struct {
                                VkImage image;
                                VkDeviceMemory memory;
                        } image_stuff{image, image_memory};
                        return image_stuff;
                };

                state->depth_images = state->allocator.allocate_object<VkImage>(state->swapchain_image_count);
                state->depth_images_memory = state->allocator.allocate_object<VkDeviceMemory>(state->swapchain_image_count);
                state->depth_images_views = state->allocator.allocate_object<VkImageView>(state->swapchain_image_count); 
                for(auto i = 0; i < state->swapchain_image_count; ++i){
                        auto [depth_image, depth_image_memory] = create_device_local_image(depth_format.value(), 1, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
                        state->depth_images[i] = depth_image;
                        state->depth_images_memory[i] = depth_image_memory;
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
                                .format = depth_format.value(),
                                .components = {},
                                .subresourceRange = depth_subresource_range,
                        };
                        if (state->device_functions.vkCreateImageView(state->device, &depth_image_view_create_info, state->vulkan_allocator, &state->depth_images_views[i]) not_eq VK_SUCCESS) {
                                std::exit(520);
                        }
                }

                state->frame_buffers = state->allocator.allocate_object<VkFramebuffer>(state->swapchain_image_count);
                for (auto i = 0; i < state->swapchain_image_count; ++i) {
                        auto const attachments = std::array{
                                swapchain_image_views[i],
                                state->depth_images_views[i],
                        };

                        auto const creat_info = VkFramebufferCreateInfo{
                                .sType = vku::GetSType<VkFramebufferCreateInfo>(),
                                .flags = {},
                                .renderPass = state->render_pass,
                                .attachmentCount = attachments.size(),
                                .pAttachments = attachments.data(),
                                .width = state->swapchain_extent.width,
                                .height = state->swapchain_extent.height,
                                .layers = 1,
                        };
                        if (device_functions.vkCreateFramebuffer(state->device, &creat_info, state->vulkan_allocator, &state->frame_buffers[i]) not_eq VK_SUCCESS) {
                                exit(630);
                        }
                }

                if (not device_functions.vkCreateCommandPool) {
                        std::exit(30);
                }
                auto const command_pool_create_info = VkCommandPoolCreateInfo{
                        .sType = vku::GetSType<VkCommandPoolCreateInfo>(),
                        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                        .queueFamilyIndex = static_cast<uint32_t>(graphics_index),
                };
                if (device_functions.vkCreateCommandPool(state->device, &command_pool_create_info, state->vulkan_allocator, &state->command_pool) not_eq VK_SUCCESS) {
                        std::puts("unable to create command pool.");
                        std::exit(40202);
                }

                settup_descriptors(state);

                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = state->command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = static_cast<uint32_t>(state->swapchain_image_count),
                };

                state->command_buffers = state->allocator.allocate_object<VkCommandBuffer>(state->swapchain_image_count);
                if (device_functions.vkAllocateCommandBuffers(state->device, &command_buffer_allocate_info, state->command_buffers) not_eq VK_SUCCESS) {
                        std::puts("unable to allocate command buffers");
                        std::exit(420);
                }

                for (auto i = 0; i < state->swapchain_image_count; ++i) {
                        auto const &command_buffer = state->command_buffers[i];
                        auto const &frame_buffer = state->frame_buffers[i];
                }

                auto current_frame = 0;
                state->image_available_semaphores = state->allocator.allocate_object<VkSemaphore>(state->swapchain_image_count);
                state->render_finished_semaphores = state->allocator.allocate_object<VkSemaphore>(state->swapchain_image_count);
                state->image_in_flieght_fences = state->allocator.allocate_object<VkFence>(state->swapchain_image_count);
                for (auto i = 0; i < max_frames_in_flieght; ++i) {
                        auto empty_semaphore_create_info = VkSemaphoreCreateInfo{
                                .sType = vku::GetSType<VkSemaphoreCreateInfo>(),
                        };
                        if (device_functions.vkCreateSemaphore(state->device, &empty_semaphore_create_info, state->vulkan_allocator, &state->image_available_semaphores[i]) not_eq VK_SUCCESS) std::exit(3939);
                        if (device_functions.vkCreateSemaphore(state->device, &empty_semaphore_create_info, state->vulkan_allocator, &state->render_finished_semaphores[i]) not_eq VK_SUCCESS) std::exit(3939);
                        auto fence_create_info = VkFenceCreateInfo{
                                .sType = vku::GetSType<VkFenceCreateInfo>(),
                                .flags = VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT,
                        };
                        if (device_functions.vkCreateFence(state->device, &fence_create_info, state->vulkan_allocator, &state->image_in_flieght_fences[i]) not_eq VK_SUCCESS) {
                                std::exit(765987);
                        }
                }

                std::puts(std::format(" present index = {}", present_index).c_str());
                device_functions.vkGetDeviceQueue(state->device, present_index, 0, &state->present_queue);
                state->device_functions.vkDeviceWaitIdle(state->device);
        }

        inline auto buffer_copy(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)const noexcept {
                auto const command_buffer_allocate_info = VkCommandBufferAllocateInfo{
                        .sType = vku::GetSType<VkCommandBufferAllocateInfo>(),
                        .commandPool = command_pool,
                        .level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        .commandBufferCount = 1,
                };
                VkCommandBuffer copy_command_buffer;
                if (device_functions.vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &copy_command_buffer) not_eq VK_SUCCESS) {
                        std::puts("unable to allocate command buffers");
                        std::exit(420);
                }

                auto const begin_info = VkCommandBufferBeginInfo{.sType = vku::GetSType<VkCommandBufferBeginInfo>()};

                device_functions.vkBeginCommandBuffer(copy_command_buffer, &begin_info);

                auto region = VkBufferCopy{.size = size};
                device_functions.vkCmdCopyBuffer(copy_command_buffer, src_buffer, dst_buffer, 1, &region);

                device_functions.vkEndCommandBuffer(copy_command_buffer);

                auto submit_info = VkSubmitInfo{
                        .sType = vku::GetSType<VkSubmitInfo>(),
                        .commandBufferCount = 1,
                        .pCommandBuffers = &copy_command_buffer,
                };

                device_functions.vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
                device_functions.vkDeviceWaitIdle(device);
        };


        template <typename T> 
        constexpr auto stage_and_copy_buffer(T *data, u64 size, VkBufferUsageFlags usage) noexcept{
                struct Buffer_Handle_and_Memory { VkDeviceMemory memory; VkBuffer buffer; }; 
                if(data == nullptr or size == 0){
                        return Buffer_Handle_and_Memory{VK_NULL_HANDLE, VK_NULL_HANDLE};
                }

                auto const buffer_size = size * sizeof(T);
                VkDeviceMemory staging_memory;
                VkBuffer staging_buffer;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT | usage, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, buffer_size, &staging_buffer, &staging_memory);

                void *buffer_data_staging_memory;
                device_functions.vkMapMemory(device, staging_memory, 0, buffer_size, 0, &buffer_data_staging_memory);
                std::memcpy(buffer_data_staging_memory, data, buffer_size);
                device_functions.vkUnmapMemory(device, staging_memory);

                VkDeviceMemory buffer_memory;
                VkBuffer buffer;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer_size, &buffer, &buffer_memory);
                buffer_copy(staging_buffer, buffer, buffer_size);

                return Buffer_Handle_and_Memory{buffer_memory, buffer};
        };
        
        constexpr auto load_mesh32( glm::vec3 const * vertices, u32 vertex_count, u32 const * indices, u32 index_count, glm::vec2 const * texture_uvs) noexcept{
                /* TODO: just put these all into one buffer and store the offsets of each thing so we aren't allocated tones of buffers when we have more things. */
                auto [vertex_memory, vertex_buffer] = stage_and_copy_buffer(vertices, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                auto [texture_uv_memory, texture_uv_buffer] = stage_and_copy_buffer(texture_uvs, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

                auto [index_memory, index_buffer] = stage_and_copy_buffer(indices, index_count, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

                return Triangle_Mesh{
                        .vertex_count = vertex_count,
                        .vertex_buffer = vertex_buffer,
                        .vertex_buffer_memory = vertex_memory,
                        .index_count = index_count,
                        .index_buffer = index_buffer,
                        .index_buffer_memory = index_memory,
                        .texture_uv_buffer = texture_uv_buffer,
                        .texture_uv_buffer_memory = texture_uv_memory,
                };
        }

        constexpr auto load_mesh64(glm::vec3 const * vertices, u64 vertex_count, u64 const * indices, u64 index_count)noexcept{
                auto [vertex_memory, vertex_buffer] = stage_and_copy_buffer(vertices, vertex_count, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
                auto [index_memory, index_buffer] = stage_and_copy_buffer(indices, index_count, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
                return Triangle_Mesh{
                        .vertex_count = vertex_count,
                        .vertex_buffer = vertex_buffer,
                        .vertex_buffer_memory = vertex_memory,
                        .index_count = index_count,
                        .index_buffer = index_buffer,
                        .index_buffer_memory = index_memory,
                };
        }

        inline void load_terrain_texture(int width, int height, u8 * const data) noexcept{
                VkDeviceSize image_size = width * height * 4;
                VkBuffer staging_buffer;
                VkDeviceMemory staging_memory;
                create_buffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, image_size, &staging_buffer, &staging_memory);
                void * mapped_memory;
                device_functions.vkMapMemory(device, staging_memory, 0, image_size, 0, &mapped_memory);
                memcpy(mapped_memory, data, image_size);
                device_functions.vkUnmapMemory(device, staging_memory);

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
                if(auto result = device_functions.vkCreateImage(device, &image_create_info, vulkan_allocator, &image); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to create image:{}", string_VkResult(result)).c_str());
                }

                VkMemoryRequirements memory_requirements;
                device_functions.vkGetImageMemoryRequirements(device, image, &memory_requirements); 
                
                auto const image_alloc_info = VkMemoryAllocateInfo{
                        .sType = vku::GetSType<VkMemoryAllocateInfo>(),
                        .allocationSize = memory_requirements.size,
                        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
                };
                VkDeviceMemory memory;
                if(auto result = device_functions.vkAllocateMemory(device, &image_alloc_info, vulkan_allocator, &memory); result not_eq VK_SUCCESS){
                        puts(std::format("Unable to allocate memory for image:{}", string_VkResult(result)).c_str());
                        exit(420);
                }

                if(auto result = device_functions.vkBindImageMemory(device, image, memory, 0); result not_eq VK_SUCCESS){
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
                if (device_functions.vkAllocateCommandBuffers(device, &command_buffer_allocate_info, &copy_command_buffer) not_eq VK_SUCCESS) {
                        std::puts("unable to allocate command buffers to load texture");
                }

                auto const begin_info = VkCommandBufferBeginInfo{.sType = vku::GetSType<VkCommandBufferBeginInfo>()};

                device_functions.vkBeginCommandBuffer(copy_command_buffer, &begin_info);
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
                device_functions.vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,0, nullptr, 0, nullptr, 1, &barrier);
                device_functions.vkCmdCopyBufferToImage(copy_command_buffer, staging_buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
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
                device_functions.vkCmdPipelineBarrier(copy_command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,0, nullptr, 0, nullptr, 1, &image_to_sampled_barrier);
                device_functions.vkEndCommandBuffer(copy_command_buffer);

                device_functions.vkQueueSubmit(graphics_queue, 1, &submit_info, nullptr);
                device_functions.vkQueueWaitIdle(graphics_queue);
                device_functions.vkFreeCommandBuffers(device, command_pool, 1, &copy_command_buffer);

                auto const view_create_info = VkImageViewCreateInfo{
                        .sType = vku::GetSType<VkImageViewCreateInfo>(),
                        .image = image,
                        .viewType = VK_IMAGE_VIEW_TYPE_2D,
                        .format = image_create_info.format,
                        .subresourceRange = image_to_sampled_barrier.subresourceRange,
                };

                VkImageView view; if (auto result = device_functions.vkCreateImageView(device, &view_create_info, vulkan_allocator, &view);result not_eq VK_SUCCESS) {
                        puts(std::format("Unable to create image views:{}", string_VkResult(result)).c_str());
                }

                VkDescriptorSet const descriptor_sets[] = {
                        terrain_descriptor_set,
                        player_descriptor_set,
                        ui_descriptor_set,
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
                        device_functions.vkUpdateDescriptorSets(device, 1, &write_sampler_descriptor, 0, nullptr);
                }

                terrain_texture = Texture{
                        .image = image,
                        .memory = memory,
                        .view = view,
                };
        }

        //TODO: this is leaking memory.
        inline void record_command_buffers(u32 swapchain_image_index) const noexcept {
                VkClearValue const clear_values[] = { 
                        VkClearValue{.color = VkClearColorValue{.float32 = {1, 0, 1, 0}}},
                        VkClearValue{.depthStencil = VkClearDepthStencilValue{.depth = 1, .stencil = 0}},
                };
                auto const viewport_scissor = VkRect2D{
                        .offset = {0, 0},
                        .extent = swapchain_extent,
                };

                auto &command_buffer = command_buffers[swapchain_image_index];
                auto &framebuffer = frame_buffers[swapchain_image_index];

                auto const command_buffer_begin_info = VkCommandBufferBeginInfo{
                        .sType = vku::GetSType<VkCommandBufferBeginInfo>(),
                };

                device_functions.vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);
                
                auto const render_pass_begin_info = VkRenderPassBeginInfo{
                        .sType = vku::GetSType<VkRenderPassBeginInfo>(),
                        .renderPass = render_pass,
                        .framebuffer = framebuffer,
                        .renderArea = VkRect2D{.offset{0, 0}, .extent = swapchain_extent},
                        .clearValueCount = 2,
                        .pClearValues = clear_values,
                };
                device_functions.vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

                device_functions.vkCmdSetViewport(command_buffer, 0, 1, &viewport);
                device_functions.vkCmdSetScissor(command_buffer, 0, 1, &viewport_scissor);
                device_functions.vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

                VkDeviceSize offsets[2] = {0};

                VkBuffer const player_mesh_buffers[] = {player_mesh.vertex_buffer, player_mesh.texture_uv_buffer };
                device_functions.vkCmdBindVertexBuffers(command_buffer, 0, 2, player_mesh_buffers, offsets);
                device_functions.vkCmdBindIndexBuffer(command_buffer, player_mesh.index_buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
                device_functions.vkCmdBindDescriptorSets(command_buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &player_descriptor_set, 0, nullptr);
                device_functions.vkCmdDrawIndexed(command_buffer, player_mesh.index_count, 1, 0, 0, 0);

                // VkBuffer const terrain_mesh_buffers[] = {terrain_mesh.vertex_buffer, terrain_mesh.texture_uv_buffer };
                // device_functions.vkCmdBindVertexBuffers(command_buffer, 0, 2, terrain_mesh_buffers, offsets);
                // device_functions.vkCmdBindIndexBuffer(command_buffer, terrain_mesh.index_buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
                // device_functions.vkCmdBindDescriptorSets(command_buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &terrain_descriptor_set, 0, nullptr);
                // device_functions.vkCmdDrawIndexed(command_buffer, terrain_mesh.index_count, 1, 0, 0, 0);

                // VkBuffer const ui_mesh_buffers[] = {ui_mesh.vertex_buffer, ui_mesh.texture_uv_buffer};
                // device_functions.vkCmdBindVertexBuffers(command_buffer, 0, 2, ui_mesh_buffers, offsets);
                // device_functions.vkCmdBindIndexBuffer(command_buffer, ui_mesh.index_buffer, 0, VkIndexType::VK_INDEX_TYPE_UINT32);
                // device_functions.vkCmdBindDescriptorSets(command_buffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &ui_descriptor_set, 0, nullptr);
                // device_functions.vkCmdDrawIndexed(command_buffer, ui_mesh.index_count, 1, 0, 0, 0);

                device_functions.vkCmdEndRenderPass(command_buffer);
                device_functions.vkEndCommandBuffer(command_buffer);
        }

        inline auto draw_frame() noexcept {
                device_functions.vkDeviceWaitIdle(device);
                auto &image_in_flieght_fence = image_in_flieght_fences[current_frame];
                auto &image_available_semaphore = image_available_semaphores[current_frame];
                auto &render_finished_semaphore = render_finished_semaphores[current_frame];

                if (device_functions.vkWaitForFences(device, 1, &image_in_flieght_fence, VK_TRUE, UINT64_MAX) not_eq VK_SUCCESS) {
                        std::puts("unable to wait for frame fence");
                }
                if (device_functions.vkResetFences(device, 1, &image_in_flieght_fence) not_eq VK_SUCCESS) {
                        std::puts("unable to reset fence");
                }

                uint32_t swapchain_image_index;
                if (device_functions.vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, image_available_semaphore, image_in_flieght_fence, &swapchain_image_index) not_eq VK_SUCCESS) {
                        std::puts("unable to aquire next swapchian image index.");
                }

                record_command_buffers(swapchain_image_index);

                if (device_functions.vkWaitForFences(device, 1, &image_in_flieght_fence, VK_TRUE, UINT64_MAX) not_eq VK_SUCCESS) {
                        std::puts("unable to wait for frame fence");
                }
                if (device_functions.vkResetFences(device, 1, &image_in_flieght_fence) not_eq VK_SUCCESS) {
                        std::puts("unable to reset fence");
                }

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

                device_functions.vkQueueSubmit(graphics_queue, 1, &submit_info, image_in_flieght_fence);

                auto const present_info = VkPresentInfoKHR{
                        .sType = vku::GetSType<VkPresentInfoKHR>(),
                        .pNext = nullptr,
                        .waitSemaphoreCount = 1,
                        .pWaitSemaphores = &render_finished_semaphore,
                        .swapchainCount = 1,
                        .pSwapchains = &swapchain,
                        .pImageIndices = &swapchain_image_index,
                };
                if (device_functions.vkQueuePresentKHR(present_queue, &present_info) not_eq VK_SUCCESS) {
                        std::puts("unable to present");
                }

                if (device_functions.vkQueueWaitIdle(present_queue) not_eq VK_SUCCESS) {
                        std::puts("could not wait for some reason");
                }

                current_frame = (current_frame + 1) % max_frames_in_flieght;
        };
};






// constexpr auto load_texture(Render_State * state, char const * path, u32 mip_levels) noexcept -> std::optional<Image>{
//         int width, height, c;
//         auto image_data = stbi_load(path, &width, &height, &c, STBI_rgb_alpha);
//
//         auto texture_format = std::optional<VkFormat>();
//         for (auto format : {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}) {
//                 VkFormatProperties physical_device_format_properties;
//                 state->instance_functions.vkGetPhysicalDeviceFormatProperties(state->physical_device, format, &physical_device_format_properties); 
//                 if ((physical_device_format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) > 0) {
//                         texture_format = format;
//                         break;
//                 }
//         }
//
//         if(not texture_format){
//                 puts(std::format("unable to find a suitable format for texture {}", path).c_str());
//         }
//
//         auto const image_info = VkImageCreateInfo{
//                 .sType = vku::GetSType<VkImageCreateInfo>(),
//                 .imageType = VK_IMAGE_TYPE_2D,
//                 .format = texture_format.value(),
//                 .extent = VkExtent3D{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1},
//                 .mipLevels = mip_levels,
//                 .arrayLayers = 1,
//                 .samples = VK_SAMPLE_COUNT_1_BIT,
//                 .tiling = VK_IMAGE_TILING_OPTIMAL,
//                 .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
//                 .queueFamilyIndexCount = 0,
//                 .pQueueFamilyIndices = nullptr,
//                 .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
//         };
//
//         VkImage image;
//         if (vkCreateImage(state->device, &image_info, state->vulkan_allocator, &image) not_eq VK_SUCCESS) {
//                 std::puts("unable to create vulkan image");
//                 std::exit(420);
//         }
//         VkMemoryRequirements image_memory_requirements;
//         vkGetImageMemoryRequirements(state->device, image, &image_memory_requirements);
//
//         VkMemoryAllocateInfo memory_allocate_info{
//                 .sType = vku::GetSType<VkMemoryAllocateInfo>(),
//                 .allocationSize = image_memory_requirements.size,
//                 .memoryTypeIndex = find_memory_type(state,image_memory_requirements.memoryTypeBits, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) 
//         };
//
//         VkDeviceMemory image_memory;
//         if (vkAllocateMemory(state->device, &memory_allocate_info, state->vulkan_allocator, &image_memory) not_eq VK_SUCCESS) {
//                 puts("unable to allocate memory for an image");
//                 exit(420);
//         }
//
//         if (vkBindImageMemory(state->device, image, image_memory, 0) not_eq VK_SUCCESS) {
//                 exit(420);
//         }
//         stbi_image_free(image_data);
//         return Image{
//                 .image = image,
//                 .memory = image_memory,
//                 .view = image_view
//         };
// }


