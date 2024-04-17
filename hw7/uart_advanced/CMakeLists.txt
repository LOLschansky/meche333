add_executable(uart_advanced
        uart_advanced.c
        ssd1306.c
        )

# pull in common dependencies and additional uart hardware support
target_link_libraries(uart_advanced pico_stdlib hardware_uart hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_advanced)

# add url via pico_set_program_url
#xample_auto_set_url(uart_advanced)
