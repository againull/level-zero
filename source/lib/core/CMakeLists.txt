# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: MIT
set(TARGET_NAME ze_api)

target_sources(${TARGET_LOADER_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_cmdqueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_cmdlist.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_barrier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_cl_interop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_copy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_fence.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_memory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_module.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_residency.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_sampler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_core_lib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_lib.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ze_lib.cpp
)

file(GLOB LEVEL_ZERO_LIBRARY_CORE_INCLUDES
    "*.h"
)

install(FILES ${LEVEL_ZERO_LIBRARY_CORE_INCLUDES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/level_zero/lib/core
    COMPONENT level-zero-devel
)
