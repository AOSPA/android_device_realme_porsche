/*
 * Copyright (C) 2023 Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));
    char* build_desc;
    char* build_fingerprint;
    char* product_model;
    char* product_name;

    switch (prjname) {
        case 136882:
            product_name = "RMX3312";
            product_model = "RMX3312";
            build_desc = "qssi-user 13 TP1A.220905.001 1695796070671 release-keys";
            build_fingerprint = "realme/RMX3312/RE58B2L1:13/TP1A.220905.001/S.1383667_1_2:user/release-keys";
            break;
        default:
            product_name = "RMX3311EEA";
            product_model = "RMX3311";
            build_desc = "qssi-user 13 TP1A.220905.001 1682070307948 release-keys";
            build_fingerprint = "realme/RMX3311EEA/RE58B2L1:13/TP1A.220905.001/S.fe5caa-da01-3c05a:user/release-keys";
            break;
    }

    OverrideProperty("ro.product.model", product_model);
    OverrideProperty("ro.product.name", product_name);
    OverrideProperty("ro.build.fingerprint", build_fingerprint);
    OverrideProperty("ro.build.description", build_desc);
    OverrideProperty("ro.boot.hardware.sku", "porsche");
}
