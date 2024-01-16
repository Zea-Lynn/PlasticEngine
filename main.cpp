import renderer;

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>

int main() noexcept {
        if (not glfwInit()) {
                std::puts("Could not initialize GLFW");
                exit(123);
        }

        const auto width = 512 * 2, height = 512 * 2;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        const auto window = glfwCreateWindow(width, height, "plastic", nullptr, nullptr);
        if (not window) {
                std::puts("Could get GLFW window");
                exit(123);
        }

        if (glfwVulkanSupported() == GLFW_API_UNAVAILABLE) {
                std::puts("Vulkan is not supported.\n");
                exit(123);
        }

        auto allocator = nullptr;

        Render_State render_state;
        initalize(&render_state, window);
        while (not glfwWindowShouldClose(window)) {
                glfwPollEvents();
                draw_frame(&render_state);

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                while (std::getchar() not_eq '\n')
                        ;
        }
}
