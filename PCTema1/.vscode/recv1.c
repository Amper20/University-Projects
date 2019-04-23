#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"
#include "utils.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc,char** argv){
  	msg r,t;
	ack ack_snd;
  	init(HOST,PORT);

  	while(1){
    		if(recv_message_timeout(&r, 10) > 0){
			custom_payload current_packet;
			memcpy(&current_packet, r.payload, sizeof(r.payload));
			//printf("receiver :%d %d\n",getSum(current_packet.data, current_packet.len) , current_packet.sum);
			
			if(getSum(current_packet.data, current_packet.len) == current_packet.sum){
				ack_snd.received = current_packet.packet_num;
				
			}else
			{
				ack_snd.received = -1;
			}
			memcpy(t.payload, &ack_snd, sizeof(ack_snd));
			send_message(&t);
		}
		
  	}
  	return 0;
}
