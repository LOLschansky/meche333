add_executable(mcp4912_spi
        mcp4912_spi.c
        )

# pull in common dependencies and additional spi hardware support
target_link_libraries(mcp4912_spi pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(mcp4912_spi)

# add url via pico_set_program_url
#example_auto_set_url(mcp4912_spi)
