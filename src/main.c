/*
 * Copyright 2026 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <anjay/anjay.h>
#include <anjay/security.h>
#include <anjay/server.h>
#include <avsystem/commons/avs_time.h>

#include <modem/lte_lc.h>
#include <modem/nrf_modem_lib.h>
#include <zephyr/logging/log.h>

#include "device.h"

LOG_MODULE_REGISTER(anjay_zephyr_demo);

const anjay_dm_object_def_t **device_obj;

// Installs Security Object and adds and instance of it.
// An instance of Security Object provides information needed to connect to
// LwM2M server.
static int setup_security_object(anjay_t *anjay) {
    if (anjay_security_object_install(anjay)) {
        return -1;
    }

    const anjay_security_instance_t security_instance = {
        .ssid = 1,
        .server_uri = CONFIG_ANJAY_ZEPHYR_DEMO_SERVER_URI,
        .security_mode = ANJAY_SECURITY_PSK,
        .public_cert_or_psk_identity =
                (const uint8_t *) CONFIG_ANJAY_ZEPHYR_DEMO_ENDPOINT_NAME,
        .public_cert_or_psk_identity_size =
                strlen(CONFIG_ANJAY_ZEPHYR_DEMO_ENDPOINT_NAME),
        .private_cert_or_psk_key =
                (const uint8_t *) CONFIG_ANJAY_ZEPHYR_DEMO_PSK,
        .private_cert_or_psk_key_size = strlen(CONFIG_ANJAY_ZEPHYR_DEMO_PSK)
    };

    // Anjay will assign Instance ID automatically
    anjay_iid_t security_instance_id = ANJAY_ID_INVALID;
    if (anjay_security_object_add_instance(anjay, &security_instance,
                                           &security_instance_id)) {
        return -1;
    }

    return 0;
}

// Installs Server Object and adds and instance of it.
// An instance of Server Object provides the data related to a LwM2M Server.
static int setup_server_object(anjay_t *anjay) {
    if (anjay_server_object_install(anjay)) {
        return -1;
    }

    const anjay_server_instance_t server_instance = {
        .ssid = 1,
        .lifetime = 60,
        .default_min_period = -1,
        .default_max_period = -1,
        .disable_timeout = -1,
        .binding = "U"
    };

    // Anjay will assign Instance ID automatically
    anjay_iid_t server_instance_id = ANJAY_ID_INVALID;
    if (anjay_server_object_add_instance(anjay, &server_instance,
                                         &server_instance_id)) {
        return -1;
    }

    return 0;
}

int main(void) {
    LOG_INF("Starting Anjay-zephyr-demo...");
    int ret;

    ret = nrf_modem_lib_init();
    if (ret) {
        LOG_ERR("LTE init failed.");
        return -1;
    }
    LOG_INF("LTE initialized.");

    ret = lte_lc_connect();
    if (ret) {
        LOG_ERR("LTE link could not be established.");
        return -1;
    }
    LOG_INF("LTE link established.");

    const anjay_configuration_t CONFIG = {
        .endpoint_name = CONFIG_ANJAY_ZEPHYR_DEMO_ENDPOINT_NAME,
        .in_buffer_size = 4000,
        .out_buffer_size = 4000,
        .msg_cache_size = 4000,
    };

    anjay_t *anjay = anjay_new(&CONFIG);
    if (!anjay) {
        LOG_ERR("Could not create Anjay object");
        return -1;
    }

    int result = 0;
    // Setup necessary objects
    if (setup_security_object(anjay) || setup_server_object(anjay)) {
        result = -1;
    }

    device_obj = device_object_create();
    anjay_register_object(anjay, device_obj);

    result = anjay_event_loop_run_with_error_handling(
            anjay, avs_time_duration_from_scalar(1, AVS_TIME_S));

    device_object_release(&device_obj);
    anjay_delete(anjay);

    device_object_reboot_if_requested();

    return result;
}
