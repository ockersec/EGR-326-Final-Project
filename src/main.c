#include "buzzer.h"

int main(void){
	BUZZER_INIT();
	update_buzzer_freq(100);
	while(1){
	}
	return 0;
}