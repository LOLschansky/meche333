add_executable(pwm
        pwm.c
        )

# pull in common dependencies
target_link_libraries(pwm pico_stdlib hardware_pwm)
#pico_enable_stdio_usb(print 1)
#pico_enable_stdio_uart(print 0)


# create map/bin/hex file etc.
pico_add_extra_outputs(pwm)


# add url via pico_set_program_url
