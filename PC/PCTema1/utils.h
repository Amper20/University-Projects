#ifndef UTILS
#define UTILS

#include "link_emulator/lib.h"

#define getPacketsNum(type) (((double)sizeof(type) * (double)(fileSize / MSGSIZE)) + sizeof(type) * 100)
#define ack_q_size  200  //should be around 200-300

char getSum(char *data, int len, int packet_num);
int min( int a, int b);
int getTimeout(int speed, int delay);

typedef struct cust_pay{
  	char sum;
  	int packet_num;
  	int len;
  	char data[MSGSIZE - 2*sizeof(int) - sizeof(char)];
} custom_payload;

typedef struct {
  	int received;
} ack;


struct messages_queue{
	int key;
	int when;
  	custom_payload pay;
};

#endif
