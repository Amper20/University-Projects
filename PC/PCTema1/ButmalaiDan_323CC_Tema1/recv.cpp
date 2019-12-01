#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
extern "C" 
{
	#include "link_emulator/lib.h"
	#include "utils.h"
}

#include <iostream>
#include <map>

#define HOST "127.0.0.1"
#define PORT 10001

//functia deschide si intoarce file descriptorul pentru fisierul de iesire
int get_file(struct cust_pay * current_packet){
	current_packet->data[current_packet->len] = '\0';
	char file_name[30];
	strcpy(file_name,"recv_");
	strcat(file_name, current_packet->data);
	return open(file_name, O_CREAT|O_WRONLY|O_TRUNC, 0666);	
}

int main(int argc,char** argv){
	msg r,t;
	ack ack_snd;
  	init(HOST,PORT);

	//ne vom folosi de un map din stl<3 pentru a pastra packetele out of order 
	std::map <int, struct cust_pay *> data;
	int procesed[10000] = {0}, index = 1, file = -10;
  	
	while(1){
    	if(recv_message_timeout(&r, 5) > 0){
			
			struct cust_pay *current_packet = (struct cust_pay *)malloc(sizeof(struct cust_pay));
			memcpy(current_packet, r.payload, sizeof(r.payload));
			if(current_packet->len  == -2) break;		
			
			std::cout << "received" << current_packet->packet_num << "\n";
			if(getSum(current_packet->data, current_packet->len, current_packet->packet_num) == current_packet->sum){
				
				ack_snd.received = current_packet->packet_num;
				
				if(current_packet->packet_num == 0 && file == -10)
					file = get_file(current_packet);
				else
					if(!procesed[current_packet->packet_num] && current_packet->packet_num != 0){
						data[current_packet->packet_num] = current_packet;
						procesed[current_packet->packet_num] = 1;
					}
				
				memcpy(t.payload, &ack_snd, sizeof(ack_snd));
				send_message(&t);
			}
		}
		//atat timp cat putem afisa packete care sunt in order le vom scrie in out
		while(data.size() && (file != -10) && (data.begin()->first == index)){
			write(file, data[index]->data, data[index]->len);
			data[index]->len;
			free(data[index]);
		  	data.erase(index);
		  	index++;
		}	
  	}
	std::cout << index; 
	close(file);
	ack_snd.received = -2;
	memcpy(t.payload, &ack_snd, sizeof(ack_snd));
	send_message(&t);
  	return 0;
}
