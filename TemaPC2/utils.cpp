#include "utils.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

void errorOccured(string message, int lineNum){
	cout << "exited with error message: --> "<< message << " " << lineNum << " <-- \n";
	exit(0);
}

int getPort(struct sockaddr_in *sin){
	return htons (sin->sin_port);
}

char* getIP(struct sockaddr_in *sin){
	return inet_ntoa(sin->sin_addr);
}

int getSFandTopic(string buffAsString, string & topic){
    topic = "";
    unsigned int j = 0;
    for(; j < buffAsString.length(); j++){
        if(buffAsString [j] == ' '){
            for(;j + 1 < buffAsString.length() && buffAsString[j + 1] != ' '; j++, topic += buffAsString[j]);
            break;
        }
    }
    if (buffAsString.find("unsubscribe") != string::npos){
        j+=2;
        return (int)(buffAsString[j] - '0');
    }
    return 0;
}

string generateMessage(string &topic, char *buff, int rec, struct sockaddr_in * udpAddr){

    string message = "";

    if (rec > 0){
        int i = 0;
        for(i = 0 ; i < 50 ; i++){
            if(buff[i] != '\0')
                topic += buff[i];
        }
        int type = (int) buff[i];
        i++; //jump over type
        string data = "";
        switch (type){
            case 0: {
                string sign = buff[i];
                i++; // jump over sign

                uint32_t val = 0;
                for(int k = 0 ; k < 4; k++){
                    val = val << 4;
                    val += (int) buff[i + k];
                }
                if(val == 0)
                    data += "0";
                while(val){
                    data += '0' + val%10;
                    val/=10;
                }
                reverse(data.begin(), data.end());
                data = " - INT - " + sign + data;
            }
            case 1:{
                uint16_t val = 0;
                for(int k = 0 ; k < 2; k++){
                    val = val << 4;
                    val += (int) buff[i + k];
                }
                for(int k = 0; k < 3 || val; k++){
                    data += '0' + val%10;
                    val/=10;
                }
                reverse(data.begin(), data.end());
                data = " - SHORT REAL - " data.substr(0, data.lengt() - 2)  + "." + data.substr(data.lengt() - 1, data.lengt());
            }
            case 2:{
                string sign = buff[i];
                i++; // jump over sign

                uint32_t val = 0;
                for(int k = 0 ; k < 4; k++){
                    val = val << 4;
                    val += (int) buff[i + k];
                }
                if(val == 0)
                    data += "0";
                while(val){
                    data += '0' + val%10;
                    val/=10;
                }
                reverse(data.begin(), data.end());
                uint8_t k = (uint8_t) buff[i + 4]; // get jmp
                data = " - FLOAT - " + sign + data.substr(0, data.lengt() - k)  + "." + data.substr(data.lengt() - k + 1, data.lengt());
                
            }
            case 3:{
                for(int j = i; j < 1500 + i; j++){
                    if(buff[j] != '\0')
                        data += buff[j]; 
                }
                data = " - STRING - " + data;
            }
            default: {
                data = "TYPE NOT DEFINED";
                break;
            }



        }
    }
}