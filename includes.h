#pragma once

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

#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/utility/vk_struct_helper.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using vec4 = glm::vec4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;

using Pos = vec3;
using Texuv = vec2;
using Color = vec4;
using Normal = vec3;

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#include "./stb/stb_rect_pack.h"
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include "./stb/stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"
