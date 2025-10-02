#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/console/console.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#define LED_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

typedef enum {
    STATE_INITIALIZATION,
    STATE_OPERATIONAL
} state_t;

static state_t current_state;

static void enter_initialization(void) {
    printk(">> Enter Initialization: initParameters\n");
    printk("[Initialization] Running init tasks (3s)...\n");
    k_sleep(K_SECONDS(3));
    printk("<< Exit Initialization: ready\n");
    current_state = STATE_OPERATIONAL;  
    printk(">> Auto transition to Operational\n");
      
}

static void enter_operational(void) {
    printk(">> Enter Operational: ledOn\n");
    gpio_pin_set_dt(&led, 1);  // LED ON
}

static void exit_operational(void) {
    printk("<< Exit Operational: stopNormalIO\n");
    gpio_pin_set_dt(&led, 0);  // LED OFF
}

/* --- Main loop --- */
void main(void)
{
    console_init();

    if (!device_is_ready(led.port)) {
        printk("Error: LED device not ready\n");
        return;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    printk("State Machine Demo (Zephyr)\n");
    printk("Press 'r' to reset\n");

    current_state = STATE_INITIALIZATION;
    enter_initialization();
    while (1) {
        char *line = console_getline();

        if (strcmp(line, "r") == 0) {
            printk("Command: reset\n");
            if (current_state == STATE_OPERATIONAL) {
                exit_operational();
                current_state = STATE_INITIALIZATION;
                enter_initialization();  
            }
        }
    }
}
