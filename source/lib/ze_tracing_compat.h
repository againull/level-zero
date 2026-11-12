/*
 * Copyright (C) 2026 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "loader/ze_loader.h"
#include "ze_ddi.h"
#include "zes_ddi.h"
#include "zet_ddi.h"

#include <atomic>

namespace ze_lib {

// The export kept its name when its ABI changed in loader 1.17.23
// (commit 35fa646, first included in the tagged release 1.17.25).
// Older implementations write through all three references, so checking only
// whether the symbol exists does not make a one-argument call safe.
inline bool hasLegacyTracingInitABI(const zel_version_t &version) {
    return version.major == 1 &&
           (version.minor < 17 || (version.minor == 17 && version.patch < 23));
}

inline ze_result_t initializeTracingTable(
    void *function, const zel_version_t &version,
    std::atomic<ze_dditable_t *> &zeTable) {
    if (function == nullptr)
        return ZE_RESULT_ERROR_UNSUPPORTED_FEATURE;

    if (hasLegacyTracingInitABI(version)) {
        using LegacyInit = ze_result_t (ZE_APICALL *)(
            std::atomic<ze_dditable_t *> &, std::atomic<zet_dditable_t *> &,
            std::atomic<zes_dditable_t *> &);
        // Legacy initialization only stores table pointers into these outputs;
        // it does not retain the references. The current shim traces ZE APIs
        // only, but must supply valid storage for both obsolete outputs.
        std::atomic<zet_dditable_t *> zetTable{nullptr};
        std::atomic<zes_dditable_t *> zesTable{nullptr};
        return reinterpret_cast<LegacyInit>(function)(zeTable, zetTable, zesTable);
    }

    using Init = ze_result_t (ZE_APICALL *)(std::atomic<ze_dditable_t *> &);
    return reinterpret_cast<Init>(function)(zeTable);
}

} // namespace ze_lib
