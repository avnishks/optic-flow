#include "mbed.h"

SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);
int read_register(int address);
Serial pc(USBTX, USBRX); // tx, rx

int main()
{
    spi.format(8,3);
    spi.frequency(1000000);
    int i;
// pc.baud(38400);

    while(i<100) {
        cs = 0;

        //spi.write(0x02);
        wait(.000075);
        int reg2 = read_register(0x02); // Reg 0x02 to determine if motion has occurred since the last time it was read
        pc.printf("02 Reg =  0x%X \t",reg2);
        //  pc.printf(" \n");
        // pc.printf("Motion read in integer = %d",reg2);
        //  pc.printf(" \n");
        // if(reg2 & 0x80)
        //{pc.printf("Motion Occured \n");
        wait(.0000005);
        //0.0001,0.01 gives zero reading may be bcz light insufficient
        {
            //spi.write(0x03);//dx]
            wait(.00006);
            int dx = read_register(0x03);
            pc.printf("dx = 0x%X \t",dx);
            //   pc.printf(" \n");
            pc.printf("(%d)\t",dx);
            //   pc.printf(" \n");
            wait(.0000005);

            //spi.write(0x04);//dy
            wait(.00006);
            int dy = read_register(0x04);
            pc.printf("dy = 0x%X \t" ,dy);
            //   pc.printf(" \n");
            pc.printf("(%d)\t",dy);
            //   pc.printf(" \n");
            wait(.0000005);

        }

        if(reg2 & 0x10)
            pc.printf("Overflow Occured ");

        pc.printf("\n");

        cs = 1;
        i++;
    }
}
int
read_register(int address)
{
    char result = 0, junk = 0;

    //backup_spi_settings();

    // take the chip select low to select the device
    spi.format(8,3);
    spi.frequency(1000000);
    // no problem with frequency of 1 MHz
    // chip select should be 0 and then turned 1 , earlier chip select was always grounded to 0 ,
    // Select the device by seting chip select low
    cs = 0;

    // Send 0x8f, the command to read the WHOAMI register
    spi.write(address);
    wait(.00006);
    // Send a dummy byte to receive the contents of the WHOAMI register
    int results = spi.write(0x00);
    //wait(SPI3_wait);
    // pc.printf(" Value on register %X\n", results);

    // Deselect the device
    cs = 1;
    //digitalWrite(_cs_pin, LOW); // ARD

    return results;
}
