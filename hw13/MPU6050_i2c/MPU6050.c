#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "MPU6050.h"

//Device address
#define ADDR _u(0b1101000)

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define WHO_AM_I     0x75
// Hardware registers

void chip_init();
int main();
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp);


/*
void set(char v) {
    uint8_t buf[2];

        // send register number followed by its corresponding value
    buf[0] = REG_OLAT;
    buf[1] = v << 7;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}
*/

/*
int read() {
    uint8_t buf[1];
    uint8_t reg = REG_GPIO;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf, 1, false);

    if (buf[0]&0b1 == 0b1){
        return 1;
    }
    else{
        return  0;
    }
}
*/
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = ACCEL_XOUT_H;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = GYRO_XOUT_H;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = TEMP_OUT_H;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}




/*
int main() {
    stdio_init_all();

    // I2C is "open drain", pull ups to keep signal high when no data is being sent
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    //Pico heartbeat
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);


    // initializie the chip
    chip_init(); // gp7 is output, gp0 is input

    // check to make sure chip is communicating
    uint8_t buf_2[1];
    uint8_t reg = WHO_AM_I;

    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buf_2, 1, false);

    if ((buf_2[0] != 0x68) && (buf_2[0] != 0x98)){
        printf("chip not found");
        //gpio_put(LED_PIN, 1);
    }

    int16_t acceleration[3], gyro[3], temp;

    while (1) {
        mpu6050_read_raw(acceleration, gyro, &temp);

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        sleep_ms(10);
    }
}
*/


void chip_init() {
    uint8_t buf[2];

    // turn the chip on
    buf[0] = PWR_MGMT_1;
    buf[1] = 0x00;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // enable the accelerometer
    buf[0] = ACCEL_CONFIG;
    buf[1] = 0x00;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    // enable gyroscope
    buf[0] = GYRO_CONFIG;
    buf[1] = 0b00011000;
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}