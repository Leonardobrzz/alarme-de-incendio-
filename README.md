# Alarme de incêndio
Projeto final da disciplina de Sistemas Microprocessados, Universidade Federal do Ceará,
turma de Engenharia de Computação 2022.2 <br>
# Integrantes:
* Leonardo Alves braz - 474370
* Rodrigo da Silva Carvalho Maia - 511162
* Horley
* Jorge
<hr>

# Components
* Buzzer
* Sensor de gás MQ-7
* Greenpill STM32F070F6P6
* Protoboard
* Jumpers
* Módulo Wi-Fi ESP8266
* Sensor de chama fogo
# Imagens
# Descrição
O projeto consiste em um circuito alarme de incêndio com a utilização do Stm32, ambos os
sensores possuem pino analógico e digital, no nosso caso com a quantidade limitada de
pinos analógicos iremos conecta-los através do pino digital, que irá retornar o valor 0 caso
não detecte nenhum problema ou 1 se detectar gás ou fogo. Também utilizaremos um pino
para ligar o buzzer, para emitir o alerta. Ambos os sensores possuem ajuste de
sensibilidade, o que permite regulá-los de acordo com a necessidade.

