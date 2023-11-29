#include "main.h"

#define DMA_Stream DMA2_Stream5

#define SIZE 10 // Размер буфера для передачи данных


//uint16_t buf[SIZE] = {0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020,0x0020, 0x0020,0x0000,  0x0000, 0x0020,0x0000 ,0x0000 , 0x0020 , 0x0000 , 0x0000 , 0x0000  ,0x0020};
uint16_t buf[SIZE] = {0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020, 0x0000, 0x0020};
// Буфер
//char buf[SIZE] = "Hello World";




void GPIO_Init(){

		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //RCC on for GPIO A
		GPIOA->MODER &= ~0x00000C00; /* clear pin mode */
		GPIOA->MODER |= GPIO_MODER_MODER5_0; /* set pin to output mode */
		GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1 ); // very high
		GPIOA->BSRR = GPIO_BSRR_BS5; // установили в высокое состояние


		}


void TIM1_UP_TIM10_IRQHandler(void){
	if(READ_BIT(TIM2->SR, TIM_SR_UIF)){


		TIM2->SR &= ~TIM_SR_UIF;

	}

}


void TIM1_Init(void)
{
    /* Enable clock for TIM2 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* Initialize TIM2 */
    TIM1->PSC = 0; // Prescaler
    TIM1->ARR = 1666 - 1; // Auto-reload value

    /* Force update generation to update registers */
    TIM1->EGR |= TIM_EGR_UG;

    /* Clear update interrupt flag */
   TIM1->SR &= ~TIM_SR_UIF;

   TIM1->DIER |= TIM_DIER_TDE;

    TIM1->DIER |= TIM_DIER_UIE;

  // TIM1->DIER |= TIM_DIER_UDE;// Update dma // Включаем запрос DMA по обновлению таймера

    /* Enable TIM2 global interrupt */

   NVIC_EnableIRQ(TIM2_IRQn);


}


/*void DMA1_Stream7_IRQHandler(void){


	if (READ_BIT(DMA1->LISR, DMA_LISR_HTIF2)){

	}

	if ((READ_BIT(DMA1->LISR, DMA_LISR_TCIF2))){ // передача по 2 каналу завершена полностью


		}

	}*/




void DMA_Init(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA_Stream->CR = (DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_2)  ; // 6 канал 110
	DMA_Stream->CR |= DMA_SxCR_MSIZE_0;// MSIZE 16 bit
	DMA_Stream->CR |= DMA_SxCR_PSIZE_0; // (0x0<<11); // PSIZE 16 bit
	DMA_Stream->CR |= DMA_SxCR_MINC; // (0x1<<10); // увеличиваем память

	DMA_Stream->CR |= DMA_SxCR_DIR_0;  //  сбрасываю 7 бит 01 Из памяти в периф



	// DMA_Stream->CR |= DMA_SxCR_PL_1; // Приоритет потока: высокий
	DMA_Stream->CR |= DMA_SxCR_CIRC ;//(0x1<<8); Circular mode



}

void DMA_Config(uint32_t perih_address, uint32_t mem_address , uint16_t data_amount ){

	DMA_Stream->NDTR = data_amount;

	DMA_Stream->PAR = perih_address;

	DMA_Stream->M0AR = mem_address;

	DMA_Stream->CR |= DMA_SxCR_EN;


}

int main(void)
{

	GPIO_Init();
	DMA_Init();
	DMA_Config( (uint32_t) &GPIOA->ODR , (uint32_t) buf, SIZE);
	TIM1_Init();
	asm("NOP");
	TIM1->DIER |= TIM_DIER_UDE;// Update dma // Включаем запрос DMA по обновлению таймера
	TIM1->CR1 |= TIM_CR1_CEN; // включаем таймер TIM2
	NVIC_EnableIRQ(DMA1_Stream7_IRQn);
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");


	while(1){


		}
}





