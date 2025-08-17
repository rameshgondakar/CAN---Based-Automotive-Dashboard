/*
 * File:   can_main_ec_pro.c
 * Author: Ramesh
 *
 * Created on 17 May, 2025, 6:08 PM
 */


#include <xc.h>

#include "can_main_ec.h"



/* Global Variables */


typedef enum _CanOpMode {
    e_can_op_mode_bits = 0xE0, /* Use this to access opmode bits */
    e_can_op_mode_normal = 0x00,
    e_can_op_mode_sleep = 0x20,
    e_can_op_mode_loop = 0x40,
    e_can_op_mode_listen = 0x60,
    e_can_op_mode_config = 0x80
} CanOpMode;

/* Configure the CAN Module */
void init_can(void) {
    /* CAN_TX = RB2, CAN_RX = RB3 */
    TRISB2 = 0; /* CAN TX */
    TRISB3 = 1; /* CAN RX */

    /* Enter CAN module into config mode */
    /* clear previous mode */
    CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_config); /* set new mode */

    /* Wait untill desired mode is set */
    while (CANSTAT != 0x80);

    /* Enter CAN module into Mode 0 */
    ECANCON = 0x00; // not requried by default it is set to 0x00 only 

    /* Initialize CAN Timing 8MHz */ // why we are setting this means we have to set 125kbps bits per sec so we set this so dont confuse why everything
    BRGCON1 = 0xE1; /* 1110 0001, SJW=4, TQ, BRP 4 */
    BRGCON2 = 0x1B; /* 0001 1011, SEG2PHTS 1 sampled once PS1=4TQ PropagationT 4TQ */
    BRGCON3 = 0x03; /* 0000 0011, PS2, 4TQ */

    /*
     * Enable Filters
     * Filter 0
     */
    RXFCON0 = 0x01;

    /*
     * Initialize Receive Filters
     * Filter 0 = 0xFFC
     */
    RXF0EIDH = 0x00;
    RXF0EIDL = 0x00;

    RXF0SIDH = 0x70;
    RXF0SIDL = 0x00;
    
    
    //incoming id & mask == mask & filter
    
    RXM0SIDH = 0x8F;
    RXM0SIDL = 0xFF;
    
    // above is now no need beacause in ECU 1 we dont need filter

    /* Enter CAN module into Loop back mode */
    CAN_SET_OPERATION_MODE_NO_WAIT(e_can_op_mode_normal);

    /* Set Receive Mode for buffers */
    RXB0CON = 0x00;
}

/* Check the buffers, if it have message */

unsigned char can_receive() {
    unsigned char rx_msg_flag = 0;

   //RB7 = 1;
    if (RXB0FUL) /* CheckRXB0 */ {
      //  RB7 = 1;
        can_payload[EIDH] = RXB0EIDH;
        can_payload[EIDL] = RXB0EIDL;
        can_payload[SIDH] = RXB0SIDH;
        can_payload[SIDL] = RXB0SIDL;
        can_payload[DLC] = RXB0DLC; // it is data length
        can_payload[D0] = RXB0D0;
        //clcd_putch(RXB0D0,LINE2(3));
        can_payload[D1] = RXB0D1;
        //clcd_putch(RXB0D1,LINE2(4));
        can_payload[D2] = RXB0D2;
        can_payload[D3] = RXB0D3;
        can_payload[D4] = RXB0D4;
        can_payload[D5] = RXB0D5;
        can_payload[D6] = RXB0D6;
        can_payload[D7] = RXB0D7;

        RXB0FUL = 0; //also a flag
        RXB0IF = 0; // just flag only

        return 1;
    } else {
        return 0;
    }
}

///* Transmit Sample Mesaage */
//int can_transmit(char msg,int dl, unsigned char data[]) {
////    RB0 = 1;
//    TXB0EIDH = 0x00; /* Extended Identifier */
//    TXB0EIDL = 0x00; /* Extended Identifier */
//
//    /* 0x35E  0110 1011 110 */
//    TXB0SIDH = msg; /* Standard Identifier */
//    TXB0SIDL = 0x00; /* Standard Identifier */
//
//    TXB0DLC = dl; //0x08;			/* Data Length Count */
//    TXB0D0 = data[0]; //'A';			/* DataByte 0 */
// //   clcd_putch(TXB0D0, LINE2(14));
//    TXB0D1 = data[1]; /* DataByte 1 */
//   // clcd_putch(TXB0D1, LINE2(15));
//    TXB0D2 = data[2]; /* DataByte 2 */
//    TXB0D3 = data[3]; /* DataByte 3 */
//    TXB0D4 = data[4]; /* DataByte 4 */
//    TXB0D5 = data[5]; /* DataByte 5 */
//    TXB0D6 = data[6]; /* DataByte 6 */
//    TXB0D7 = data[7]; /* DataByte 7 */
//
//    TXB0REQ = 1; /* Set the buffer to transmit */
//    while (TXB0REQ);
//    if (TXB0CONbits.TXERR)
//        return 0;
//    return 1;
//
//}

