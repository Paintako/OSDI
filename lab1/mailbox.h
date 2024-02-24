/*
The mailbox mechanism consists of three components mailbox registers, channels,
and messages.
*/

#ifndef MAILBOX_H
#define MAILBOX_H

/* channels */
#define MBOX_CH_POWER 0
#define MBOX_CH_FB 1
#define MBOX_CH_VUART 2
#define MBOX_CH_VCHIQ 3
#define MBOX_CH_LEDS 4
#define MBOX_CH_BTNS 5
#define MBOX_CH_TOUCH 6
#define MBOX_CH_COUNT 7
#define MBOX_CH_PROP 8 // we only use channel 8 (CPU->GPU) for communication.

int mbox_call(unsigned int *mbox, unsigned char ch);
void mbox_board_revision();

#endif
