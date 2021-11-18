// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#define PORT 8000 

typedef struct {
	
	uint8_t u8sof ;
	uint8_t u8Sensor ;
	uint8_t u8Data ;
	uint8_t u8Checksum ;
	
} Myframe;

typedef struct {
	
	uint8_t u8sof ;
	uint8_t u8Sensor ;
	uint8_t u8Data ;
	uint8_t u8Checksum ;
	
} MyReceivedframe;



Myframe Mytrama;
MyReceivedframe MyReceivedtrama;
//Pensado meterse en una librería .h
int32_t server_fd; 
int32_t new_socket; 
uint32_t valread;
struct sockaddr_in address;
int32_t opt = 1;
int32_t addrlen = sizeof(address);
unsigned char buffer[1024] = {0};
unsigned char ValTosend[4] = {0}; 	

void vfn_ServerInit(void);
void vfn_PrepPack(void);
uint8_t vfn_VerifPack();

void vfn_ServerInit(void){

// Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
       // return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
       // return 1;
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
       // return -1;
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
       // return 1;
    }

}
int main(int argc, char  *argv[])
{
    
	
    char *hello = "Hello from server";
	 char *Error = "ENVIANDO ERROR";
	 
	vfn_ServerInit();
	
    valread = read( new_socket, buffer, 1024);
	
    if(vfn_VerifPack()){
		printf("****VALORES RECIBIDOS****\n");
		printf("%x\n",buffer[0]);
		printf("%x\n",buffer[1]);
		printf("%x\n",buffer[2]);
		printf("%x\n",buffer[3]);
	} else {
		send(new_socket, Error, strlen(Error), 0);
		send(new_socket, ValTosend, 4, 0);
		return 0;
	}
	
	vfn_PrepPack();
	
	send(new_socket , ValTosend , 4 , 0 );
	printf("VALORES ENVIADOS\n");
	printf("%x\n",ValTosend[0]);
    printf("%x\n",ValTosend[1]);
    printf("%x\n",ValTosend[2]);
    printf("%x\n",ValTosend[3]);
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent\n");
    return 0;
}

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

uint8_t vfn_VerifPack(){
	
		MyReceivedtrama.u8sof = buffer[0];
		MyReceivedtrama.u8Sensor = buffer[1];
		MyReceivedtrama.u8Data = buffer[2];
		MyReceivedtrama.u8Checksum = buffer[3];
	
	if(MyReceivedtrama.u8Checksum == 0x55 || MyReceivedtrama.u8Checksum == 0x56){
		printf("El valor es correcto");
		return 1;
	} else if (MyReceivedtrama.u8Sensor != 0xFF) {
		MyReceivedtrama.u8Sensor = 0xFE;
		MyReceivedtrama.u8Checksum = 0xFF;
		return 0;	
	} else {
		MyReceivedtrama.u8Sensor = 0xFE;
		MyReceivedtrama.u8Checksum = 0xFF;
		return 0;
	}
	
		buffer[0]  = MyReceivedtrama.u8sof;
		buffer[1]  = MyReceivedtrama.u8Sensor;
		buffer[2]  = MyReceivedtrama.u8Data;
		buffer[3]  = MyReceivedtrama.u8Checksum;
	
}