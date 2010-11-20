#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"hardware.h"
#include"timer.h"

void reset_all(void);

unsigned char counter_sec = 0, counter_min = 0,counter_hour = 0,count_enable = 1;

void t2_cb(unsigned char mode)
{
	static int prev_state;

	if(PIND&1 && count_enable == 1)
	{
		if(prev_state == 0)
		{
			reset_all();
		}
		if(counter_sec<99)
		{
			 counter_sec++;
			CLK_SEC_PORT|=(1<<CLK_SEC_PIN);
			_delay_us(100);
			CLK_SEC_PORT&=~(1<<CLK_SEC_PIN);
		}

		else
		{
			counter_sec = 0;
			RST_SEC_PORT &=~(1<<RST_SEC_PIN);



			if(counter_min<59)
			{
				counter_min++;
				CLK_MIN_PORT|=(1<<CLK_MIN_PIN);
				_delay_us(100);
			
				CLK_MIN_PORT&=~(1<<CLK_MIN_PIN);
			}
			else
			{
				counter_min = 0;
				counter_hour++;
			
				RST_MIN_PORT &= ~(1<<RST_MIN_PIN);
				CLK_HOUR_PORT |= (1<<CLK_HOUR_PIN);
				_delay_us(100);
				CLK_HOUR_PORT &= ~(1<<CLK_HOUR_PIN);
				RST_MIN_PORT |=  (1<<RST_MIN_PIN);
			}

			RST_SEC_PORT|=(1<<RST_SEC_PIN);

		}
	}

	prev_state = PIND&1;



}

int main(void)
{
	//Init IO Pins
	DDRB = 0b11111000;
	DDRC = 0b00011100;
	DDRD = 0b11110000;
	PORTD|=1;//Enable Line pull up
	PORTA|=0b11;


	//DISABLE LAMP TEST
	LAMP_TEST_PORT|=(1<<LAMP_TEST_PIN);

	//Reset all digits
	RST_MIN_PORT |=(1<<RST_MIN_PIN);
	RST_SEC_PORT |=(1<<RST_SEC_PIN);
	RST_HOUR_PORT|=(1<<RST_HOUR_PIN);
	_delay_us(100);
	RST_MIN_PORT &=~(1<<RST_MIN_PIN);
	RST_SEC_PORT &=~(1<<RST_SEC_PIN);
	RST_HOUR_PORT&=~(1<<RST_HOUR_PIN);
	_delay_us(100);
	RST_MIN_PORT |=(1<<RST_MIN_PIN);
	RST_SEC_PORT |=(1<<RST_SEC_PIN);
	RST_HOUR_PORT|=(1<<RST_HOUR_PIN);


	/*while(1)
	{
		CLK_SEC_PORT|=(1<<CLK_SEC_PIN);
		CLK_MIN_PORT|=(1<<CLK_MIN_PIN);
		CLK_HOUR_PORT|=(1<<CLK_HOUR_PIN);
		_delay_ms(100);
		CLK_SEC_PORT&=~(1<<CLK_SEC_PIN);
		CLK_MIN_PORT&=~(1<<CLK_MIN_PIN);
		CLK_HOUR_PORT&=~(1<<CLK_HOUR_PIN);
		_delay_ms(100);

	}*/




	//Init timer2
	init_timer2(PR_8,EXTERNAL,40);//0,01s
	enable_timer_2_interrupt(CTC,t2_cb);

	


	while(1)
	{

		if(!((BUTTON_PORT&(1<<BUTTON_PIN))))
		{
			reset_all();
			
		}


		if(((PINA&1) == 0) || (PINA&2) == 0)
		{
			count_enable = 0;
		}

		if(count_enable == 0 && (PIND&1) == 0)
		{
			count_enable = 1;
		}
	}
	
	return 0;
}

void reset_all(void)
{
			//Reset all digits
			RST_MIN_PORT |=(1<<RST_MIN_PIN);
			RST_SEC_PORT |=(1<<RST_SEC_PIN);
			RST_HOUR_PORT|=(1<<RST_HOUR_PIN);
			_delay_us(100);
			RST_MIN_PORT &=~(1<<RST_MIN_PIN);
			RST_SEC_PORT &=~(1<<RST_SEC_PIN);
			RST_HOUR_PORT&=~(1<<RST_HOUR_PIN);
			_delay_us(100);
			RST_MIN_PORT |=(1<<RST_MIN_PIN);
			RST_SEC_PORT |=(1<<RST_SEC_PIN);
			RST_HOUR_PORT|=(1<<RST_HOUR_PIN);
}
