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
		
	 Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	current_cursor=1,indx=1;
	// Horizontal borders
for(i = 0 ;i < 8 ;i++){
		Nokia5110_SetCursor(i, 1);
		Nokia5110_OutString("-");
		Nokia5110_SetCursor(i, 3);
		Nokia5110_OutString("-");
	}

// Vertical borders
for(j = 0 ;j < 5 ;j+=2){
	Nokia5110_SetCursor(2, j);
	Nokia5110_OutString("|");
	Nokia5110_SetCursor(5, j);
	Nokia5110_OutString("|");	
	}

// arrayay values
 for(i = 0 ;i < 5;i+=2) {
	for(j = 0 ;j < 8 ;j+=3){
			Nokia5110_SetCursor(j, i);
			Nokia5110_OutChar(array[indx++]);
	}
}
 
// Displaying cursor
 for(i = 0 ;i < 5;i+=2) {
	for(j = 1 ;j < 8 ;j+=3){
		if(current_cursor == cursor){
			Nokia5110_SetCursor(j, i);
			Nokia5110_OutChar('.');
		}
		current_cursor++;
		}
}

// Display Trun
	Nokia5110_SetCursor(8,0);
	Nokia5110_OutString("Turn");
	Nokia5110_SetCursor(9,1);
	Nokia5110_OutChar(turn);

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


