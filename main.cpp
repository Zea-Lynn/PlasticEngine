
#include "includes.h"
#include "mesh.cpp"
#include "renderer.cpp"
#include "gui.cpp"

#include "gltf_stuff.cpp"
#include "./smol-gltf/smol_gltf.h"

int main() noexcept {
        if (not glfwInit()) {
                puts("Could not initialize GLFW");
                exit(123);
        }

        // TODO: maybe figure out a better way.
        smol_GLTF gltf; 
        smol_allocator smol_allocator = {.user_data = nullptr, .allocate = [](auto user_data, auto size){return malloc(size);}, .free = [](auto user_data, auto ptr){free(ptr);}};
        if(not smol_parse_GLTF(monkey_glb_len, monkey_glb, &gltf, smol_allocator)){
                puts("error parsing gltf, oopsie");
                exit(420);
        }
        size_t pos_byte_length = 0;
        size_t pos_offset = 0;
        size_t pos_count = 0; 
        size_t pos_index_byte_length = 0;
        size_t pos_index_offset = 0; 
        size_t pos_index_count = 0;
        size_t uv_offset = 0;
        for(auto attribute_index = 0; attribute_index < gltf.meshes[0].primitives[0].attribute_count; ++attribute_index){
                auto attribute = gltf.meshes->primitives->attributes[attribute_index];
                if(attribute.name == smol_POSITION){
                        auto position_view = gltf.buffer_views[gltf.accessors[attribute.accessor].buffer_view];
                        pos_count = gltf.accessors[attribute.accessor].count;
                        pos_byte_length = position_view.byte_length;
                        pos_offset = position_view.byte_offset;
                        auto index_view = gltf.buffer_views[gltf.accessors[gltf.meshes->primitives->indices].buffer_view];
                        pos_index_byte_length = index_view.byte_length;
                        pos_index_offset = index_view.byte_offset;
                        pos_index_count = gltf.accessors[gltf.meshes->primitives->indices].count;

                }
                if(attribute.name == smol_TEXCOORD){
                        uv_offset = gltf.buffer_views[gltf.accessors[attribute.accessor].buffer_view].byte_offset;
                }
        }
        auto gltf_points = reinterpret_cast<glm::vec3 const *>(gltf.data + pos_offset);
        auto gltf_texcoords = reinterpret_cast<glm::vec2 const *>(gltf.data + uv_offset);
        auto gltf_indices = std::vector<u32>(pos_index_count);
        for(auto pos_index_index = 0; pos_index_index < pos_index_count; ++pos_index_index){
                gltf_indices[pos_index_index] = static_cast<u32>(reinterpret_cast<u16 const *>(gltf.data + pos_index_offset)[pos_index_index]);
        }

        const auto width = 512 * 2, height = 512 * 2;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        const auto window = glfwCreateWindow(width, height, "plastic", nullptr, nullptr);
        if (not window) {
                puts("Couldn't get GLFW window");
                exit(123);
        }

        if (glfwVulkanSupported() == GLFW_API_UNAVAILABLE) {
                puts("Vulkan is not supported.\n");
                exit(123);
        }

        auto allocator = nullptr;

        Render_State renderer;
        Render_State::initalize(&renderer, window);

        // auto imgui = IMGUI::initalize(); 

        {
                auto [points, indices] = create_platform();
                auto uvs = std::vector<glm::vec2>{{-1,-1}, {1,-1}, {-1,1}, {1,1}};
                renderer.terrain_mesh = renderer.load_static_mesh32(points, 4, indices, 6, uvs.data());
                int x, y, comp;
                auto texture = stbi_load("./sometexture.png",&x, &y, nullptr, STBI_rgb_alpha);
                if(texture == nullptr){
                        puts("unable to load texture");
                        exit(420);
                }
                renderer.load_terrain_texture(x, y, texture);
                renderer.load_ui_texture(x, y, texture);
        }
        {
                renderer.player_mesh = renderer.load_static_mesh32(gltf_points, pos_count, gltf_indices.data(), gltf_indices.size(), gltf_texcoords);
        }

        glfwSetWindowUserPointer(window, &renderer);
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                auto render_state = (Render_State *)glfwGetWindowUserPointer(window);
                if (key == GLFW_KEY_ESCAPE) {
                        exit(0);
                }
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow *window) { exit(0); });

        auto ui = UI::initalize();
        renderer.allocate_ui(420, 69420);

        s8 something_button_id = -1;

        // for (auto i = 0; i < renderer.swapchain_image_count; ++i) renderer.record_command_buffers(i);

        double mouse_start_x = -1, mouse_start_y = -1;

        auto const view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        auto const projection = glm::perspective(glm::radians(45.0f), renderer.swapchain_extent.width / (float)renderer.swapchain_extent.height, 0.1f, 10.0f);
        renderer.player_ubo.camera = projection * view;
        renderer.player_ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1,0,0));
        renderer.terrain_ubo.camera = projection * view;
        renderer.terrain_ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1,0,0)) * glm::translate(glm::mat4(1.0f), {-.5f, -.5f, -.5f});

        auto start_time = std::chrono::high_resolution_clock::now();
        float rotation_offset = 0;
        float x_offset = 0;
        bool dragging = false;


        while (true /* not glfwWindowShouldClose(window) */) {
                glfwPollEvents();
                ui.begin(1, 1, 0,0, key::none, UI::Font_Atlas{{400,400}, {20,20}, {0,0}});
                if(ui.button(&something_button_id, "Something")){
                        //TODO: do button stuff.
                }
                auto gui_data = ui.finish_and_draw();
                renderer.load_ui_data(gui_data.indices.size(), gui_data.indices.data(), gui_data.positions.size(), gui_data.positions.data(), gui_data.texuvs.data(), gui_data.colors.data());
                // render_state.ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                // render_state.ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                // render_state.ubo.projection = glm::perspective(glm::radians(45.0f), render_state.swapchain_extent.width / (float) render_state.swapchain_extent.height, 0.1f, 10.0f);
                // auto const model = glm::rotate(glm::mat4(1.0f), time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                // render_state.ubo.model = model;
                memcpy(renderer.player_ubo_mapped_memory, &renderer.player_ubo, sizeof(Ubo));
                memcpy(renderer.terrain_ubo_mapped_memory, &renderer.terrain_ubo, sizeof(Ubo));

                auto mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                if(mouse_state == GLFW_PRESS){
                        if(not dragging){
                                glfwGetCursorPos(window, &mouse_start_x, &mouse_start_y);
                                dragging = true;
                        }else{
                                double x_pos = 0.1, y_pos = 0.1;
                                glfwGetCursorPos(window, &x_pos, &y_pos);
                                x_offset = (mouse_start_x - x_pos)/(double)width;
                                renderer.player_ubo.model = glm::rotate(glm::mat4(1.0f), (rotation_offset + x_offset) * glm::radians(180.0f), glm::vec3(1,0,0));
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

                renderer.draw_frame();

                // std::this_thread::sleep_for(std::chrono::milliseconds(33));
                // while (std::getchar() not_eq '\n')
                //         ;
        }
}
