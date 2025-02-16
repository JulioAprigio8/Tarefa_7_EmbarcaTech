#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lwipopts.h"
#include "inc/ssd1306_i2c.h"
#include "inc/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define VRX 27
#define VRY 26
#define JOYSTICK_BTN 22
#define BUZZER_PIN 21
#define CANCEL_BTN 20

typedef enum {
    MENU_MAIN,
    MENU_USE_SENSORS,
    MENU_SIMULATE_SENSORS
} MenuState;

typedef struct {
    const char* text;
    void (*action)(void);
} MenuItem;

volatile bool button_pressed = false;
MenuState current_state = MENU_MAIN;
int current_selection = 0;
uint8_t display_buffer[ssd1306_buffer_length];

MenuItem main_menu[] = {
    {"USAR SENSORES", NULL},
    {"SIMULAR SENSORES", NULL},
};

void draw_menu() {
    memset(display_buffer, 0, ssd1306_buffer_length);
    ssd1306_draw_string(display_buffer, 0, 0, "Menu Principal");
    for(int i = 0; i < 2; i++) {
        ssd1306_draw_string(display_buffer, 10, (i+1)*16, (char*)main_menu[i].text);
        if(i == current_selection) {
            ssd1306_draw_line(display_buffer, 0, (i+1)*16, 5, (i+1)*16+5, true);
        }
    }
    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width-1,
        .start_page = 0,
        .end_page = ssd1306_n_pages-1
    };
    calculate_render_area_buffer_length(&area);
    render_on_display(display_buffer, &area);
}

void use_sensors() {
    // Implementação da leitura real dos sensores
    printf("Usando sensores reais\n");
}

void simulate_sensors() {
    printf("Simulando sensores\n");
    float temperature = 25.0;
    while(true) {
        temperature += 0.5;
        printf("Temperatura simulada: %.1f°C\n", temperature);
        if(temperature > 35.0) {
            pwm_set_gpio_level(BUZZER_PIN, 1000);
            printf("ALERTA: Temperatura crítica!\n");
            while(gpio_get(CANCEL_BTN)) {
                sleep_ms(10);
            }
            pwm_set_gpio_level(BUZZER_PIN, 0);
            break;
        }
        sleep_ms(1000);
    }
}

int main() {
    stdio_init_all();
    
    i2c_init(I2C_PORT, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init();
    
    adc_init();
    adc_gpio_init(VRY);
    adc_select_input(0);
    
    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN);
    
    gpio_init(CANCEL_BTN);
    gpio_set_dir(CANCEL_BTN, GPIO_IN);
    gpio_pull_up(CANCEL_BTN);
    
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_wrap(slice_num, 255);
    pwm_set_enabled(slice_num, true);
    
    while(true) {
        if(current_state == MENU_MAIN) {
            draw_menu();
            uint16_t y_raw = adc_read();
            if(y_raw < 1000) {
                current_selection = (current_selection + 1) % 2;
            } else if(y_raw > 3000) {
                current_selection = (current_selection - 1 + 2) % 2;
            }
            
            if(!gpio_get(JOYSTICK_BTN)) {
                sleep_ms(20);
                if(!gpio_get(JOYSTICK_BTN)) {
                    if(current_selection == 0) {
                        use_sensors();
                    } else {
                        simulate_sensors();
                    }
                }
            }
        }
        sleep_ms(100);
    }
    
    return 0;
}
