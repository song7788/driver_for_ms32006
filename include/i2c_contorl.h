#ifndef __I2C_CONTORL_H__
#define __I2C_CONTORL_H__

#include "type_info.h"


/**
 * @brief   I2C write byte
 * 
 * @param[in]   fd          
 * @param[in]   command     Register address
 * @param[in]   value       
 * @return  int 
 * 
 */
int i2c_write_byte(int fd, unsigned int command, unsigned int value);

/**
 * @brief   I2C read byte
 * 
 * @param[in]   fd          
 * @param[in]   command     Register address
 * @return  int 
 * 
 */
int i2c_read_byte(int fd, unsigned int command);

/**
 * @brief   I2C Write data interface
 * 
 * @param[in]   fd          
 * @param[in]   dev_addr    I2C Device address
 * @param[in]   reg_addr    Register address
 * @param[in]   data        
 * @param[in]   len         data len
 * @return  int
 * 
 */
int i2c_write_date(int fd, unsigned short dev_addr, unsigned char reg_addr, unsigned char* data, unsigned int len);


/**
 * @brief   I2C Read data interface
 * 
 * @param[in]   fd          
 * @param[in]   dev_addr    I2C Device address
 * @param[in]   reg_addr    Register address
 * @param[in]   data        
 * @param[in]   len         data len
 * @return  int 
 * 
 */
int i2c_read_date(int fd, unsigned short dev_addr,  unsigned char reg_addr, unsigned char* data, unsigned int len);

/**
 * @brief   i2c init
 * 
 * @param[in]   file_path   
 * @param[in]   address     
 * @return  int 
 * 
 */
int i2c_init(char *file_path, unsigned int address);

/**
 * @brief   i2c close
 * 
 * @param[in]   fd
 * @return  int 
 * 
 */
int i2c_close(int fd);
#endif