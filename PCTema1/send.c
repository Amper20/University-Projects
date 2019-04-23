#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include "utils.h"

#define HOST "127.0.0.1"
#define PORT 10000

//functia calculeaza marimea ferestrei si marimea fisierului 
void initialSetUp(char *argv[], long long *fileSize, int *bandWidht, int *delay, int *w){
  
  	struct stat st;
  	stat(argv[1],&st);
	*fileSize = (st.st_size);
  
  	*bandWidht = atoi(argv[2]);
  	*delay = atoi(argv[3]); 

	double factor = ((1000.0)/(MSGSIZE*8.0));  
  	*w = ((*bandWidht) * (*delay)) * (factor);

}


//funtia populeaza un packet cu date
void generatePacket(char *data, int len, int packet_cnter, struct messages_queue *packet){
  
  	packet->pay.sum = getSum(data, len ,packet_cnter);
  	packet->key = -1;
  	packet->pay.len = len;
  	packet->pay.packet_num = packet_cnter;
  
  	memcpy (packet->pay.data, data, len);
}

//functia populeaza arrayul de packete 
int generatePackets(char *fileName, struct messages_queue * sent ){

  	int packet_cnter = 1, number_of_bits = MSGSIZE - 4*sizeof(int), cnt = 0;
  	char data[MSGSIZE - 3*sizeof(int) + 1];
    int file = open(fileName, O_RDONLY);
	//vom genera primul packet ca o entiatate separata pentru a putea salva numele fisierului
  	generatePacket(fileName, strlen(fileName), 0, &sent[0]);
	for(;1; packet_cnter++){
		cnt = read(file, data, number_of_bits);
		if (!cnt) break;
		generatePacket(data, cnt, packet_cnter, &sent[packet_cnter]);
  	}
  	return packet_cnter;

}

//funtia intoarce true daca mai avem packete nack si false daca am primit ack pentru toate packetele
int checkPachets(struct messages_queue * packets, int n){
	for(int i = 0; i < n; i++)
		if(packets[i].key == -1)
			return 1;
	return 0;
}

//functia face update marcand packetele primite cu 1
void updateKeys(msg ack_mes, struct messages_queue * packets, int len){
	ack ack_cur;
	memcpy(&ack_cur, &ack_mes.payload, sizeof(ack));
	printf("ack %d\n", ack_cur.received);
	if(ack_cur.received >= 0 && ack_cur.received < len)
		packets[ack_cur.received].key = 1;
}

//functia intoarce packetul care nu se afla in window si inca nu a fost expediat
int getMissingPacket(struct messages_queue * packets, int w, 
			int packetsSize, int timer, int *start){
	
	while(*start < packetsSize && packets[*start].key > 0)
		(*start)++;

	for(int i = *start; i < packetsSize; i++){
		//verificam daca ar fi trebuit sa primi packetul la timpul timer
		if(packets[i].key < 0 && timer >= packets[i].when + w + 4){
			return i;
		}
	}
	return -1;
}

int main(int argc,char** argv){

  	init(HOST,PORT);
  	msg current_ack, message;

  	long long fileSize;
  	int bandWidth, delay, w, start = 0;

	//initializam arrayul de packete, timerul si arrayul productivity 
  	initialSetUp(argv, &fileSize, &bandWidth, &delay, &w);
  	struct messages_queue * packets = (struct messages_queue *) malloc(getPacketsNum(struct messages_queue) + 50);
  	memset(packets, 0, getPacketsNum(struct messages_queue) + 50);
  	int packetsSize = generatePackets(argv[1], packets), timeout = getTimeout(bandWidth,delay);
	int timer = 0,  productivity[10000]= {0}, cnt_loss = 0;

	//trimitem packete cat sa uplem windowul
  	for (int i = 0; i < min(w,packetsSize); i++, timer++){
		if(packets[i].key < 0){
			msg message;
			packets[i].key = -1;
			packets[i].when = timer;
			memcpy (message.payload, &packets[i].pay, MSGSIZE);
			message.len = MSGSIZE;
			send_message(&message);
		}
  	}

	//cat timp mai avem packete netrimise le trimitem
	for(;checkPachets(packets, packetsSize);){
		if(recv_message_timeout(&current_ack, timeout) > 0){
			updateKeys(current_ack, packets, packetsSize);//updatam keyul pentru ackul primit
			timer++;
		}
		//obtinem numarul packetului lipsa
		int index = getMissingPacket(packets , w, packetsSize, timer, &start);
		printf("sent %d %d\n", index, timer);
		if(cnt_loss == 5) timer++, cnt_loss = 0;
		if(index != -1){
			//trimitem mesajul daca e posibil
			productivity[index]++;
			packets[index].when = timer;
			memcpy (message.payload, &packets[index].pay, MSGSIZE);
			message.len = MSGSIZE;
			send_message(&message);
		}else	
			cnt_loss++;
	}
	
	//verificam daca s-a terminat scrierea fisierelor si inchidem procesul
	struct messages_queue packet;
	packet.pay.len = -2;
	memcpy (message.payload, &packet.pay, MSGSIZE);
	message.len = MSGSIZE;
		
	while(1){
		send_message(&message);
		if(recv_message_timeout(&current_ack, 4) > 0){
			ack ack_cur;
			memcpy(&ack_cur, &current_ack.payload, sizeof(ack));
			if(ack_cur.received ==-2)
				return 0;
		}
	}
  	return 0;
}
