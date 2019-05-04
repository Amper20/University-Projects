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
#include <algorithm>
#include <cmath>
using namespace std;

//throws an exceptions kills the program
void errorOccured(string message, int lineNum){
	cout << "exited with error message: --> "<< message << " " << lineNum << " <-- \n";
	exit(0);
}

//gets Port from adress
int getPort(struct sockaddr_in *sin){
	return htons (sin->sin_port);
}

//gets port from IP
char* getIP(struct sockaddr_in *sin){
	return inet_ntoa(sin->sin_addr);
}

//gets data from subscription message
int getSFandTopic(string buffAsString, string & topic){
    topic = "";
    unsigned int j = 0;
    for(; j < buffAsString.length(); j++)
        if(buffAsString [j] == ' '){
            for(;j + 1 < buffAsString.length() && buffAsString[j + 1] != ' ' && buffAsString[j + 1] != '\n'; j++, topic += buffAsString[j]);
            break;
        }
    if (buffAsString.find("unsubscribe") == string::npos)
        return (int)(buffAsString[j + 2] - '0');
    return 0;
}

//generates generates message for tcp client
string generateMessage(string &topic, char *buff, int rec, struct sockaddr_in * udpAddr){
    string data = "";
    if (rec > 0){
        int i = 0;
        for(i = 0 ; i < 50 ; i++)
            if(buff[i] != '\0')
                topic += buff[i];
        int type = (int) buff[i];
        i++; //jump over type
        switch (type){
            case 0: {
                string sign = ((int)buff[i] == 0 ? "" :"-");
                i++; // jump over sign
                uint32_t val = 0;
                for(int k = 0 ; k < 4; k++){
                    val = val << 8;
                    val += (unsigned char) buff[i + k];
                }
                if(val == 0) data += "0";
                for(;val;val/=10)
                    data += ('0' + val%10);
                reverse(data.begin(), data.end());
                data = " - INT - " + sign + data;
                break;
            }
            case 1:{
                uint16_t val = 0;
                for(int k = 0 ; k < 2; k++){
                    val = val << 8;
                    val += (unsigned char) buff[i + k];
                }
                for(int k = 0; k < 3 || val; k++, val/=10)
                    data += '0' + val%10;
                reverse(data.begin(), data.end());
                data = " - SHORT REAL - " + data.substr(0, data.length() - 2)  + "." + data.substr(data.length() - 2, data.length());
                break;
            }
            case 2:{
                int sign = ((int)buff[i] == 0 ? 1 : -1);
                i++; // jump over sign
                uint32_t val = 0;
                for(int k = 0 ; k < 4; k++){
                    val = val << 8;
                    val += (unsigned char)buff[i + k];
                }
                float valFloat = val;
                valFloat *= (sign * 1.0) * pow(10, -(unsigned char)buff[i+4]);
                char dataChr[25];
                sprintf(dataChr, "%.*f", (unsigned char)buff[i+4], valFloat);
                data = dataChr;
                data = " - FLOAT - " + data;
                break;
            }
            case 3:{
                for(int j = i; j < 1500 + i; j++)
                    if(buff[j] != '\0')
                        data += buff[j]; 
                    else 
                        break;
                data = " - STRING - " + data;
                break;
            }
            default: {
                data = "TYPE NOT DEFINED";
                break;
            }
        }
    }
    //1.2.3.4:4573 - UPB/precis/1/temperature
    string ip = getIP(udpAddr) ;
    char port[20];
    sprintf(port, "%d", getPort(udpAddr));
    string posrtString = port;
    return  ip + ":" + posrtString + " - " + topic +  data;
}