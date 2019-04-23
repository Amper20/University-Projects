
#include "utils.h"

//intoarce suma payloadului curent
char getSum(char *data, int len, int packet_num){
	if(len <= 0 || len > MSGSIZE - 4*sizeof(int))
		return -1;
  	char ret = 0;
  	for(int i = 0; i < len; i++)
    		ret = ret ^ data[i];
	char *ln = (char *)(&len);
	for(int i = 0 ; i < 4; i++)
		ret = ret ^ ln[i];
  	char *pk = (char *)(&packet_num);
	for(int i = 0 ; i < 4; i++)
		ret = ret ^ pk[i];
	return ret;
}

//guess what it does :)
int min( int a, int b){
	if(a < b)return a;
	return b;
}

//intoarce un timeout optim pentru un speed si delay
int getTimeout(int speed, int delay){
	int timeout = (delay >= 90 ? 1: delay/3);
	if(speed == 5 && delay >= 90) timeout = delay/2;
	if((speed == 20) && delay >=10 && delay <= 90) timeout = 2;
	if(speed == 10 && delay >= 10 && delay <= 90 ) timeout = 4;
	return timeout;
}