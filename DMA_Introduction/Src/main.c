#include "main.h"
#include "string.h"

#define SIZE 8

uint8_t buf[SIZE];

uint8_t recieve_buf[2*SIZE];

uint8_t i = 0;




/*void *memcpy(void *dest, const void *src, size_t n)
{
    // Приводим указатели к типу char *, так как char занимает 1 байт
    char *d = (char *)dest;
    const char *s = (const char *)src;
    // Копируем n байтов из src в dest по одному
    while (n--)
    {
        *d++ = *s++;
    }
    // Возвращаем указатель на dest
    return dest;
}

*/





void USART1_Init(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Включаем тактирование порта A
	GPIOA->MODER |= GPIO_MODER_MODER5_0; /* set pin to output mode */

	GPIOA->MODER |= GPIO_MODER_MODER10_1;
	GPIOA->AFR[1] |= (GPIO_AFRH_AFSEL10_2 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_0); // Выбираем альтернативную функцию 7 (USART1) для пина PA10

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Включаем тактирование USART1
	USART1->BRR = 0x683; // Устанавливаем скорость передачи 9600 бод при частоте 16 МГц
	USART1->CR1 |= (USART_CR1_RE | USART_CR1_RXNEIE);
	USART1->CR3 |= (USART_CR3_DMAR);
	USART1->CR1 |= USART_CR1_UE; // Включаем USART1
}

void DMA_Init(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream2->CR = DMA_SxCR_CHSEL_2  ; // (0x4<<25) 4 канал
	DMA2_Stream2->CR &= ~DMA_SxCR_MSIZE;// (0x0<<13); MSIZE 8 bit
	DMA2_Stream2->CR &= ~DMA_SxCR_PSIZE; // (0x0<<11); // PSIZE 8 bit
	DMA2_Stream2->CR |= DMA_SxCR_MINC; // (0x1<<10); // увеличиваем память
	//DMA2_Stream5->CR |= (0x0<<9); //НЕ увеличиваем адрес периф
	DMA2_Stream2->CR &= ~(3UL<<6);  // Из переферии в память
	DMA2_Stream2->CR |= DMA_SxCR_CIRC ;//(0x1<<8); Circular mode
	DMA2_Stream2->CR |= DMA_SxCR_HTIE;
    DMA2_Stream2->CR |= DMA_SxCR_TCIE;
    DMA2_Stream2->CR |= DMA_SxCR_TEIE;
}

void DMA_Config(uint32_t perih_address, uint32_t mem_address , uint8_t data_amount ){

	DMA2_Stream2->NDTR = data_amount;

	DMA2_Stream2->PAR = perih_address;

	DMA2_Stream2->M0AR = mem_address;

	DMA2_Stream2->CR |= DMA_SxCR_EN;
	//// Включение DMA происходит только после того, как мы инициализировали GPIO и USART
    // И не надо его выключать и включать после, он уже работает в цикле.


}


void DMA2_Stream2_IRQHandler(void){

	if (READ_BIT(DMA2->LISR, DMA_LISR_HTIF2)){

		memcpy (&recieve_buf[i], &buf[0], SIZE/2);
		DMA2->LIFCR |= DMA_LIFCR_CHTIF2; // очистили флаг
		i = i+(SIZE/2);
		if (i>(2*SIZE)-1) i=0;

	}

	if ((READ_BIT(DMA2->LISR, DMA_LISR_TCIF2))){ // передача по 2 каналу завершена полностью

		memcpy (&recieve_buf[i], &buf[SIZE/2], SIZE/2);
		  DMA2->LIFCR |= DMA_LIFCR_CTCIF2; // очистили флаг
		  i = i+(SIZE/2);
		  if (i>(2*SIZE)-1) i=0;

		}
	}






int main(void)
{

	USART1_Init();
	DMA_Init();
	DMA_Config( (uint32_t) &USART1->DR, (uint32_t) buf, SIZE);

	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	//NVIC_SetPriority(DMA2_Stream2_IRQn, 4);





	while(1){

	}
}
