/*
 * File:   main.c
 * Author: Ramesh
 *
 * Created on 6 May, 2025, 6:08 PM
 */


#include <xc.h>
#include"clcd.h"
#include"can.h"
#define EDGE 1
#define LEVEL 0
#define SW1 1
#define SW2 2
#define SW3 3

void delay(unsigned short factor) {
    unsigned short i, j;

    for (i = 0; i < factor; i++) {
        for (j = 500; j--;);
    }
}

void init_adc(void) {
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

unsigned short read_adc(unsigned char channel) {
    unsigned short reg_val;


    ADCON0 = (ADCON0 & 0xC3) | (channel << 2);

    GO = 1;
    while (GO);
    reg_val = (ADRESH << 8) | ADRESL;

    return reg_val;
}

char read_dk_switch(unsigned char det) {
    static int flag = 1;
    if (det == LEVEL) {
        if ((PORTC & 0x0F) != 0x0F) {
            return (PORTC & 0x0F);
        }
    } else if (det == EDGE) {
        if (((PORTC & 0x0F) != 0x0F) && flag == 1) {
            flag = 0;
            // if()
            return (PORTC & 0x0F);
        } else if ((PORTC & 0x0F) == 0x0F) {
            flag = 1;
        }
    }
    return 0x0F;
}

void display(char *data) {
    //    PORTA=PORTA&0xF0;
    for (int i = 0; i < 4; i++) {
        PORTD = data[i];
        PORTA = (PORTA & 0xF0) | (1 << i);
        for (unsigned long int delay = 1000; delay--;);
    }
}

void main(void) {


    char left = 0, right = 0, neu = 0;
    int rpm;
    char sw;
    TRISD = 0x00;
    TRISA = TRISA & 0xf0;
    PORTD = 0x00;
    PORTA = 0x00;
    TRISC = TRISC | 0x0F;
    char digit[] = {0xE7, 0x21, 0xCB, 0x6B, 0x2D, 0x6E, 0xEE, 0x23, 0xEF, 0x6F};
    char ssd[4];
    char data[9] = {0};

    init_adc();
    init_can();
    // TRISB = TRISB & 0x30;
    //PORTB =PORTB & 0x30;
    TRISB6 = 0;
    RB6 = 0;
    TRISB0 = 0;
    RB0 = 0;
    TRISB1 = 0;
    RB0 = 0;

    TRISB7 = 0;
    RB7 = 0;
    while (1) {

        rpm = read_adc(0x04)*5.865;
        data[0] = rpm / 1000 + 48;
        data[1] = ((rpm / 100) % 10) + 48;
        data[2] = ((rpm / 10) % 10) + 48;
        data[3] = (rpm % 10) + 48;

        can_transmit(0x40, 4, data);
        __delay_ms(100);

        //        can_receive();
        //        if (can_payload[2] == 0x10) 
        //        {
        //            ssd[0] = digit[can_payload[D0] - 48];
        //            ssd[1] = digit[can_payload[D1] - 48];
        //            ssd[2] = digit[can_payload[D2] - 48];
        //            ssd[3] = digit[can_payload[D3] - 48];
        //            display(ssd);
        //        }

        sw = read_dk_switch(EDGE);
        data[0] = sw;
        can_transmit(0x50, 1, data);
        __delay_ms(100);
        //  can_receive();
        if (sw == 0x0E) {

            //  data[0]=1+48;
            right = 0;
            left = 1;
        } else if (sw == 0x0D) {
            //data[0]=2+48;
            left = 0;
            right = 1;
            // can_transmit(0x20, 1,right);
        } else if (sw == 0x0B) {
            // data[0]=3+48;
            left = 0;
            right = 0;
            //  neu=3;
            //   can_transmit(0x20, 1, neu);

        }


        if (left == 1) {
            RB6 = 0;
            RB7 = 0;

            RB0 = 1;
            RB1 = 1;
            //RB6=0;
            // clcd_putch(0x7F,LINE2(8));
        } else if (right == 1) {
            RB0 = 0;
            RB1 = 0;

            RB6 = 1;
            RB7 = 1;
            //RB1=0;
            //  clcd_putch(0x7E,LINE2(8));
        } else {
            RB6 = 0;
            RB7 = 0;
            RB0 = 0;
            RB1 = 0;
            // PORTB=PORTB & 0x30;
            //            RB0=0;
            //            RB1=0;
            //            RB7=0;
            //            RB6=0;
            // clcd_print("OFF",LINE2(8));
        }

    }
    return;
}
