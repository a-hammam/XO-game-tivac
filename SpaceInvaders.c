#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);
void PortE_Init(void);        
void PortF_Init(void);
void EdgeCounter_Init(void); 
void draw(void);
void reset(void);
void theWinnerIs(void);
void Delay(void);
int moves=0;
int delay;
void Delay(void);
volatile int cursor=1;
char array[10]={'a',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char turn = 'X';



void PortE_Init(void){
	SYSCTL_RCGC2_R  |= (1<<4);        //port E clock
  delay = 0;                        // delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE
  GPIO_PORTE_CR_R = (1<<0)|(1<<1);           // allow changes to PE0,PE1    
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x1;          // 5) PE0 OUTPUT PE1 INPUT 
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_DEN_R = 0x1F;        // enable pullup resistors on PE1  
  GPIO_PORTE_DEN_R =0x1F;        // 7) enable digital pins PE0,PE1     
 GPIO_PORTE_IS_R &= ~(1<<1);             //EDGE SENSE
	GPIO_PORTE_IBE_R &= ~(1<<1);            //SINGLE EDGE
	GPIO_PORTE_IEV_R |= (1<<1);              //RAISING EDGE
	GPIO_PORTE_ICR_R = (1<<1);     // CLEAR FLAGS ON PE1
	GPIO_PORTE_IM_R |= (1<<1);			// ARM INTERRUPT PE1
	NVIC_PRI1_R =(5<<5);             //PRIORITY 5
NVIC_EN0_R =(1<<4);          //ENABLE INTERRUPT 4
EnableInterrupts();
	 
}

void GPIOPortE_Handler (void){
	if(GPIO_PORTE_RIS_R &(1<<1)){
		GPIO_PORTE_ICR_R =(1<<1); 
		if(array[cursor]==' '){
			if (turn =='X'){
				array[cursor]='X';
				turn='O';
			}
			else{ 
			array[cursor]='O';
		  turn='X';
		
			}
			draw();
			moves++;
		
		  theWinnerIs();
		
		}
	}

}

void draw(void){
}

void theWinnerIs(void){
}

int main(void){
 
  TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  Random_Init(1);
 	PortE_Init(); 

EnableInterrupts();
  while(1){
		WaitForInterrupt();
	}

}


void Delay(void){unsigned long volatile time;
  time = 727240*200/91;  // 0.1sec
  while(time){
		time--;
  }
}


