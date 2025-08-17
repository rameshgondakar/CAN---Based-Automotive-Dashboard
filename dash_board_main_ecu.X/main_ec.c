/*
 * File:   main_ec.c
 * Author: Ramesh
 *
 * Created on 16 May, 2025, 5:59 PM
 */


#include <xc.h>
#include"clcd.h"
#include"can_main_ec.h"
#define SPEED 0x10
#define TEMP 0x20
#define GEAR 0x30
#define RPM 0x40
#define INC 0x50
void main(void)
{
     init_clcd();
     init_can();
     TRISB7 = 0;
     RB7 = 0;
     TRISB6 = 0;
     RB6 = 0;
     TRISB0 = 0;
     RB0 = 0;
     TRISB1 = 0;
     RB1 = 0;
     int left=0,right=0,col=0;
    while(1)
    {
         clcd_print("SD TEM GR RP IN", LINE1(0));
         //clcd_print("RPM  INC", LINE1(9));
        can_receive();
        if(can_payload[2]==SPEED)
        {
            clcd_putch(can_payload[D0],LINE2(0));
            clcd_putch(can_payload[D1],LINE2(1));             
        }
        can_receive();
        if(can_payload[2]==TEMP)
        {
            clcd_putch(can_payload[D0],LINE2(3));
            clcd_putch(can_payload[D1],LINE2(4));
            clcd_putch(can_payload[D1],LINE2(5));             
        }
        can_receive();
        if(can_payload[2]==GEAR)
        {
            clcd_putch(can_payload[D0],LINE2(7));                      
        }
        can_receive();
        if(can_payload[2]==RPM)
        {
            clcd_putch(can_payload[D0],LINE2(9));
            clcd_putch(can_payload[D1],LINE2(10));
            clcd_putch(can_payload[D2],LINE2(11));
            clcd_putch(can_payload[D3],LINE2(12));
        }
        can_receive();
        if(can_payload[2]==INC)
        {
             if(can_payload[D0]== 0x0E)
                {
                 clcd_putch('L',LINE2(14));  
                
                 RB6=0;
                 RB7=0;
                 left=1;
                 right=0;
                 col=0;
                }
                else if(can_payload[D0]==0x0D)
                {
                    clcd_putch('R',LINE2(14));  
                    RB0=0;
                    RB1=0;
                    RB6=1;
                    RB7=1;
                    right=1;
                    left=0;
                    col=0;
                }
                else if(can_payload[D0]==0x0B)
                {
                    clcd_print("OF",LINE2(14));  
                      RB0=0;
                      RB1=0;
                      RB6=0;
                      RB7=0;  
                     
                      left=0;
                      right=0;
                }
               if(left==1)
               {
                 RB0=1;
                 RB1=1;
                 for(int delay=4000;delay--;);
                 RB0=0;
                 RB1=0;
               }
              if(right==1)
               {
                 RB6=1;
                 RB7=1;
                 for(int delay=4000;delay--;);
                 RB6=0;
                 RB7=0;
               }          
             
          }       
   
    }
    return;
}
