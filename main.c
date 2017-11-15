//use MSS hardware timer1 @100MHz and GPIO mapped to LEDS

#include "timer.h"
#include "mss_gpio.h"
//#include "mss_timer.h"

//simple led toggle timer function
void led0() { MSS_GPIO_set_outputs(MSS_GPIO_get_outputs( )^ MSS_GPIO_0_MASK);}
void led1() { MSS_GPIO_set_outputs(MSS_GPIO_get_outputs( )^ MSS_GPIO_1_MASK);}
void led2() { MSS_GPIO_set_outputs(MSS_GPIO_get_outputs( )^ MSS_GPIO_2_MASK);}
void led3() { MSS_GPIO_set_outputs(MSS_GPIO_get_outputs( )^ MSS_GPIO_3_MASK);}
void led4() { MSS_GPIO_set_outputs(MSS_GPIO_get_outputs( )^ MSS_GPIO_4_MASK);}

void init_DAC () {
	ACE_init();

	/* DAC initialization */
	ACE_configure_sdd(
		SDD1_OUT,
		SDD_16_BITS,
		SDD_VOLTAGE_MODE | SDD_RETURN_TO_ZERO,
		INDIVIDUAL_UPDATE
	);
	ACE_enable_sdd(SDD1_OUT);
}

void playwav () {
	int num;
	scanf("%d", &num);
	printf("%d\r\n", num);
	ACE_set_sdd_value(SDD1_OUT, (uint32_t)(num/65536));
}

int main(){

	MSS_GPIO_init();
	MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config( MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config( MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config( MSS_GPIO_4, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_set_outputs(0x0);

	startTimerOneshot(&led0, 100000000);
	startTimerContinuous(&led1, 10000000);
	startTimerContinuous(&led2, 10000000);
	startTimerContinuous(&led3, 25000000);
	startTimerContinuous(&led4, 100000000);
	while(1);
}

