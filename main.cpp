
#include "includes.h"
#include "mesh.cpp"
#include "renderer.cpp"

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


        {
                auto [points, indices] = create_platform();
                render_state.terrain_mesh = load_mesh32(&render_state, points, 4, indices, 6);
        }
        {
                auto [points, indices] = create_icosphere(); 
                render_state.player_mesh = load_mesh32(&render_state, points.data(), points.size(), indices.data(), indices.size());
        }

        glfwSetWindowUserPointer(window, &render_state);
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                auto render_state = (Render_State *)glfwGetWindowUserPointer(window);
                if (key == GLFW_KEY_ESCAPE) {
                        exit(0);
                }
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow *window) { exit(0); });

        for (auto i = 0; i < render_state.swapchain_image_count; ++i) record_command_buffers(&render_state, i);

        double mouse_start_x = -1, mouse_start_y = -1;

        auto const view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 100.0f));
        auto const projection = glm::perspective(glm::radians(45.0f), render_state.swapchain_extent.width / (float)render_state.swapchain_extent.height, 0.1f, 10.0f);
        auto const camera_u = render_state.ubo.camera * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        auto const camera_v = render_state.ubo.camera * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        render_state.ubo.camera = projection * view;
        render_state.ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1,0,0));

        auto start_time = std::chrono::high_resolution_clock::now();
        float rotation_offset = 0;
        float x_offset = 0;
        bool dragging = false;
        while (true /* not glfwWindowShouldClose(window) */) {
                glfwPollEvents();


                // render_state.ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                // render_state.ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                // render_state.ubo.projection = glm::perspective(glm::radians(45.0f), render_state.swapchain_extent.width / (float) render_state.swapchain_extent.height, 0.1f, 10.0f);
                // auto const model = glm::rotate(glm::mat4(1.0f), time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                // render_state.ubo.model = model;
                memcpy(render_state.ubo_mapped_memory, &render_state.ubo, sizeof(Ubo));

                auto mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                if(mouse_state == GLFW_PRESS){
                        if(not dragging){
                                glfwGetCursorPos(window, &mouse_start_x, &mouse_start_y);
                                dragging = true;
                        }
                        if(dragging){
                                double x_pos = 0.1, y_pos = 0.1;
                                glfwGetCursorPos(window, &x_pos, &y_pos);
                                x_offset = (mouse_start_x - x_pos)/(double)width;
                                render_state.ubo.model = glm::rotate(glm::mat4(1.0f), (rotation_offset + x_offset) * glm::radians(180.0f), glm::vec3(1,0,0));
                        }

                }else if(mouse_state == GLFW_RELEASE and dragging){
                        dragging = false;
                        rotation_offset += x_offset;
                        // start_time = std::chrono::high_resolution_clock::now();
                }else{
                        // auto current_time = std::chrono::high_resolution_clock::now();
                        // rotation_offset = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
                        // render_state.ubo.model = glm::rotate(glm::mat4(1.0f), rotation_offset * glm::radians(180.0f), glm::vec3(1,0,0));
                }



                draw_frame(&render_state);

                // std::this_thread::sleep_for(std::chrono::milliseconds(33));
                // while (std::getchar() not_eq '\n')
                //         ;
        }
}
