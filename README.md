# Введение DMA 

![Static Badge](https://img.shields.io/badge/Unic_Lab-green)
![Static Badge](https://img.shields.io/badge/STM32-red)
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/Vernicovskiy/STM32_TIM)
![GitHub Repo stars](https://img.shields.io/github/stars/Vernicovskiy/STM32_TIM)
![GitHub watchers](https://img.shields.io/github/watchers/Vernicovskiy/STM32_TIM)
![GitHub top language](https://img.shields.io/github/languages/top/Vernicovskiy/STM32_TIM)

Любая операция в МК -  запись единицы или ноля в определённую ячейку (регистр) памяти.

Когда отправляем данные по USAR, то берём данные из какого-то нашего массива, и последовательно перекладываем их в регистр (USART->DR).

Все действия, запись в регистр, чтение, перекладывание из массива в регистр и обратно, производятся с помощью CPU

<p align="center">
<img src="PNG/image.png" alt="Diagram of System Timer (SysTick)" width="500"/>
</<p align="center">

То есть CPU тратит своё время на эти операции и само собой программа ничего другого в это время делать не может.

Для повышения производительности существует модуль **DMA**

DMA позволяет осуществлять транзакции в фоновом режиме, без вмешательства со стороны CPU. Во время этой операции основной CPU может выполнять другие задачи, прерываясь только в том случае, когда транзакция закончилась и новый блок данных целиком доступен для обработки. Большие объемы данных могут быть переданы без значительного ухудшения общей производительности системы.


<p align="center">
<img src="PNG/image-1.png" alt="Diagram of System Timer (SysTick)" width="500"/>
</<p align="center">



