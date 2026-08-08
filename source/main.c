/*
 * ============================================================
 * UNDERGROUND CABLE FAULT DETECTOR
 * ============================================================
 *
 * Microcontroller : ATmega16
 * ADC             : 10-bit ADC
 * Display         : 16x2 LCD
 * Programming     : Embedded C
 *
 * Description:
 * Detects short-circuit-to-ground faults in a simulated
 * underground three-phase cable and estimates the distance
 * of the fault from the feeder end.
 *
 * The three resistor networks represent:
 *      Phase R -> ADC0
 *      Phase Y -> ADC1
 *      Phase B -> ADC2
 *
 * NOTE:
 * This is a reconstructed/reference implementation based
 * on the project documentation. The original source code
 * was not available in the project report.
 * ============================================================
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

/* ============================================================
 * LCD CONNECTION
 * ============================================================
 *
 * LCD Data Pins:
 *      D4 -> PC4
 *      D5 -> PC5
 *      D6 -> PC6
 *      D7 -> PC7
 *
 * Control:
 *      RS -> PD0
 *      EN -> PD1
 *
 * RW is connected to GND.
 * ============================================================
 */

#define LCD_PORT PORTC
#define LCD_DDR  DDRC

#define LCD_RS   PD0
#define LCD_EN   PD1

#define LCD_CTRL_PORT PORTD
#define LCD_CTRL_DDR  DDRD


/* ============================================================
 * CABLE PARAMETERS
 * ============================================================
 *
 * The physical prototype uses resistors to represent sections
 * of underground cable.
 *
 * Adjust these values according to the actual resistor network
 * used in the prototype.
 * ============================================================
 */

#define CABLE_LENGTH_KM       3.0
#define CABLE_RESISTANCE_KM   1000.0


/* ============================================================
 * LCD FUNCTIONS
 * ============================================================
 */

void LCD_Enable(void)
{
    LCD_CTRL_PORT |= (1 << LCD_EN);
    _delay_us(1);

    LCD_CTRL_PORT &= ~(1 << LCD_EN);
    _delay_us(100);
}


void LCD_Send4Bits(unsigned char data)
{
    LCD_PORT &= 0x0F;

    LCD_PORT |= (data & 0xF0);

    LCD_Enable();
}


void LCD_Command(unsigned char command)
{
    LCD_CTRL_PORT &= ~(1 << LCD_RS);

    LCD_PORT = (LCD_PORT & 0x0F) | (command & 0xF0);
    LCD_Enable();

    LCD_PORT = (LCD_PORT & 0x0F) | ((command << 4) & 0xF0);
    LCD_Enable();

    _delay_ms(2);
}


void LCD_Char(unsigned char data)
{
    LCD_CTRL_PORT |= (1 << LCD_RS);

    LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
    LCD_Enable();

    LCD_PORT = (LCD_PORT & 0x0F) | ((data << 4) & 0xF0);
    LCD_Enable();

    _delay_us(100);
}


void LCD_String(const char *str)
{
    while (*str)
    {
        LCD_Char(*str++);
    }
}


void LCD_Clear(void)
{
    LCD_Command(0x01);
    _delay_ms(2);
}


void LCD_GotoXY(unsigned char row, unsigned char column)
{
    unsigned char address;

    if (row == 0)
        address = 0x80 + column;
    else
        address = 0xC0 + column;

    LCD_Command(address);
}


void LCD_Init(void)
{
    LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_EN);

    LCD_DDR |= 0xF0;

    _delay_ms(20);

    LCD_CTRL_PORT &= ~(1 << LCD_RS);

    LCD_PORT = (LCD_PORT & 0x0F) | 0x30;
    LCD_Enable();
    _delay_ms(5);

    LCD_PORT = (LCD_PORT & 0x0F) | 0x30;
    LCD_Enable();
    _delay_us(150);

    LCD_PORT = (LCD_PORT & 0x0F) | 0x20;
    LCD_Enable();

    LCD_Command(0x28);   // 4-bit, 2-line, 5x8 font
    LCD_Command(0x0C);   // Display ON, cursor OFF
    LCD_Command(0x06);   // Auto increment cursor
    LCD_Command(0x01);   // Clear display

    _delay_ms(2);
}


/* ============================================================
 * ADC FUNCTIONS
 * ============================================================
 */

/*
 * Initialize ADC.
 *
 * AVCC is used as the reference voltage.
 * ADC clock = F_CPU / 64
 */

void ADC_Init(void)
{
    ADMUX = (1 << REFS0);

    ADCSRA =
        (1 << ADEN)  |
        (1 << ADPS2) |
        (1 << ADPS1);
}


/*
 * Read ADC channel.
 *
 * channel:
 *      0 -> ADC0
 *      1 -> ADC1
 *      2 -> ADC2
 */

unsigned int ADC_Read(unsigned char channel)
{
    unsigned int value;

    channel &= 0x07;

    ADMUX = (1 << REFS0) | channel;

    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC));

    value = ADC;

    return value;
}


/*
 * Take multiple ADC samples and calculate average.
 * This reduces small fluctuations in the ADC reading.
 */

unsigned int ADC_ReadAverage(unsigned char channel)
{
    unsigned long sum = 0;
    unsigned char i;

    for (i = 0; i < 10; i++)
    {
        sum += ADC_Read(channel);
        _delay_ms(2);
    }

    return (unsigned int)(sum / 10);
}


/* ============================================================
 * FAULT DISTANCE CALCULATION
 * ============================================================
 *
 * The project works using the resistance/voltage relationship
 * described in the project report.
 *
 * A fault changes the resistance seen by the feeder.
 * The corresponding voltage is measured by the ADC.
 *
 * For this reconstructed implementation, the ADC value is
 * mapped to the equivalent cable distance.
 *
 * CALIBRATION:
 * The actual resistor values in the prototype must be used
 * to obtain an accurate physical distance.
 * ============================================================
 */

float Calculate_Distance(unsigned int adc_value)
{
    float voltage;
    float distance;

    /*
     * ATmega16 ADC:
     * 10-bit resolution -> 0 to 1023
     *
     * Assuming 5V reference.
     */

    voltage = ((float)adc_value * 5.0) / 1023.0;

    /*
     * Convert measured voltage to equivalent distance.
     *
     * This relationship depends on the actual resistor
     * network and must be calibrated experimentally.
     *
     * For the reference implementation:
     * voltage is normalized against 5V.
     */

    distance = (voltage / 5.0) * CABLE_LENGTH_KM;

    return distance;
}


/* ============================================================
 * DISPLAY FAULT INFORMATION
 * ============================================================
 */

void Display_Fault(char phase, float distance)
{
    char buffer[17];

    LCD_Clear();

    LCD_GotoXY(0, 0);

    if (phase == 'R')
        LCD_String("FAULT: PHASE R");

    else if (phase == 'Y')
        LCD_String("FAULT: PHASE Y");

    else
        LCD_String("FAULT: PHASE B");

    LCD_GotoXY(1, 0);

    sprintf(buffer, "DIST: %.2f KM", distance);

    LCD_String(buffer);
}


/* ============================================================
 * DISPLAY NORMAL CONDITION
 * ============================================================
 */

void Display_Normal(void)
{
    LCD_Clear();

    LCD_GotoXY(0, 0);
    LCD_String("CABLE STATUS:");

    LCD_GotoXY(1, 0);
    LCD_String("NO FAULT");
}


/* ============================================================
 * MAIN PROGRAM
 * ============================================================
 */

int main(void)
{
    unsigned int adc_R;
    unsigned int adc_Y;
    unsigned int adc_B;

    float distance_R;
    float distance_Y;
    float distance_B;

    /*
     * ADC pins:
     * PA0 -> Phase R
     * PA1 -> Phase Y
     * PA2 -> Phase B
     */

    DDRA &= ~((1 << PA0) |
              (1 << PA1) |
              (1 << PA2));

    /*
     * Disable digital input buffers on ADC pins.
     */

    DIDR = (1 << ADC0D) |
           (1 << ADC1D) |
           (1 << ADC2D);


    /*
     * Initialize peripherals.
     */

    LCD_Init();
    ADC_Init();


    /*
     * Startup message.
     */

    LCD_Clear();

    LCD_GotoXY(0, 0);
    LCD_String("UNDERGROUND");

    LCD_GotoXY(1, 0);
    LCD_String("CABLE DETECTOR");

    _delay_ms(2000);


    while (1)
    {
        /*
         * Read the three cable phases.
         */

        adc_R = ADC_ReadAverage(0);
        adc_Y = ADC_ReadAverage(1);
        adc_B = ADC_ReadAverage(2);


        /*
         * Convert ADC values into estimated
         * fault distances.
         */

        distance_R = Calculate_Distance(adc_R);
        distance_Y = Calculate_Distance(adc_Y);
        distance_B = Calculate_Distance(adc_B);


        /*
         * Fault detection.
         *
         * A practical implementation should use calibrated
         * threshold values obtained from the actual resistor
         * network.
         */

        if (adc_R < 900)
        {
            Display_Fault('R', distance_R);
        }

        else if (adc_Y < 900)
        {
            Display_Fault('Y', distance_Y);
        }

        else if (adc_B < 900)
        {
            Display_Fault('B', distance_B);
        }

        else
        {
            Display_Normal();
        }


        _delay_ms(500);
    }

    return 0;
}
