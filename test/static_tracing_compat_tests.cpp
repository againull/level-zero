/*
 * Copyright (C) 2026 Intel Corporation
 * SPDX-License-Identifier: MIT
 */

#include "gtest/gtest.h"
#include "source/lib/ze_tracing_compat.h"

namespace {

ze_dditable_t zeTable = {};
zet_dditable_t zetTable = {};
zes_dditable_t zesTable = {};
ze_result_t initResult = ZE_RESULT_SUCCESS;
unsigned calls = 0;

ze_result_t ZE_APICALL legacyInit(std::atomic<ze_dditable_t *> &ze,
                                std::atomic<zet_dditable_t *> &zet,
                                std::atomic<zes_dditable_t *> &zes) {
    EXPECT_EQ(zet.load(), nullptr);
    EXPECT_EQ(zes.load(), nullptr);
    ze.store(&zeTable);
    zet.store(&zetTable);
    zes.store(&zesTable);
    ++calls;
    return initResult;
}

ze_result_t ZE_APICALL modernInit(std::atomic<ze_dditable_t *> &ze) {
    ze.store(&zeTable);
    ++calls;
    return initResult;
}

class StaticTracingCompatibility : public ::testing::Test {
protected:
    void SetUp() override {
        calls = 0;
        initResult = ZE_RESULT_SUCCESS;
    }
};

TEST_F(StaticTracingCompatibility, LegacyLoaderReceivesAllThreeOutputs) {
    for (auto version : {zel_version_t{1, 14, 0}, zel_version_t{1, 17, 6},
                         zel_version_t{1, 17, 22}}) {
        EXPECT_TRUE(ze_lib::hasLegacyTracingInitABI(version));
        std::atomic<ze_dditable_t *> output{nullptr};
        EXPECT_EQ(ZE_RESULT_SUCCESS,
                  ze_lib::initializeTracingTable(
                      reinterpret_cast<void *>(legacyInit), version, output));
        EXPECT_EQ(output.load(), &zeTable);
    }
    EXPECT_EQ(calls, 3u);
}

TEST_F(StaticTracingCompatibility, ModernLoaderReceivesOneOutput) {
    for (auto version : {zel_version_t{1, 17, 23}, zel_version_t{1, 17, 25},
                         zel_version_t{1, 18, 0},
                         zel_version_t{1, 35, 0}}) {
        EXPECT_FALSE(ze_lib::hasLegacyTracingInitABI(version));
        std::atomic<ze_dditable_t *> output{nullptr};
        EXPECT_EQ(ZE_RESULT_SUCCESS,
                  ze_lib::initializeTracingTable(
                      reinterpret_cast<void *>(modernInit), version, output));
        EXPECT_EQ(output.load(), &zeTable);
    }
    EXPECT_EQ(calls, 4u);
}

TEST_F(StaticTracingCompatibility, InitializationErrorsArePreserved) {
    initResult = ZE_RESULT_ERROR_DEVICE_LOST;
    std::atomic<ze_dditable_t *> output{nullptr};
    EXPECT_EQ(ZE_RESULT_ERROR_DEVICE_LOST,
              ze_lib::initializeTracingTable(
                  reinterpret_cast<void *>(legacyInit), {1, 17, 6}, output));
    EXPECT_EQ(ZE_RESULT_ERROR_DEVICE_LOST,
              ze_lib::initializeTracingTable(
                  reinterpret_cast<void *>(modernInit), {1, 17, 25}, output));
}

TEST_F(StaticTracingCompatibility, MissingEntryPointIsUnsupported) {
    std::atomic<ze_dditable_t *> output{nullptr};
    EXPECT_EQ(ZE_RESULT_ERROR_UNSUPPORTED_FEATURE,
              ze_lib::initializeTracingTable(nullptr, {1, 17, 6}, output));
    EXPECT_EQ(output.load(), nullptr);
    EXPECT_EQ(calls, 0u);
}

} // namespace
