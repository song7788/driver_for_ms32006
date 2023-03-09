//example 

#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include "motor_control.h"
#include "i2c_contorl.h"
#include "type_info.h"


//speed 228pps   20°/s  x = 360/(4095/motor_speed)

int main(int argc, const char **argv)
{
    int  i2c_fd = i2c_init(I2C_DEV, I2C_ADDRESS);
    ms32006_init(i2c_fd);

    while(1)
    {
        ms32006_move(i2c_fd, CW, 228, MOTOR_MAX_STEP, CW, 228, MOTOR_MAX_STEP);
        while(ms32006_read_run_state(i2c_fd, MOT1) | ms32006_read_run_state(i2c_fd, MOT2))
        {
            usleep(1000);
        }

        ms32006_move(i2c_fd, CCW, 228, MOTOR_MAX_STEP, CCW, 228, MOTOR_MAX_STEP);   
        while(ms32006_read_run_state(i2c_fd, MOT1) | ms32006_read_run_state(i2c_fd, MOT2))
        {
            usleep(1000);
        }
        ms32006_close(i2c_fd, MOT1);
        ms32006_close(i2c_fd, MOT2);
        ms32006_clear_step(i2c_fd, MOT1);
        ms32006_clear_step(i2c_fd, MOT2);
    }
}
