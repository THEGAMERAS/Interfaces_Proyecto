// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#define PORT 8000

typedef struct {
	
	uint8_t u8sof ;
	uint8_t u8Sensor ;
	uint8_t u8Data ;
	uint8_t u8Checksum ;
	
} Myframe;

void vfn_SocketInit(void);
void vfn_PrepPack(void);


Myframe Mytrama;
//Pensado implementarse en una librería
int sock = 0;
char buffer[1024] = {0};
char u8ValRead = 0;
unsigned char ValTosend[4] = {0xAA,0xFF,0x0,0x0}; 
void vfn_SocketInit(void){
	
	
	struct sockaddr_in serv_addr;
	
	 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        //return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "192.168.100.49", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        //return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        //return -1;
    }
}
   
int main(int argc, char const *argv[])
{
    int valread;
    
    char *Error = "ENVIANDO ERROR";
   
	
	vfn_SocketInit();
	vfn_PrepPack();
    //send(sock , hello , strlen(hello) , 0 );
	send(sock, ValTosend, 4, 0);
    printf("Hello message sent\n");
	printf("****VALORES ENVIADOS****\n");
	printf("%x\n",ValTosend[0]);
    printf("%x\n",ValTosend[1]);
    printf("%x\n",ValTosend[2]);
    printf("%x\n",ValTosend[3]);
    valread = read( sock , buffer, 1024);
    
	
    return 0;
}

/*void vfn_PrepPack(void){
	
	uint8_t u32Checksum = 0;
	uint8_t u8Cont = 0;
	
	if(u8ValRead == 1){
		ValTosend[2] = 0x1;
	} else {
		ValTosend[2] = 0x0;
	}
	
	while(u8Cont < 3){
		u32Checksum += ValTosend[u8Cont];
		printf("%x\n",u32Checksum);
		u8Cont++;
	}
	//printf("%x\n",u32Checksum);
	u32Checksum = ~u32Checksum;
	//printf("%x\n",u32Checksum);
	ValTosend[3] = u32Checksum;
	u8Cont = 0;
	
}*/

void vfn_PrepPack(void){
	
	uint8_t u32Checksum = 0;
	uint8_t u8Cont = 0;
	
	Mytrama.u8sof = 0xAA;
	ValTosend[0] = Mytrama.u8sof;
	Mytrama.u8Sensor = 0xFF;
	ValTosend[1] = Mytrama.u8Sensor;
	Mytrama.u8Sensor = 0x01;
	ValTosend[2] = Mytrama.u8Data;
	
	while(u8Cont < 3){
		u32Checksum += ValTosend[u8Cont];
		printf("%x\n",u32Checksum);
		u8Cont++;
	}
	
	u32Checksum = ~u32Checksum;
	Mytrama.u8Checksum = u32Checksum;
	
	ValTosend[3] = Mytrama.u8Checksum;
	
	u8Cont = 0;
	
}