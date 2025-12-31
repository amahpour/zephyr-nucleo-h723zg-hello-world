/*
 * GPIO tracing hooks for native_sim
 * Prints debug messages when GPIO operations occur
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/gpio/gpio_emul.h>

void sys_trace_gpio_port_toggle_bits_exit_user(const struct device *port, int ret)
{
    /* Read actual LED state after toggle */
    int state = gpio_emul_output_get(port, 0);
    printk("[DEBUG] (GPIO) LED state: %s\n", state ? "ON" : "OFF");
}

void sys_trace_gpio_pin_configure_exit_user(const struct device *port,
                                            gpio_pin_t pin, int ret)
{
    printk("[DEBUG] (GPIO) pin %d configured (ret=%d)\n", pin, ret);
}

