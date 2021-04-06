#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#define FOSC 1843200 // clock speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr);
void update_clock_speed(void);
void USART_Transmit( unsigned char data);
void print(const uint8_t *buffer, size_t n);
void print2(const char* str);


void main(void)
{
    update_clock_speed();
    USART_Init(MYUBRR);

    while(1)
    {
        print2("test\n");
    }
}

void USART_Init(unsigned int ubrr)
{
    /* set baud rate */

    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;

    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data)
{
    while (!(UCSR0A & (1<<UDRE0)));

    UDR0 = data;
}

void print(const uint8_t *buffer, size_t n)
{
    while (n--)
        USART_Transmit(*buffer++);
}

void print2(const char* str)
{
    if (strlen(str) !=0) print((const uint8_t *) str, strlen(str));
}


void update_clock_speed(void)
{
    char temp;
    char mod;

    temp = eeprom_read_byte((void *)1);

    if (temp==0 || temp==1)
    {
        mod = eeprom_read_byte((void *) 0);
        if (temp==0)
        {
            if (mod != 0xff) OSCCAL+=mod;
        }
        else
        {
            if (mod != 0xff) OSCCAL-=mod;
        }
    }
}