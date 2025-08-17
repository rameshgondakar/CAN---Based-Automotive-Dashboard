/*
 * File:   ec.c
 * Author: Ramesh
 *
 * Created on 5 May, 2025, 3:48 PM
 */


#include <xc.h>
#include <xc.h>
#include "clcd.h"
#include"can.h"

void delay(unsigned short factor)
{
	unsigned short i, j;

	for (i = 0; i < factor; i++)
	{
		for (j = 500; j--; );
	}
}

void init_adc(void)
{
	ADFM = 1;
    
	ACQT2 = 0;
	ACQT1 = 1;
	ACQT0 = 0;

	
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	
	GODONE = 0;

	VCFG1 = 0;
	VCFG0 = 0;

	
	ADRESH = 0;
	ADRESL = 0;

	
	ADON = 1;
}

unsigned short read_adc(unsigned char channel)
{
	unsigned short reg_val;

	
	ADCON0 = (ADCON0 & 0xC3) | (channel << 2);

	GO = 1;
	while (GO);
	reg_val = (ADRESH << 8) | ADRESL; 

	return reg_val;
}
//static int speed;
static int temp;
void main(void)
{
   // int speed=0;
   
    char key=0,val;
   // char val;
    int i=0,flag=1,temp=0,speed=0,col=0;
  //  int flag=1;
    //int temp=0;
    float t=0;
    
    init_matrix_keypad();
    init_adc();
    init_clcd();
  
   // 
     //int col=0;
     
     init_can();
     char data[8];
     TRISB7 = 0;
     RB7 = 0;
    while(1){
       
           speed=read_adc(0x04)/10.33;
           data[0]=(speed/10) + 48;
           data[1]= (speed%10) + 48;
//                    clcd_putch('0'+(speed/10),LINE2(8));
//                   clcd_putch('0'+(speed%10),LINE2(9));
             
             can_transmit(0x10,2,data);     
             __delay_ms(100);
//          /   delay(800);
//             can_receive();
//             if (can_payload[2]==0x10)
//	         {              
//                clcd_putch(can_payload[5], LINE2(0));
//                clcd_putch(can_payload[6], LINE2(1));                 
//             }
             
          
              t=((read_adc(0x06)*5)/(float)1023);
              temp=(float)t*100;
           
//        
////            clcd_putch('0'+(temp/100),LINE2(6));
////            clcd_putch('0'+(temp/10)%10,LINE2(7));
////            clcd_putch('0'+(temp%10),LINE2(8));
////            
////            clcd_putch(0xDF,LINE2(9));
////            clcd_putch('C',LINE2(10));
////
             data[0]=(temp/100)+48;
             data[1]=((temp/10)%10)+48;
             data[2]=(temp%10)+48;
             
             can_transmit(0x20,3,data);
             __delay_ms(100);
//             delay(800);
//              can_receive();
//             if (can_payload[2]==0x20)
//	         {              
//                clcd_putch(can_payload[D0], LINE2(5));
//                clcd_putch(can_payload[D1], LINE2(6));        
//                clcd_putch(can_payload[D2], LINE2(7));
//             }
////             
//        
      key =  read_switches(STATE_CHANGE);
      if(key==1)
      {
           if(col==1) {i=0;col=0;} 
          if(i<6) i++;
        
          else i=6;
         
       }
      if(key==2)
      {
           if(col==1){ i=7;}
           else if(i>=1) i--;
               
          else i=0;
               
      }
      if(key==3)
      {
          i=7;
          col=1;
      }
     
     // if(flag==1)
    //  {
//          if(i==0)
//          {
////             can_transmit(0x30,,data);    
////             can_recieve();
////             if(can_payload[2]==0x30)
//              clcd_putch('N', LINE2(13));
//          }
         // else
          { 
              data[0]=i+48;
              can_transmit(0x30,1,data);  
              __delay_ms(100);
//              delay(800);
//              can_receive();
//             if(can_payload[2]==0x30)
//               clcd_putch(can_payload[D0], LINE2(13));
          }
    //  }
          

    }
    return;
}
