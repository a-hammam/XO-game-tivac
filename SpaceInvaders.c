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
void EdgeCounter_Init(void){                          
                        
  SYSCTL_RCGC2_R |= 0x00000020;  //activate clock for port F
  delay = 0;
  GPIO_PORTF_LOCK_R=0x4C4F434B;  //Unlock portF
  GPIO_PORTF_CR_R=0x1F;      			//allow changes to PF0-4      
  GPIO_PORTF_DIR_R &= ~0x0E;     //PF4-0 INPUT, PF1-2-3 OUTPUT
  GPIO_PORTF_AFSEL_R &= ~0x11;   //disable alt funct on PF4-0
  GPIO_PORTF_DEN_R |= 0x11;      //enable digital I/O on PF4-0
  GPIO_PORTF_PCTL_R &= ~0x00000000; //GPIO clear bit
  GPIO_PORTF_AMSEL_R = 0;    			//Disable analog functions 
  GPIO_PORTF_PUR_R |= 0x11;       //enable pull-up resistors on PF0-4
  GPIO_PORTF_IS_R &= ~0x11;   	  //PF4-0 is edge sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    	//PF0-4 is single edge
  GPIO_PORTF_IEV_R &= ~0x11;      //PF4-0 is raising Edges
  GPIO_PORTF_ICR_R = 0x11;      	//clear flags 0 and 4
  GPIO_PORTF_IM_R |= 0x11;     	  //arm interrupt on PF0 and PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; //priority 5
  NVIC_EN0_R = 0x40000000;     		//EnableInterrupts interrupt 30 on NVIC
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
void portF_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |=0x00000020;	// activate clock for port F
	delay=SYSCTL_RCGC2_R;					//delay
	GPIO_PORTF_LOCK_R=0x4C4F434B;	//Unloock portF
	GPIO_PORTF_CR_R=0x1F;					//Allow changes to PF0-4
	GPIO_PORTF_AMSEL_R=0x00;			//disable analog function
	GPIO_PORTF_PCTL_R=0x00000000;	//GPIO clear bit PCTL
	GPIO_PORTF_DIR_R=0x0E;				//PF4,PF0 input, PF3,PF2,PF1 output 			
	GPIO_PORTF_AFSEL_R=0x00;			//no alternate function
	GPIO_PORTF_PUR_R=0x11;				//enable pullup resistors on PF4,PF0
	GPIO_PORTF_DEN_R=0x1F;				//enable digital pins PF4-PF0 
}
void GPIOPortF_Handler(void)
	{
	if (GPIO_PORTF_RIS_R &(1<<4))  
	{   
			GPIO_PORTF_ICR_R = 0x10;
		cursor--;
		while(array[cursor]!=' '){
			
			cursor--;
		if(cursor<1) cursor=9;
		}
		draw();
		
		
	}
	 if(GPIO_PORTF_RIS_R &(1<<0)){
		 GPIO_PORTF_ICR_R = 0x01;
		cursor++;
		while(array[cursor]!=' '){
			
			cursor++;
		if(cursor>9) cursor=1;
		}
		draw();
		
		
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

void reset(void){
	int i;
	array[0]='a';
	for(i=1;i<10;i++){
		array[i]=' ';
		
	}
	turn='X';
	moves=0;
	cursor=1;
	GPIO_PORTE_DATA_R &=~ (1<<0);
	draw();

}

void theWinnerIs(void){//check if there is a winner
	
	
	
	

	/*
		to win you should have x or o in
	0,1,2 or 3,4,5 or 6,7,8 or
	0,3,6 or 1,4,7 or 2,5,8 or
	0,4,8 or 2,4,6
	*/

	//check if X win`
	if((array[1] == x_id && array[2] == x_id && array[3] == x_id) ||
		 (array[4] == x_id && array[5] == x_id && array[6] == x_id) ||
		 (array[7] == x_id && array[8] == x_id && array[9] == x_id) ||

		 (array[1] == x_id && array[4] == x_id && array[7] == x_id) ||
		 (array[2] == x_id && array[5] == x_id && array[8] == x_id) ||
		 (array[3] == x_id && array[6] == x_id && array[9] == x_id) ||

	   (array[1] == x_id && array[5] == x_id && array[9] == x_id) ||
		 (array[3] == x_id && array[5] == x_id && array[7] == x_id) 		){
		 
			//Delay();
			 Timer2_Init(10000000);
			 WaitForInterrupt();
			 Nokia5110_ClearBuffer();
			 Nokia5110_DisplayBuffer();
			 Nokia5110_SetCursor(0,1);
		   Nokia5110_OutString("winner is X");
			 x_score++;
			 Nokia5110_SetCursor(0,2);
	Nokia5110_OutString("X:");
	Nokia5110_SetCursor(1,2);
	Nokia5110_OutUDec(x_score);
	 
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("O:");
	Nokia5110_SetCursor(1,3);
	Nokia5110_OutUDec(o_score);
			 
			 GPIO_PORTE_DATA_R |=(1<<0);
			 //Delay();
			  Timer2_Init(80000000);
			 WaitForInterrupt();
			 reset();
		 }
	//check if O win`
	else if((array[1] == o_id && array[2] == o_id && array[3] == o_id) ||
		 (array[4] == o_id && array[5] == o_id && array[6] == o_id) ||
		 (array[7] == o_id && array[8] == o_id && array[9] == o_id) ||

		 (array[1] == o_id && array[4] == o_id && array[7] == o_id) ||
		 (array[2] == o_id && array[5] == o_id && array[8] == o_id) ||
		 (array[3] == o_id && array[6] == o_id && array[9] == o_id) ||

	   (array[1] == o_id && array[5] == o_id && array[9] == o_id) ||
		 (array[3] == o_id && array[5] == o_id && array[7] == o_id) 		)
{
		 
		//Delay();
	 Timer2_Init(10000000);
			 WaitForInterrupt();
			  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
			 Nokia5110_SetCursor(0,1);
			Nokia5110_OutString("winner is O");
	o_score++;
	Nokia5110_SetCursor(0,2);
Nokia5110_OutString("X:");
	Nokia5110_SetCursor(1,2);
	Nokia5110_OutUDec(x_score);
	 
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("O:");
	Nokia5110_SetCursor(1,3);
	Nokia5110_OutUDec(o_score);
	
	 GPIO_PORTE_DATA_R |=(1<<0);
	
	//Delay();
	 Timer2_Init(80000000);
	 WaitForInterrupt();
			 reset();
}
else if(moves==9){
 Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
			 Nokia5110_SetCursor(3,1);
			Nokia5110_OutString("DRAW");
	Nokia5110_SetCursor(0,2);
Nokia5110_OutString("X:");
	Nokia5110_SetCursor(1,2);
	Nokia5110_OutUDec(x_score);
	 
	Nokia5110_SetCursor(0,3);
	Nokia5110_OutString("O:");
	Nokia5110_SetCursor(1,3);
	Nokia5110_OutUDec(o_score);
	GPIO_PORTE_DATA_R |=(1<<0);
	//Delay();
	Timer2_Init(80000000);
	 WaitForInterrupt();
	reset();
}
}


int main(void){
 
  TExaS_Init(NoLCD_NoScope);  // set system clock to 80 MHz
  Random_Init(1);
 	PortE_Init(); 
	portF_Init();
	EdgeCounter_Init();
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

void Timer2_Init(unsigned long period){ 
  unsigned long volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}
void Timer2A_Handler(void){ 
	
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  
}
