#include "includes.h"

#include "mesh.cpp"
#include "renderer.cpp"
#include "gui.cpp"

#include "gltf_stuff.cpp"
#include "./smol-gltf/smol_gltf.h"

int main() noexcept {
        #ifdef DEBUG
        puts("debug build");
        #endif

        if (not glfwInit()) {
                puts("Could not initialize GLFW");
                exit(123);
        }

        // TODO: maybe figure out a better way.
        smol_GLTF gltf; 
        smol_allocator smol_allocator = {
                .user_data = nullptr, 
                .allocate = [](auto user_data, auto size){return malloc(size);}, 
                .free = [](auto user_data, auto ptr){free(ptr);}
        };

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
        auto monitor = glfwGetPrimaryMonitor();
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
        }
        {
                renderer.player_mesh = renderer.load_static_mesh32(gltf_points, pos_count, gltf_indices.data(), gltf_indices.size(), gltf_texcoords);
        }

        glfwSetWindowCloseCallback(window, [](GLFWwindow *window) { exit(0); });

        auto ui = UI::initalize();
        renderer.allocate_ui(420, 69420);
        s8 something_button_id = -1;
        s8 blerg_button = -1;
        s8 exit_button = -1;
        s8 frame_field = -1;
        s8 increase_far_plane_id = -1;
        s8 decrease_far_plane_id = -1;
        s8 fullscrean = -1;
        s8 exit_fullscrean = -1;
        bool is_fullscrean = false;
        f32 far_plane = 10;
        u64 frame_count = 0;
        bool ui_should_close = false;
        bool ui_should_open = false;
        bool ui_open = false;
        UI::Action ui_action = UI::Action::none;
        int key_to_wait_for_release = 0;

        double mouse_start_x = -1, mouse_start_y = -1;


        auto const view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
        auto projection = glm::perspective(glm::radians(45.0f), renderer.swapchain_info.imageExtent.width / (float)renderer.swapchain_info.imageExtent.height, 0.1f, 1000.0f);
        renderer.player_ubo.camera = projection * view;
        renderer.player_ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1,0,0));
        renderer.terrain_ubo.camera = projection * view;
        renderer.terrain_ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1,0,0)) * glm::translate(glm::mat4(1.0f), {-.5f, -.5f, -.5f});
        memcpy(renderer.player_ubo_mapped_memory, &renderer.player_ubo, sizeof(Ubo));
        memcpy(renderer.terrain_ubo_mapped_memory, &renderer.terrain_ubo, sizeof(Ubo));
        // for(auto i = 0; i < renderer.swapchain_image_count; ++i) renderer.record_command_buffers(i);

        auto start_time = std::chrono::high_resolution_clock::now();
        float rotation_offset = 0;
        float x_offset = 0;
        bool dragging = false;

        // auto font_atlas = UI::generate_font_atlas_from_file("/usr/share/fonts/noto/NotoSans-Light.ttf", 50);
        auto font_atlas = UI::genearte_default_font_atlas(50);
        if(not font_atlas) puts("no font atlas");
        renderer.load_ui_texture(font_atlas->extent.width, font_atlas->extent.height, font_atlas->bitmap);

        while (true /* not glfwWindowShouldClose(window) */) {
                glfwPollEvents();

                auto esc_pressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
                auto esc_released = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE;
                if(esc_pressed and not ui_open and not ui_should_open and not ui_should_close){
                        ui_should_open = true;
                }
                if(esc_pressed and ui_open and not ui_should_open and not ui_should_close){
                        ui_should_close = true;
                }
                if(esc_released and ui_open and ui_should_open) ui_should_open = false;
                if(esc_released and not ui_open and ui_should_close) ui_should_close = false;
                if(ui_should_open) ui_open = true;
                if(ui_should_close) ui_open = false;
                if(ui_open){
                        ui.ui_is_focused = true;
                        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
                                ui.element_is_active = true;
                        }else{
                                ui.element_is_active = false;
                        }
                        ui.field_s64(&frame_field, "Blerg", (++frame_count));
                        if(not is_fullscrean){
                                if(ui.button(&fullscrean, "full screan no boarder")){
                                        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
                                        is_fullscrean = true;

                                }
                        }else{
                                if(ui.button(&exit_fullscrean, "exit fullscrean")){
                                        glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
                                        is_fullscrean = false;

                                }
                        }
                        if(ui.button(&exit_button, "Exit")){
                                exit(420);
                        }

                        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS ){
                                if(ui_action == UI::Action::none and key_to_wait_for_release not_eq GLFW_KEY_J){
                                        ui_action = UI::Action::down;
                                }else if(ui_action == UI::Action::down){
                                        ui_action = UI::Action::none;
                                        key_to_wait_for_release = GLFW_KEY_J;
                                }
                        }else if(glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE){
                                if(key_to_wait_for_release == GLFW_KEY_J) key_to_wait_for_release = 0;
                        }
                        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS ){
                                if(ui_action == UI::Action::none and key_to_wait_for_release not_eq GLFW_KEY_K){
                                        ui_action = UI::Action::up;
                                }else if(ui_action == UI::Action::up){
                                        ui_action = UI::Action::none;
                                        key_to_wait_for_release = GLFW_KEY_K;
                                }
                        }else if(glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE){
                                if(key_to_wait_for_release == GLFW_KEY_K) key_to_wait_for_release = 0;
                        }

                        auto gui_data = ui.finish_and_draw(renderer.swapchain_info.imageExtent.width, renderer.swapchain_info.imageExtent.height, &font_atlas.value(), ui_action);
                        renderer.load_ui_data(gui_data.indices.size(), gui_data.indices.data(), gui_data.positions.size(), gui_data.positions.data(), gui_data.texuvs.data(), gui_data.colors.data());
                }else{
                        ui.ui_is_focused = false;
                        renderer.load_ui_data(0, nullptr, 0, nullptr, nullptr, nullptr);
                }

                auto mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                if(mouse_state == GLFW_PRESS){
                        puts("pressed");
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
        }
}
