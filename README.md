# sv-lora-sender-receiver
Programa de exemplo de Sender &amp; Receiver utilizando LoRa

- Versao....: 1.0
- Data......: 28/04/2024
- Autor.....: Sidney Visentini

OBS:
Para funcionar com a placa V2, TEM QUE USAR A VERSAO 1.0.9

![image](https://github.com/svisentini/sv-lora-sender-receiver/assets/15691421/e5fb8715-60c4-48c6-8b76-adf414037e08)


# Preparação do ambiente de Desenvolvimento
https://docs.heltec.org/en/node/esp32/quick_start.html

# Link do video >> Botao e buzina
https://www.youtube.com/watch?v=FizuHaatRAs&t=390s

# Receiver tem um led que liga de acordo com o botao do Sender
https://www.usinainfo.com.br/blog/esp32-lora-wifi-sx1278/

# Linux
- Listas as portas seriais (USB)

dmesg | grep ttyUSBD

- Liberar acesso a porta

sudo chmod a+rw /dev/ttyUSB0
