/*
 * GPIO Tracing Hooks for native_sim
 * 
 * This file provides debug output for GPIO operations when running on native_sim.
 * It uses Zephyr's tracing subsystem to hook into GPIO calls without modifying main.c.
 * 
 * How it works:
 * - Zephyr's tracing system defines weak `_user` callback functions
 * - When CONFIG_TRACING_GPIO=y and CONFIG_TRACING_USER=y are set, these callbacks
 *   are invoked on GPIO operations
 * - We override the weak functions to print debug messages
 * - gpio_emul_output_get() reads the actual GPIO state from the emulator
 * 
 * Enabled via boards/native_sim.conf:
 *   CONFIG_TRACING=y
 *   CONFIG_TRACING_GPIO=y
 *   CONFIG_TRACING_USER=y
 * 
 * This file is conditionally compiled only when tracing is enabled (see CMakeLists.txt)
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/gpio/gpio_emul.h>

void sys_trace_gpio_port_toggle_bits_exit_user(const struct device *port, int ret)
{
    /* Read actual LED state after toggle using GPIO emulator API */
    int state = gpio_emul_output_get(port, 0);
    printk("[DEBUG] (GPIO) LED state: %s\n", state ? "ON" : "OFF");
}

void sys_trace_gpio_pin_configure_exit_user(const struct device *port,
                                            gpio_pin_t pin, int ret)
{
    printk("[DEBUG] (GPIO) pin %d configured (ret=%d)\n", pin, ret);
}

