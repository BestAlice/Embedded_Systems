#include <hal.h>

struct status{
	GPIO_PinState state[2];
}; 

struct frame{
	int state[8];
};

int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
				 GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
				 GPIO_PIN_11, GPIO_PIN_12};			 

unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
struct status pin_cond = {GPIO_PIN_RESET, GPIO_PIN_SET};

int frame=0;
int standart_delay = 500;
int pin_state = 0;
int T = 100;
int full_delay = standart_delay;

struct frame animations[8] = {
	{1,0,0,0,0,0,0,1},
	{1,1,0,0,0,0,1,1},
	{1,1,1,0,0,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,0,0,1,1,1},
	{1,1,0,0,0,0,1,1},
	{1,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,0,0},};
	
void TIM6_IRQ_Handler(){
	for(int i=0; i<8;i++){
		HAL_GPIO_WritePin(GPIOD, leds_num[i], pin_cond.state[animations[frame].state[i]]);
	}
	frame = (frame+1)%8;
}


int umain(){
	registerTIM6_IRQHandler(TIM6_IRQ_Handler);

	
	__enable_irq();
	
	WRITE_REG(TIM6_ARR, full_delay);
	WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
	WRITE_REG(TIM6_PSC, 0);
	WRITE_REG(TIM6_CR1, TIM_CR1_CEN);

	
	
	while(true){
		GPIO_PinState states[4];
		for(int i = 0; i < 4; i++)
        {
            states[i] = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
        }
		pin_state = 0;
		for(int i=0; i<4;i++){
			if (states[i] == GPIO_PIN_SET){
				pin_state += 4-i;
			}
		}
		full_delay = standart_delay + T * pin_state;
		WRITE_REG(TIM6_ARR, full_delay);
	}
	return 0;
}