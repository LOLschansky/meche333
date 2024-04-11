add_executable(mcp23008_i2c
        mcp23008_i2c.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(mcp23008_i2c pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(mcp23008_i2c)

# add url via pico_set_program_url
