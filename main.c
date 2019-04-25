#include <avr/io.h>
#include <avr/iom8.h>
#define F_CPU 8000000UL // частота резонатора 1МГц
#include <util/delay.h>
#include <avr/interrupt.h>


#define SPI_PORTX PORTB
#define SPI_DDRX DDRB

#define SPI_MISO 4
#define SPI_MOSI 3
#define SPI_SCK 5
#define SPI_SS 2



void SPI_Init(void);
void SPI_WriteByte(uint8_t data);
ISR(TIMER2_OVF_vect);
void Timer2_Init(void);

//void Tick();
//void flash();
//void checkTime();

int seconds = 0;

uint8_t i = 1;
uint8_t m = 2;
uint8_t n = 3;
uint8_t t = 4;

uint8_t digit[10] = {
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};

int main(void)
{


    DDRC = 0b11111001;

    PORTC |= 1 << 0;    // Установить 0й бит
    PORTC |= 1 << 1;
    PORTC |= 1 << 2;

    PORTC &= (~(1<<3));  // обнулить 4й бит

    Timer2_Init();
    SPI_Init();

    //DDRD = 0xFF;PORTC = 0xFF;


    //uint8_t byte = 0x00;


    while(1)
    {

        //SPI_WriteByte(~digit[i]);
        //SPI_WriteByte(~digit[i]);



        flash();






                if ((PINC & (1<<1)) == 0)
                {
                    // блок будет выполняться, только если сброшен
                    // первый бит переменной tmp
                    int in;
                    for (in = 0 ; in < 3 ; in ++)
                    {
                        SPI_WriteByte(~0b00001000);
                        SPI_WriteByte(0x7F);

                        PORTC |= 1 << 0;
                        _delay_ms(50);
                        PORTC &= (~(1<<0));

                        SPI_WriteByte(~0b00000100);
                        SPI_WriteByte(0x7F);

                        PORTC |= 1 << 0;
                        _delay_ms(50);
                        PORTC &= (~(1<<0));


                        SPI_WriteByte(~0b00000010);
                        SPI_WriteByte(0x7F);

                        PORTC |= 1 << 0;
                        _delay_ms(50);
                        PORTC &= (~(1<<0));


                        SPI_WriteByte(~0b00000001);
                        SPI_WriteByte(0x7F);

                        PORTC |= 1 << 0;
                        _delay_ms(50);
                        PORTC &= (~(1<<0));

                    }

                    i = 0;
                    m = 0;
                    n = 0;
                    t = 0;

                    cli();

                    uint8_t temp = 0;


                    while (1)
                    {
                        _delay_ms(100);
                        if(temp == 0)
                        {
                            SPI_WriteByte(~0b00001000);
                            SPI_WriteByte(~digit[t]);

                            PORTC |= 1 << 0;
                            _delay_ms(1);
                            PORTC &= (~(1<<0));

                            if ((PINC & (1<<2)) == 0)
                            {
                                t++;
                                _delay_ms(50);
                            }

                        }

                        if(temp == 1)
                        {
                            SPI_WriteByte(~0b00000100);
                            SPI_WriteByte(~digit[n]);

                            PORTC |= 1 << 0;
                            _delay_ms(1);
                            PORTC &= (~(1<<0));

                            if ((PINC & (1<<2)) == 0)
                            {
                                n++;
                                _delay_ms(50);
                            }

                        }


                        if(temp == 2)
                        {
                            SPI_WriteByte(~0b00000010);
                            SPI_WriteByte(~digit[m]);

                            PORTC |= 1 << 0;
                            _delay_ms(1);
                            PORTC &= (~(1<<0));

                            if ((PINC & (1<<2)) == 0)
                            {
                                m++;
                                _delay_ms(50);
                            }

                        }

                        if(temp == 3)
                        {

                            SPI_WriteByte(~0b00000001);
                            SPI_WriteByte(~digit[i]);

                            PORTC |= 1 << 0;
                            _delay_ms(1);
                            PORTC &= (~(1<<0));


                            if ((PINC & (1<<2)) == 0)
                            {
                                i++;
                                _delay_ms(50);
                            }

                        }




                        if ((PINC & (1<<1)) == 0) temp++;
                        _delay_ms(50);

                        if(temp > 3)
                        {

                            _delay_ms(100);
                            seconds = 0;
                            sei();
                            checkTime();
                            break;
                        }
                    }

                    // второй бит переменной tmp
                }


        //PORTC = ~PORTC;
        //SPI_WriteByte(~digit);
        //byte ++;
        //_delay_ms(100);
        //TIMSK |= (1 << TOIE2); // Разрешаем прерывание по переполнению Т2
    }
}

/*инициализация SPI модуля в режиме master*/
void SPI_Init(void)
{

    /*настройка портов ввода-вывода
   все выводы, кроме MISO выходы*/
    SPI_DDRX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(0<<SPI_MISO);
    SPI_PORTX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);

    /*разрешение spi,старший бит вперед,мастер, режим 0*/
    SPCR = (1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR1)|(0<<SPR0);
    SPSR = (0<<SPI2X);


//    DDRB |= ((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //ножки SPI на выход

//      PORTB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5)); //низкий уровень

//      SPCR = ((1<<SPE)|(1<<MSTR));//включим шину, объявим ведущим
}

// отправка байта
void SPI_WriteByte(uint8_t data)
{
    SPI_PORTX &= ~(1<<SPI_SS);
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    SPI_PORTX |= (1<<SPI_SS);
}

//Прерывание по переполнению Т2
ISR(TIMER2_OVF_vect)
{


    //    i++;
    //    if(i==10)
    //    {
    //        m++;
    //        i = 0;
    //    }

    Tick();



    //m += 2;
    //n += 3;
    //t += 4;


    DDRD = 0xFF;
    PORTD ^= 1 << 0;


    //PORTC = 0xFF;
    //_delay_ms(500);
    // Выводим данные на дисплей
}
void Timer2_Init()
{

    TIMSK &= ~(1 << OCIE2)|(1 << TOIE2); // Запрещаем прерывания по T2
    ASSR |= (1 << AS2); // Включаем асинхронный режим T2
    TCNT2 = 0; // Сбрасываем регистр счета
    TCCR2 |= (1 << CS22)|(1 << CS20); // Предделитель на 128(32768/128 = 256 тиков/c)

    sei(); // Глобально разрешаем прерывания

    TIMSK |= (1 << TOIE2); // Разрешаем прерывание по переполнению Т2
}

void Tick()
{

    seconds ++;

    if(seconds > 59)
    {
        i ++ ;
        seconds = 0;
        if(i > 9)
        {
            m++;
            i=0;

        }
        if (m > 5 ) {
            n++;
            m = 0;
            i = 0;
        }


        if (t < 2 && n > 9)
        {
            t++;
            n = 0;
        }




        if(t == 2 && n > 3)
        {

            i = 0;
            m = 0;
            n = 0;
            t = 0;
        }
    }
}

void flash()
{
    SPI_WriteByte(~0b0000001);
    SPI_WriteByte(digit[t]);

        PORTC |= 1 << 0;
       //   _delay_ms(1);
       PORTC &= (~(1<<0));

       _delay_ms(1);
    SPI_WriteByte(~0b00000010);
     SPI_WriteByte(digit[n]);

     PORTC |= 1 << 0;
     //  _delay_ms(1);
    PORTC &= (~(1<<0));
_delay_ms(1);
    SPI_WriteByte(~0b00000100);
    SPI_WriteByte(digit[m]);

    PORTC |= 1 << 0;
    //  _delay_ms(1);
   PORTC &= (~(1<<0));

_delay_ms(1);
    SPI_WriteByte(~0b00001000);
    SPI_WriteByte(digit[i]);

    PORTC |= 1 << 0;
    //  _delay_ms(1);

   PORTC &= (~(1<<0));
_delay_ms(1);
}

void checkTime()
{


    if(i > 9)
    {
        i = 0;
        m = 0;
        n = 0;
        t = 0;

    }
    if (m > 5 ) {
        i = 0;
        m = 0;
        n = 0;
        t = 0;
    }


    if (t < 2 && n > 9)
    {
        i = 0;
        m = 0;
        n = 0;
        t = 0;
    }




    if(t == 2 && n > 3)
    {

        i = 0;
        m = 0;
        n = 0;
        t = 0;
    }

    if(t > 2)
    {

        i = 0;
        m = 0;
        n = 0;
        t = 0;
    }

}
