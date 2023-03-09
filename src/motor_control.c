#include "i2c_contorl.h"
#include "motor_control.h"

static unsigned int flag_type = OUT_INT;       
static unsigned int mot_type = MOT54;          
static unsigned int mot_run_mode = HALF;       
unsigned int ms32006_fclk = 24000000;            

unsigned int mot1_move_step=0x00;     
unsigned int mot2_move_step=0x00;     

unsigned int save_pwm_frequ=0x00;     


unsigned char ms32006_read_temp_state(unsigned int fd,  unsigned int mot)
{
	unsigned int  readstate=0x00;
	if(mot==MOT1)
	{ 
		 readstate = i2c_read_byte(fd, 0x0B); //read BH data
		 if(readstate&0x20){  return(TRUE);  } else {  return(FALSE); }					
	}
	else if(mot==MOT2)
	{
		 readstate = i2c_read_byte(fd, 0x0D); //read DH data
		 if(readstate&0x20){  return(TRUE);  } else {  return(FALSE); }		
	}
}


unsigned char ms32006_read_run_state(unsigned int fd,  unsigned int mot)
{
	unsigned int  readstate=0x00;
	if(mot==MOT1)
	{ 
		 readstate = i2c_read_byte(fd, 0x0B); //read BH data
		 if(readstate&0x10){  return(TRUE);  } else {  return(FALSE); }					
	}
	else if(mot==MOT2)
	{
		 readstate = i2c_read_byte(fd, 0x0D); //read DH data
		 if(readstate&0x10){  return(TRUE);  } else {  return(FALSE); }		
	}
}


unsigned char ms32006_read_buffer_state(unsigned int fd,  unsigned int mot)
{
	unsigned int  readstate=0x00;
	if(mot==MOT1)
	{ 
        readstate = i2c_read_byte(fd, 0x0B); //read BH data
        if(readstate&0x40){  return(TRUE);  } else {  return(FALSE); }					
	}
	else if(mot==MOT2)
	{
        readstate = i2c_read_byte(fd, 0x0D); //read DH data
        if(readstate&0x40){  return(TRUE);  } else {  return(FALSE); }		
	}
}


void ms32006_clear_step(unsigned int fd,  unsigned int mot)
{
	unsigned int rdata=0x00,readstate=0x00;
	if(mot==MOT1)
	{
		 rdata=i2c_read_byte(fd, 0x04);    	//read 0x04 data
		 rdata^=(1<<6);                     //BIT6 inverts and changes the direction once.
		 rdata = rdata & 0XF0;              //Clear the lower 4 digits to zero
		 i2c_write_byte(fd, 0x03, 0X00);    //Number of movement steps
		 i2c_write_byte(fd, 0x04, rdata);   //write 0x04 data
		
		 readstate = i2c_read_byte(fd, 0x09); //read 0x09 data
		 readstate |=0x80; //motA enable
		 i2c_write_byte(fd, 0x09, readstate);  //write 0x09 data
	}
	else if(mot==MOT2)
	{
		 rdata=i2c_read_byte(fd, 0x08);    //read 0x08 data
		 rdata^=(1<<6);                     
		 rdata = rdata & 0XF0;               
		 i2c_write_byte(fd, 0x07, 0X00);   //Number of movement steps
		 i2c_write_byte(fd, 0x08, rdata);  //write 0x08 data
		 
		 readstate = i2c_read_byte(fd, 0x09); 
		 readstate |=0x40; ///motB enable
		 i2c_write_byte(fd, 0x09, readstate); 
	}
}


unsigned int ms32006_readstep(unsigned int fd,  unsigned int mot)
{
	unsigned int rdb=0x00; unsigned int rdc=0x00;  
    
	if(mot==MOT1)
	{
		rdb = i2c_read_byte(fd, 0x0b); //read 0x0b data
		rdc = i2c_read_byte(fd, 0x0c); //read 0x0c data
		
		//putstring("\r\n R_MOT1=0X"); uart1_print_hex(rdb); uart1_print_hex(rdc); putstring("\r\n");
	}
	else if(mot==MOT2)
	{
		rdb = i2c_read_byte(fd, 0x0d); 
		rdc = i2c_read_byte(fd, 0x0e);
		
		//putstring("\r\n R_MOT2=0X"); uart1_print_hex(rdb); uart1_print_hex(rdc); putstring("\r\n");
	}
	return((rdb*0x100+rdc)&0xfff );     //clears up to 4 bits
 
}


void ms32006_pwm_out(unsigned int fd, unsigned int pwm_frequ, unsigned int pwm_duty)
{
	if(pwm_frequ>3)
	{
	   i2c_write_byte(fd, 0x09, 0x03);
	   save_pwm_frequ=0x03;
	}
	else
	{
		i2c_write_byte(fd, 0x09, pwm_frequ);
		save_pwm_frequ=pwm_frequ;
	}
	
	if(pwm_duty>128)
	{
	   i2c_write_byte(fd, 0x0A, 128);
	}
	else
	{
	   i2c_write_byte(fd, 0x0A, pwm_duty);
	}
}


void ms32006_ircut(unsigned int fd, unsigned int ircut_state)
{
	unsigned int setdata=0x00;
  
	if(ircut_state == IRCUT_CW)          
	{
		setdata =0X04;
		i2c_write_byte(fd, 0x09, setdata); //BIT2=1 BIT3=0 
	}
	else if(ircut_state == IRCUT_CCW)   
	{
		setdata =0X08;
		i2c_write_byte(fd, 0x09, setdata); //BIT2=0 BIT3=1	
	}
	else if(ircut_state == IRCUT_CLOSE)   
	{
		setdata =0X00;
		i2c_write_byte(fd, 0x09, setdata); //BIT2=0 BIT3=0	高阻态
	}
	
	
// delay 200-500mS 
// Be sure to set it to high resistance, otherwise the IRCT will heat up
}


void ms32006_close(unsigned int fd, unsigned int set_mot)
{
   unsigned int set_data=0x00,readstate=0x00;
	
	 readstate=0x00;
	 readstate = i2c_read_byte(fd, 0x09); 
 
    if(set_mot==MOT1)
    {
        i2c_write_byte(fd, 0x04, set_data); 
        
        readstate |=0x80; 
        i2c_write_byte(fd, 0x09, readstate);  
    }
    
	if(set_mot==MOT2)
    {
        i2c_write_byte(fd, 0x08, set_data); 
        
        readstate |=0x40; 
        i2c_write_byte(fd, 0x09, readstate);  
    }
}


void ms32006_pause(unsigned int fd, unsigned int set_mot)
{
   unsigned int readstate=0x00;
   readstate = i2c_read_byte(fd, 0x00); 
	 if(set_mot==MOT1)
	 {
        readstate |=0x04; 
	    i2c_write_byte(fd, 0x00, readstate); 
	 }
	 else if(set_mot==MOT2)
	 {
        readstate |=0x02; 
	    i2c_write_byte(fd, 0x00, readstate); 
	 }
}


void ms32006_move(unsigned int fd, unsigned int mot1_dir , unsigned int mot1_pps, unsigned int mot1_step , unsigned int mot2_dir , unsigned int mot2_pps, unsigned int mot2_step)
{
	 unsigned int mot_set_speed_data=0x00;     
	 unsigned int mot_set_speed_l=0x00;        
	 unsigned int mot_set_speed_h=0x00;        

	 unsigned int mot_set_step_data =0x00;     
	 unsigned int mot_set_step_l=0x00;         
	 unsigned int mot_set_step_h=0x00;         
   
	 unsigned int read_data=0x00;
	
    unsigned int readstate=0x00;
    readstate = i2c_read_byte(fd, 0x00);
   
    if(mot1_step>0) 
	  {
        mot_set_speed_data=ms32006_fclk/mot1_pps/128;             
        if(mot_set_speed_data<32){mot_set_speed_data=32;}         
        if(mot_set_speed_data>16383){mot_set_speed_data=16383;}   

        mot_set_speed_l = mot_set_speed_data&0x00ff; 
        mot_set_speed_h = mot_set_speed_data/0x100;

        if(mot_run_mode==HALF)
        {
            mot_set_speed_h |=0x80;
        }
        else
        {
            mot_set_speed_h &=0x7f;
        }

        mot_set_step_data=mot1_step;
        mot_set_step_l = mot_set_step_data&0x00ff;
        mot_set_step_h = mot_set_step_data/0x100;
        mot_set_step_h |=0x80; 
        

        if(mot1_dir == CW)
        {
            mot_set_step_h &=0xBF;
        }
        else if(mot1_dir == CCW)
        {
            mot_set_step_h |=0x40;
        }
        else
        {
            mot_set_step_h &=0xBF;
        }
        readstate &=0xfb;
        i2c_write_byte(fd, 0x00, readstate);   
        i2c_write_byte(fd, 0x01, mot_set_speed_l);
        i2c_write_byte(fd, 0x02, mot_set_speed_h);
        i2c_write_byte(fd, 0x03, mot_set_step_l);
        i2c_write_byte(fd, 0x04, mot_set_step_h);
        
        
	}
	
	if(mot2_step>0)
	{
        mot_set_speed_data=ms32006_fclk/mot2_pps/128;              
        if(mot_set_speed_data<32){mot_set_speed_data=32;}         
        if(mot_set_speed_data>16383){mot_set_speed_data=16383;}

        mot_set_speed_l = mot_set_speed_data&0x00ff;
        mot_set_speed_h = mot_set_speed_data/0x100;

        if(mot_run_mode==HALF)
        {
            mot_set_speed_h |=0x80;
        }
        else
        {
            mot_set_speed_h &=0x7f;
        }

        mot_set_step_data=mot2_step;
        mot_set_step_l = mot_set_step_data&0x00ff;
        mot_set_step_h = mot_set_step_data/0x100;
        mot_set_step_h |=0x80;
    
        if(mot2_dir == CW)
        {
            mot_set_step_h &=0xBF;
        }
        else if(mot2_dir == CCW)
        {
            mot_set_step_h |=0x40;
        }
        else
        {
            mot_set_step_h &=0xBF;
        }
        readstate &=0xfD;
        i2c_write_byte(fd, 0x00, readstate);
        i2c_write_byte(fd, 0x05, mot_set_speed_l);
        i2c_write_byte(fd, 0x06, mot_set_speed_h);
        i2c_write_byte(fd, 0x07, mot_set_step_l);
        i2c_write_byte(fd, 0x08, mot_set_step_h);
        
        
	}
 
  
	readstate=0x00;
	readstate = i2c_read_byte(fd, 0x09);
	if((mot1_step>0)&&(mot2_step>0))
	{
		readstate |=0xC0;
	}
	else if(mot1_step>0)
	{
		readstate |=0x80;
	}
	else if(mot2_step>0)
	{
		readstate |=0x40;
	}
	i2c_write_byte(fd, 0x09, readstate);
	 
}	


void ms32006_init(int fd)
{
    unsigned int mot_set_speed_data=0x00;
    unsigned int mot_set_speed_l=0x00;
    unsigned int mot_set_speed_h=0x00;
    
    unsigned int mot_set_step_data =0;
    unsigned int mot_set_step_l=0x00;
    unsigned int mot_set_step_h=0x00;

    
    i2c_write_byte(fd, 0x00, 0x00);
    usleep(100000);

    if(mot_type==MOT42)
	 {
		 if(flag_type==OUT_INT)
		 {
            i2c_write_byte(fd, 0x00, 0x01);
		 }
		 else
		 {
            i2c_write_byte(fd, 0x00, 0x41);
		 }
	 }
	 else
	 {
		 if(flag_type==OUT_INT)
		 {
            i2c_write_byte(fd, 0x00, 0x81);
		 }
		 else
		 {
            i2c_write_byte(fd, 0x00, 0xC1);
		 }
	 }
    mot_set_speed_data = ms32006_fclk/200/128; 
    if(mot_set_speed_data<32){mot_set_speed_data=32;}
    if(mot_set_speed_data>16383){mot_set_speed_data=16383;}

    mot_set_speed_l = mot_set_speed_data&0x00ff;
    mot_set_speed_h = mot_set_speed_data/0x100;

    if(mot_run_mode==HALF)
    {
        mot_set_speed_h |=0x80;
    }
    else
    {
        mot_set_speed_h &=0x7f;
    }

    mot_set_step_l = mot_set_step_data&0x00ff;
    mot_set_step_h = mot_set_step_h/0x100;
    mot_set_step_h |=0x80;

    
    i2c_write_byte(fd, 0x01, mot_set_speed_l); 
    i2c_write_byte(fd, 0x02, mot_set_speed_h); 
    i2c_write_byte(fd, 0x03, mot_set_step_l);
    i2c_write_byte(fd, 0x04, mot_set_step_h);
    
    i2c_write_byte(fd, 0x05, mot_set_speed_l);
    i2c_write_byte(fd, 0x06, mot_set_speed_h);
    i2c_write_byte(fd, 0x07, mot_set_step_l);
    i2c_write_byte(fd, 0x08, mot_set_step_h);
    
    i2c_write_byte(fd, 0x09, 0x2f);
    i2c_write_byte(fd, 0x0a, 0x08);
}
