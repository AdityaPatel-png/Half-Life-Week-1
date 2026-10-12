// Note to self: The pico is not registering on device as of writing the code, so make sure it does before running hardware. (Maybe switch it to arduino so its easier if thats possible somehow)

#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>

#define I2C_PORT i2c1
const u_int32_t PIN_SDA = 26;
const u_int32_t PIN_SCL = 27;
const u_int32_t PIN_DR = 31;
const u_int32_t PIN_RST = 29;
const u_int32_t PIN_SD_CS = 32;
const u_int32_t PIN_SD_SCK = 24;
const u_int32_t PIN_SD_MOSI = 25;
const u_int32_t PIN_SD_MISO = 34;

const u_int32_t NUM_KEYS = 18;
const u_int32_t KEY_PINS[NUM_KEYS] = {
    10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7, 
    8, 9, 16, 17
};

void init_gpio_input(uintptr_t gpio_pin) {
    gpio_init(gpio_pin);
    gpio_pin_dir(gpio_pin, GPIO_IN);
    gpio_pin_pull_up(gpio_pin);
};

void init_touchpad_i2c() {
    i2c_init(TOUCHPAD_I2C_PORT, 400*1000 );
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);
    gpio_init(PIN_DR);
    gpio_set_dir(PIN_DR, GPIO_IN);
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    gpio_put(PIN_RESET, 0);
    sleep_ms(20);
    gpio_put(PIN_RESET, 1);
}

int main() {
    studio_init_all();
    sleep_ms(2000);

    for (u_int32_t i = 0; i < NUM_KEYS; i++) {
        init_gpio_input(KEY_PINS[i]);
    }

    init_touchpad_i2c();
    
    while (true)
    {
        for (uintptr_t i = 0; i < NUM_KEYS; i++) {
            if (!gpio_get(KEY_PINS[i])) 
            {
                printf("SWITCH ON KEY %d\n ACTIVE /n", KEY_PINS[i]);
            }
        }

        if (gpio_get(PIN_DR)) {
            uintptr_t buffer[6];

            int bytes_read = i2c_read_blocking(TOUCHPAD_I2C_PORT, 0x2A, buffer, 6, false);

            if (bytes_read>0) {
                u_int16_t x = (buffer[2] & 0x0F) << 8| buffer[1];
                u_int16_t y = (buffer[4] & 0x0F) << 8| buffer[3];
            }
        }

        sleep_ms(8);
    }
    
    return 0;
}