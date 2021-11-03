#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint32_t.h>

void vfn_gpio(void);
void vfn_delay(void);
void vfn_loop(void);
void u8_read();
char u8_readbtn();
void vfn_LEDS(char Led_value);
void vfn_RecordState(char u8_data);
void u8_PlayState();

int32_t u32_TempTime;


int32_t main(void){
        vfn_gpio();
        vfn_loop();
}

void vfn_gpio(void){
        //Inputs
        int32_t32_t btn1 = open("/sys/class/gpio/export", O_WRONLY);
        write(btn1,"26",2);
        close(btn1);

        int32_t btn2 = open("/sys/class/gpio/export", O_WRONLY);
        write(btn2,"19",2);
        close(btn2);

        int32_t btn3 = open("/sys/class/gpio/export", O_WRONLY);
        write(btn3,"13",2);
        close(btn3);

        int32_t btn4 = open("/sys/class/gpio/export", O_WRONLY);
        write(btn4,"6",2);
        close(btn4);
        
        int32_t sw1 = open("/sys/class/gpio/export", O_WRONLY);
        write(sw1,"25",2);
        close(sw1);

        int32_t sw2 = open("/sys/class/gpio/export", O_WRONLY);
        write(sw2,"5",2);
        close(sw2);

	 //Outputs
        int32_t fd1 = open("/sys/class/gpio/export",O_WRONLY);
        write(fd1,"21",2);
        close(fd1);

        fd1 = open("/sys/class/gpio/gpio21/direction",O_WRONLY);
        write(fd1,"out",3);
        close(fd1);

        int32_t fd2 = open("/sys/class/gpio/export", O_WRONLY);
        write(fd2,"20",2);
        close(fd2);

	 int32_t fd3 = open("/sys/class/gpio/export",O_WRONLY);
        write(fd3,"16",2);
        close(fd3);

        fd3 = open("/sys/class/gpio/gpio16/direction",O_WRONLY);
        write(fd3,"out",3);
        close(fd3);

        int32_t fd4 = open("/sys/class/gpio/export",O_WRONLY);
        write(fd4,"12",2);
        close(fd4);

        fd4 = open("/sys/class/gpio/gpio12/direction",O_WRONLY);
        write(fd4,"out",3);
        close(fd4);
}

void vfn_loop(void){
 	int32_t u32Contador = 0;
        if(u32 Contador == 4000000){
                u8_read();
                vfn_delay();
		u32Contador = 0;
        }
	u32Contador++;
}

void  u8_read(){
        char Switch1[2];
        char Switch2[2];
        char u8_Valorbtn;

        int32_t sw1 = open("/sys/class/gpio/gpio25/value", O_RDONLY);
        read(sw1,Switch1,1);

        int32_t sw2 = open("/sys/class/gpio/gpio5/value", O_RDONLY);
        read(sw2,Switch2,1);

	 if(Switch1[0] == Switch2[0]){ //Do nothing state
                fflush(stdout);
                printf("Secuencia detenida\n");
		printf("Tiempo de grabacion = %d", u32_TempTime);
		u32_TempTime = 0;
                fflush(stdout);
        } else if(Switch1[0] == '1' && Switch2[0] == '0'){ //Recording state
                fflush(stdout);
                printf("Secuencia de grabacion\n");
                //Poner la función de lectura de los botones
                u8_Valorbtn = u8_readbtn();
                //Guardar lo leído por parte de eso (void vfn_record(int32_t u8_btn$
                //Encender LEDS
                vfn_RecordState(u8_Valorbtn);
                vfn_LEDS(u8_Valorbtn);
                fflush(stdout);
	 }else if(Switch1[0] == '0' && Switch2[0] == '1'){ //Reproduction state
                fflush(stdout);
                printf("Reprodccion\n");
                //leer la secuencia de los leds
                u8_PlayState();

                fflush(stdout);
        }
        close(sw1);
        close(sw2);
}

char u8_readbtn(){
        char button1[2];
        char button2[2];
        char button3[2];
        char button4[2];

        int32_t btn1 = open("/sys/class/gpio/gpio26/value", O_RDONLY);
        read(btn1,button1,1);

        int32_t btn2 = open("/sys/class/gpio/gpio19/value", O_RDONLY);
        read(btn2,button2,1);

        int32_t btn3 = open("/sys/class/gpio/gpio13/value", O_RDONLY);
        read(btn3,button3,1);

        int32_t btn4 = open("/sys/class/gpio/gpio6/value", O_RDONLY);
        read(btn4,button4,1);

	  if(button1[0] == '1'){
                printf("Botón 1 se presiona\t");
                return '1';
        } else if (button2[0] == '1'){
                printf("Boton 2 se presiona\t");
                return '2';
        } else if (button3[0] == '1'){
                printf("Boton 3 se presiona\t");
                return '3';

        } else if (button4[0] == '1'){
                printf("Boton 4 se presiona\t");
		   return '4';
        }else{
                return '0';
        }

        close(btn1);
        close(btn2);
        close(btn3);
        close(btn4);
}

void vfn_LEDS(char Led_value){

        //Outputs
        int32_t fd1 = open("/sys/class/gpio/gpio21/value",O_WRONLY);
        int32_t fd2 = open("/sys/class/gpio/gpio20/value",O_WRONLY);
        int32_t fd3 = open("/sys/class/gpio/gpio16/value",O_WRONLY);
        int32_t fd4 = open("/sys/class/gpio/gpio12/value",O_WRONLY);


                if(Led_value == '1'){
                        write(fd1,"1",1);
                        write(fd2,"0",1);
                        write(fd3,"0",1);
                        write(fd4,"0",1);
                        printf("LED1");
                } else if(Led_value == '2'){
                        write(fd1,"0",1);
                        write(fd2,"1",1);
			  write(fd3,"0",1);
                        write(fd4,"0",1);
                        printf("LED2");
                } else if(Led_value == '3'){
                        write(fd1,"0",1);
                        write(fd2,"0",1);
                        write(fd3,"1",1);
                        write(fd4,"0",1);
                        printf("LED3");
                } else if(Led_value == '4'){
		        write(fd1,"0",1);
                        write(fd2,"0",1);
                        write(fd3,"0",1);
                        write(fd4,"1",1);
                        printf("LED4");
                }

}

void vfn_RecordState(char u8_data){
	int32_t u32Contador;
        FILE *fp1;
        fp1 = fopen("Prueba.txt", "a");


        if(fp1 == NULL){
                printf("NO SE PUDO ABRIR");
                exit(1);
	 } else if (fp1 != NULL){
		
                printf("\n\n\n %c\n\n",u8_data);
                fputc(u8_data,fp1);
		u32Contador++;
                fclose(fp1);
        }

	if(u32Contador == 25){
		u32TempTime++;
	}

}


void u8_PlayState(){
	int32_t u32_cont;
	static int32_t u32_ContSeq = 0;
        char  u8_read;
        FILE *fp1;
	
	
	if(u32_ContSeq == 0){

		 fp1 = fopen("Prueba.txt","r");
       		 fseek(fp1,0,SEEK_END);
	        long u32_ArrSize = ftell(fp1);
	        rewind(fp1);
		u32_ContSeq = 1;
			
	} else if(u32_contSeq < u32_ArrSize){
                	u8_read = fgetc(fp1);
                	printf("%c\n",u8_read);
                	vfn_LEDS(u8_read);
                	u32_contSeq++;
	} else {

		u32contSeq = 1;
	
	}
        
       
        fclose(fp1);

}

