#include <hal.h>


struct status{
	GPIO_PinState state[2];
}; 

struct frame{
	int state[8];
};

int umain(){
	int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
					 GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
					 GPIO_PIN_11, GPIO_PIN_12};
	//HAL_GPIO_WritePin(GPIOD, led_num[0], GPIO_PIN_SET);				 
	
	unsigned int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
	struct status pin_stat = {GPIO_PIN_RESET, GPIO_PIN_SET};
	
	int count=0;
	int delay = 500;
	bool play = true;
	struct frame animations[14] = {
		{1,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,1,0,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{1,1,1,1,1,0,0,0},
		{1,1,1,1,1,1,0,0},
		{1,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,0},
		{1,1,1,1,1,1,0,0},
		{1,1,1,1,1,0,0,0},
		{1,1,1,1,0,0,0,0},
		{1,1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0,0}};
		
	while(true){
		GPIO_PinState states[4];
		GPIO_PinState state_button = GPIO_PIN_SET;
		state_button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
		if(state_button==GPIO_PIN_RESET){
			play = !play;
		}
		for(int i = 0; i < 4; i++)
        {
            states[i] = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
            
        }
		if((states[0]||states[2])==GPIO_PIN_RESET && (states[1]&&states[3])==GPIO_PIN_SET){
			for(int i=0; i<8;i++){
				HAL_GPIO_WritePin(GPIOD, leds_num[i], pin_stat.state[animations[count].state[i]]);
			}
			if(play){count++;
			    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

			if(count>13){
				count=0;
			}} else {
				    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

			}
		}
		else{
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

			for(int i = 0; i < 4; i++)
        {
            HAL_GPIO_WritePin(GPIOD, leds_num[i], states[i]);
        }
			for(int i = 4; i < 8; i++){
				HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
			}
		}
		HAL_Delay(delay);
	}
	return 0;
}