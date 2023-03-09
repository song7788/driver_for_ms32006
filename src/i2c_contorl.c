
#include "i2c_contorl.h"
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

int i2c_write_byte(int fd, unsigned int command, unsigned int value)
{
    return i2c_smbus_write_byte_data(fd, command, value);
}

int i2c_read_byte(int fd, unsigned int command)
{
    return i2c_smbus_read_byte_data(fd, command);
}

int i2c_write_date(int fd, unsigned short dev_addr, unsigned char reg_addr, unsigned char* data, unsigned int len)
{
    int ret = -1;
    unsigned char buff[20] = { 0 };
    
    buff[0] = reg_addr;
    memcpy(&buff[1], data, len);


    struct i2c_msg msg = {
        .addr = dev_addr,
        .flags = 0,
        .len = len + 1,
        .buf = buff,
    };
    
    struct i2c_rdwr_ioctl_data rdwr_msg = {
        .msgs = &msg,
        .nmsgs = 1,
    };
    
    ret = ioctl(fd, I2C_RDWR, &rdwr_msg);
    
    return ret;
}

int i2c_read_date(int fd, unsigned short dev_addr,  unsigned char reg_addr, unsigned char* data, unsigned int len)
{
    int ret = -1;

 
    struct i2c_msg msg[2] = {
        {
            .addr = dev_addr,   
            .flags = 0,         
            .len = 1,           
            .buf = &reg_addr,   
        },
        {
            .addr = dev_addr,   
            .flags = I2C_M_RD,  
            .len = len,         
            .buf = data,        
        },
    };

    struct i2c_rdwr_ioctl_data rdwr_msg = {
        .msgs = msg,
        .nmsgs = 2,
    };

    ret = ioctl(fd, I2C_RDWR, &rdwr_msg);

    return ret;
 }

int i2c_init(char *file_path, unsigned int address)
{
    int fd = 0;
    fd = open(file_path, O_RDWR);
    if (ioctl(fd,I2C_SLAVE_FORCE, address) < 0) 
    {            
        printf("set slave address failed \n");
    }
    return fd;
}

int i2c_close(int fd)
{
    return close(fd);
}