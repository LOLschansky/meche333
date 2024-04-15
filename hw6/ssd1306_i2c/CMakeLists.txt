add_executable(ssd1306_i2c
        ssd1306.c
        main.c
        )

# pull in common dependencies and additional spi hardware support
target_link_libraries(ssd1306_i2c pico_stdlib hardware_i2c hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(ssd1306_i2c)

# add url via pico_set_program_url
#example_auto_set_url(ssd1306_i2c)
