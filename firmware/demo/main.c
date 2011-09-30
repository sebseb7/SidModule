#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int tty_fd;
void sid_flush(void);
void write_reg(int,int);
 
int main(int argc,char** argv)
{
        struct termios tio;
 
        printf("Please start with %s /dev/ttyUSB0 (for example)\n",argv[0]);
 
        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;
 
        tty_fd=open(argv[1], O_RDWR | O_NONBLOCK);      
        cfsetospeed(&tio,B115200);            // 115200 baud
        cfsetispeed(&tio,B115200);            // 115200 baud
 
        tcsetattr(tty_fd,TCSANOW,&tio);

		int addr = 0;
		for(addr = 0; addr < 25;addr++)
        {
			write_reg(addr,0);
		}
		write_reg(1,0x50);
		write_reg(5,0x37);
		write_reg(6,0x73);
		write_reg(24,0x08);
		sid_flush();

        while (1)
        {
			write_reg(4,0x20);
			sid_flush();
			sleep(1);
			write_reg(4,0x21);
			sid_flush();
			sleep(1);
        }
 
        close(tty_fd);
}

void write_reg(int addr, int data)
{
	int c=addr; 
	write(tty_fd,&c,1);
	c=data;
	write(tty_fd,&c,1);
}

void sid_flush(void)
{
	int c=0xf7;
	write(tty_fd,&c,1);
}

