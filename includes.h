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

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"
