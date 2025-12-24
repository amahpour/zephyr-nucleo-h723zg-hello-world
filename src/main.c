#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static const struct device *uart = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

static void uart_cb(const struct device *dev, void *user_data) {
    uint8_t c;
    if (uart_irq_rx_ready(dev) && uart_fifo_read(dev, &c, 1) == 1) {
        printk("You pressed: %c\n", c);
    }
}

int main(void) {
    if (!gpio_is_ready_dt(&led)) {
        return -1;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    
    if (!device_is_ready(uart)) {
        return -1;
    }
    uart_irq_callback_set(uart, uart_cb);
    uart_irq_rx_enable(uart);
    
    while (1) {
        gpio_pin_toggle_dt(&led);
        k_msleep(500);
    }
    return 0;
}

