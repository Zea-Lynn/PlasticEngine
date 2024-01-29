#include "defines.h"
#include "includes.h"
#pragma once

auto create_d20() {
        struct {
                glm::vec3 vertcies[12];
                u32 indices[20 * 3];
        } ron;

        auto const t = (1.0 + sqrt(5)) / 2.0f;
        ron.vertcies[0] = {-1, t, 0};
        ron.vertcies[1] = {1, t, 0};
        ron.vertcies[2] = {-1, -t, 0};
        ron.vertcies[3] = {1, -t, 0};
        ron.vertcies[4] = {0, -1, t};
        ron.vertcies[5] = {0, 1, t};
        ron.vertcies[6] = {0, -1, -t};
        ron.vertcies[7] = {0, 1, -t};
        ron.vertcies[8] = {t, 0, -1};
        ron.vertcies[9] = {t, 0, 1};
        ron.vertcies[10] = {-t, 0, -1};
        ron.vertcies[11] = {-t, 0, 1};

        ron.indices[0] = 0;
        ron.indices[1] = 11;
        ron.indices[2] = 5;
        ron.indices[3] = 0;
        ron.indices[4] = 5;
        ron.indices[5] = 1;
        ron.indices[6] = 0;
        ron.indices[7] = 1;
        ron.indices[8] = 7;
        ron.indices[9] = 0;
        ron.indices[10] = 7;
        ron.indices[11] = 10;
        ron.indices[12] = 0;
        ron.indices[13] = 10;
        ron.indices[14] = 11;

        ron.indices[15] = 1;
        ron.indices[16] = 5;
        ron.indices[17] = 9;
        ron.indices[18] = 5;
        ron.indices[19] = 11;
        ron.indices[20] = 4;
        ron.indices[21] = 11;
        ron.indices[22] = 10;
        ron.indices[23] = 2;
        ron.indices[24] = 10;
        ron.indices[25] = 7;
        ron.indices[26] = 6;
        ron.indices[27] = 7;
        ron.indices[28] = 1;
        ron.indices[29] = 8;

        ron.indices[30] = 3;
        ron.indices[31] = 9;
        ron.indices[32] = 4;
        ron.indices[33] = 3;
        ron.indices[34] = 4;
        ron.indices[35] = 2;
        ron.indices[36] = 3;
        ron.indices[37] = 2;
        ron.indices[38] = 6;
        ron.indices[39] = 3;
        ron.indices[40] = 6;
        ron.indices[41] = 8;
        ron.indices[42] = 3;
        ron.indices[43] = 8;
        ron.indices[44] = 9;

        ron.indices[45] = 4;
        ron.indices[46] = 9;
        ron.indices[47] = 5;
        ron.indices[48] = 2;
        ron.indices[49] = 4;
        ron.indices[50] = 11;
        ron.indices[51] = 6;
        ron.indices[52] = 2;
        ron.indices[53] = 10;
        ron.indices[54] = 8;
        ron.indices[55] = 6;
        ron.indices[56] = 7;
        ron.indices[57] = 9;
        ron.indices[58] = 8;
        ron.indices[59] = 1;

        return ron;
}

auto midpoint(glm::vec3 a, glm::vec3 b) {
        glm::vec3 c;
        c.x = (a.x + b.x) / 2;
        c.y = (a.y + b.y) / 2;
        c.z = (a.z + b.z) / 2;
        return c;
}
//TODO: real subdivide instead of whatever i'm doing down below. also move this to a mesh_shader.
auto sqrt3_subdivide(auto mesh){
}

auto create_icosphere() {
        auto bla = create_d20();
        struct {
                std::vector<glm::vec3> points;
                std::vector<u32> indices;
        } sphere{.points = std::vector(bla.vertcies, bla.vertcies + 12), .indices = std::vector(bla.indices, bla.indices + 60)};
        // for (auto &&point : sphere.points) point = glm::normalize(point);

                std::vector<glm::vec3> midpoints;
                auto midpoints_by_point = std::vector<std::array<s32, 5>>(sphere.points.size(), {-1, -1, -1, -1, -1});
                auto counts_of_midpoints_by_point = std::vector<u32>(sphere.points.size(), 0);

                for (auto face = 0; face < sphere.indices.size(); face += 3) {
                        for (auto i = 0; i < 3; ++i) {
                                auto a_index = sphere.indices[i + face];
                                auto b_index = sphere.indices[((i + 1) % 3) + face];
                                // Check midpoint already exists
                                for (auto a_m = 0; a_m < counts_of_midpoints_by_point[a_index]; ++a_m) {
                                        for (auto b_m = 0; b_m < counts_of_midpoints_by_point[b_index]; ++b_m) {
                                                if (midpoints_by_point[a_index][a_m] == midpoints_by_point[b_index][b_m]) {
                                                        goto midpoint_exists;
                                                }
                                        }
                                }

                                midpoints_by_point[a_index][counts_of_midpoints_by_point[a_index]] = midpoints.size();
                                ++counts_of_midpoints_by_point[a_index];

                                midpoints_by_point[b_index][counts_of_midpoints_by_point[b_index]] = midpoints.size();
                                ++counts_of_midpoints_by_point[b_index];

                                midpoints.push_back(glm::normalize(midpoint(sphere.points[a_index], sphere.points[b_index])));
                        midpoint_exists:
                                continue;
                        }
                }
                auto find_midpoint_index = [&midpoints_by_point](u32 a, u32 b) {
                        for (auto a_m = 0; a_m < 5; ++a_m) {
                                for (auto b_m = 0; b_m < 5; ++b_m) {
                                        if (midpoints_by_point[a][a_m] == midpoints_by_point[b][b_m]) {
                                                return midpoints_by_point[a][a_m];
                                        }
                                }
                        }
                        puts("this is bad");
                        exit(420);
                };
                auto midpoints_to_new_points = std::vector<s32>(midpoints.size(), -1);
                auto points_to_new_points = std::vector<s32>(sphere.points.size(),  -1);
                auto points = std::vector<glm::vec3>(sphere.points.size() + midpoints.size());
                auto indices = std::vector<u32>(sphere.indices.size() * 4);

                for (auto face = 0; face < sphere.indices.size(); face += 3) {
                        u32 point_indices[3];
                        u32 midpoint_indices[3];
                        for (auto i = 0; i <  3; ++i) {
                                auto a_index = sphere.indices[i + face];
                                auto b_index = sphere.indices[((i + 1) % 3) + face];
                                auto a_b_index = find_midpoint_index(a_index, b_index);
                                if(points_to_new_points[a_index] < 0){
                                        points_to_new_points[a_index] = points.size();
                                        point_indices[i] = points.size();
                                        points.push_back(glm::normalize(sphere.points[a_index]));
                                }else{
                                        point_indices[i] = points_to_new_points[a_index];
                                }
                                auto midpoint_index = find_midpoint_index(a_index, b_index);
                                if(midpoints_to_new_points[midpoint_index] < 0){
                                        midpoints_to_new_points[midpoint_index] = points.size();
                                        midpoint_indices[i] = points.size();
                                        points.push_back(midpoints[midpoint_index]);
                                }else{
                                        midpoint_indices[i] = midpoints_to_new_points[midpoint_index];
                                }
                        }

                        indices.push_back(point_indices[0]);
                        indices.push_back(midpoint_indices[0]);
                        indices.push_back(midpoint_indices[2]);

                        indices.push_back(midpoint_indices[0]);
                        indices.push_back(midpoint_indices[1]);
                        indices.push_back(midpoint_indices[2]);

                        indices.push_back(point_indices[1]);
                        indices.push_back(midpoint_indices[1]);
                        indices.push_back(midpoint_indices[0]);

                        indices.push_back(point_indices[2]);
                        indices.push_back(midpoint_indices[2]);
                        indices.push_back(midpoint_indices[1]);
                }

                sphere.indices = indices;
                sphere.points = points;
        return sphere;
}

auto create_platform(){
        struct{
                glm::vec3 points[4]{{-10,0,-10}, {10,0,-10}, {-10,0,10}, {10,0,10}};
                u32 indices[6]{0,2,1,1,2,3};
        } platform;

        return platform;
}
