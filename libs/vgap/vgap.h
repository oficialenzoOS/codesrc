#include <stdint.h>
#include <stdio.h>

static int arch = 0;

static void load_arch() {

    if (arch)
        return;

    FILE *f =
    fopen("/sys/optinals/arch.prop", "r");

    if (!f)
        return;

    fscanf(
        f,
        "arch.sys=%d",
        &arch
    );

    fclose(f);
}

static uint32_t hex_to_rgb(
const char *hex) {

    uint32_t rgb = 0;

    if (*hex == '#')
        hex++;

    for (int i = 0; i < 6; i++) {

        char c = hex[i];

        rgb <<= 4;

        if (c >= '0' && c <= '9')
            rgb |= c - '0';

        else if (
            c >= 'A' &&
            c <= 'F')
            rgb |= c - 'A' + 10;

        else if (
            c >= 'a' &&
            c <= 'f')
            rgb |= c - 'a' + 10;
    }

    return rgb;
}

static void draw_vga(
const char *text) {

    volatile uint16_t *vga =
    (uint16_t*)0xB8000;

    int i = 0;

    while (text[i]) {

        vga[i] =
        (0x0F << 8)
        | text[i];

        i++;
    }
}

static void draw_fb(
uint32_t color,
const char *text) {

    volatile uint32_t *fb =
    (uint32_t*)0x10000000;

    int i = 0;

    while (text[i]) {

        fb[i] = color;

        i++;
    }
}

static void draw_uart(
const char *text) {

    volatile uint8_t *uart =
    (uint8_t*)0x10000000;

    int i = 0;

    while (text[i]) {

        uart[i] = text[i];

        i++;
    }
}

void vgap(
const char *hex,
const char *text) {

    load_arch();

    uint32_t color =
    hex_to_rgb(hex);

    switch (arch) {

        case 1:
        case 2:
            draw_vga(text);
            break;

        case 3:
        case 4:
            draw_fb(
            color,
            text);
            break;

        case 5:
            draw_uart(text);
            break;
    }
}
