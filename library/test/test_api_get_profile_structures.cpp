/*
 * Copyright (c) 2021-2022 Valve Corporation
 * Copyright (c) 2021-2022 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "test.hpp"
#ifndef VULKAN_PROFILES_HEADER_ONLY
#include <vulkan/vulkan_profiles.hpp>
#else
#include <vulkan/vulkan_profiles.h>
#endif

TEST(api_get_profile_structures, get_properties2) {
    VkPhysicalDeviceProperties2 profileProperties2{};
    profileProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    profileProperties2.pNext = nullptr;

    const VpProfileProperties Profile = {VP_LUNARG_DESKTOP_PORTABILITY_2021_NAME, 1};

    vpGetProfileStructures(&Profile, &profileProperties2);

    EXPECT_EQ(16384, profileProperties2.properties.limits.maxImageDimension1D);
    EXPECT_EQ(16384, profileProperties2.properties.limits.maxImageDimension2D);
    EXPECT_EQ(2048, profileProperties2.properties.limits.maxImageDimension3D);
    EXPECT_EQ(16384, profileProperties2.properties.limits.maxImageDimensionCube);
    EXPECT_EQ(2048, profileProperties2.properties.limits.maxImageArrayLayers);
    EXPECT_EQ(8, profileProperties2.properties.limits.maxColorAttachments);
}

TEST(api_get_profile_structures, get_properties_single) {
    VkPhysicalDeviceDescriptorIndexingProperties properties{};
    properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES;
    properties.pNext = nullptr;

    const VpProfileProperties Profile = {VP_LUNARG_DESKTOP_PORTABILITY_2021_NAME, 1};

    vpGetProfileStructures(&Profile, &properties);

    EXPECT_EQ(1048576, properties.maxUpdateAfterBindDescriptorsInAllPools);
    EXPECT_EQ(16, properties.maxPerStageDescriptorUpdateAfterBindSamplers);
}

TEST(api_get_profile_structures, get_properties_chain) {
    VkPhysicalDeviceDescriptorIndexingProperties properties0{};
    properties0.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_PROPERTIES;
    properties0.pNext = nullptr;

    VkPhysicalDeviceMultiviewPropertiesKHR properties1{};
    properties1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR;
    properties1.pNext = &properties0;

    const VpProfileProperties Profile = {VP_LUNARG_DESKTOP_PORTABILITY_2021_NAME, 1};

    vpGetProfileStructures(&Profile, &properties1);

    EXPECT_EQ(1048576, properties0.maxUpdateAfterBindDescriptorsInAllPools);
    EXPECT_EQ(16, properties0.maxPerStageDescriptorUpdateAfterBindSamplers);
    EXPECT_EQ(134217727, properties1.maxMultiviewInstanceIndex);
    EXPECT_EQ(6, properties1.maxMultiviewViewCount);
}

TEST(api_get_profile_structures, get_features) {
    VkPhysicalDeviceVulkan12Features deviceVulkan12Features = {};
    deviceVulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    deviceVulkan12Features.pNext = nullptr;

    const VpProfileProperties Profile = {VP_KHR_ROADMAP_2022_NAME, 1};

    vpGetProfileStructures(&Profile, &deviceVulkan12Features);

    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.samplerMirrorClampToEdge);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.drawIndirectCount);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.storageBuffer8BitAccess);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.uniformAndStorageBuffer8BitAccess);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.storagePushConstant8);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.shaderBufferInt64Atomics);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.shaderSharedInt64Atomics);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.shaderFloat16);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.shaderInt8);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.descriptorIndexing);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.shaderUniformTexelBufferArrayDynamicIndexing);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.uniformBufferStandardLayout);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.shaderSubgroupExtendedTypes);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.separateDepthStencilLayouts);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.hostQueryReset);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.bufferDeviceAddress);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.vulkanMemoryModel);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.vulkanMemoryModelDeviceScope);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.vulkanMemoryModelAvailabilityVisibilityChains);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.subgroupBroadcastDynamicId);
    EXPECT_EQ(VK_TRUE, deviceVulkan12Features.imagelessFramebuffer);
    EXPECT_EQ(VK_FALSE, deviceVulkan12Features.shaderOutputLayer);
}

TEST(api_get_profile_structures, properties_full) {
    const VpProfileProperties profile = {VP_KHR_ROADMAP_2022_NAME, 1};

    uint32_t propertyCount = 0;
    VkResult result0 = vpGetProfileStructureProperties(&profile, &propertyCount, nullptr);
    EXPECT_EQ(VK_SUCCESS, result0);
    EXPECT_EQ(8, propertyCount);

    propertyCount = 9;

    std::vector<VpStructureProperties> properties(propertyCount);
    VkResult result1 = vpGetProfileStructureProperties(&profile, &propertyCount, &properties[0]);
    EXPECT_EQ(VK_SUCCESS, result1);
    EXPECT_EQ(8, propertyCount);

    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, properties[0].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, properties[1].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, properties[2].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, properties[3].type);

    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, properties[4].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES, properties[5].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES, properties[6].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES, properties[7].type);
}

TEST(api_get_profile_structures, properties_partial) {
    const VpProfileProperties profile = {VP_KHR_ROADMAP_2022_NAME, 1};

    uint32_t propertyCount = 0;
    VkResult result0 = vpGetProfileStructureProperties(&profile, &propertyCount, nullptr);
    EXPECT_EQ(VK_SUCCESS, result0);
    EXPECT_EQ(8, propertyCount);

    propertyCount = 5;

    std::vector<VpStructureProperties> structureTypes(propertyCount);
    VkResult result1 = vpGetProfileStructureProperties(&profile, &propertyCount, &structureTypes[0]);
    EXPECT_EQ(VK_INCOMPLETE, result1);
    EXPECT_EQ(5, propertyCount);

    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, structureTypes[0].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, structureTypes[1].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, structureTypes[2].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES, structureTypes[3].type);
    EXPECT_EQ(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2, structureTypes[4].type);
}