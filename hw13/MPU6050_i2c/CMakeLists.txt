add_executable(MPU6050_i2c main.c MPU6050.c usb_descriptors.c
        )
# Make sure TinyUSB can find tusb_config.h
target_include_directories(MPU6050_i2c PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies and additional i2c hardware support
target_link_libraries(MPU6050_i2c pico_stdlib hardware_i2c pico_unique_id pico_binary_info tinyusb_device tinyusb_board)
pico_enable_stdio_usb(MPU6050_i2c 1)
pico_enable_stdio_uart(MPU6050_i2c 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(MPU6050_i2c)

# add url via pico_set_program_url
