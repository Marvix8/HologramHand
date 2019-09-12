
# Sistemas Operativos Avanzados

## Grupo: &nbsp;X1
- Amengual, Miguel Angel &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 35991055
- Castagna, Cristian Damian &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 37398696
- Krause, Ezequiel Otto &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 39758862
- Scarpello, Franco Dario &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 37842567

## Proyecto: &nbsp;HologramHand
El fin es crear una experiencia salida de pelicula, al poder **controlar hologramas con gestos de la mano.**

Para poder vivir la experiencia se necesita del guante que estamos desarrollando, siendo este un periférico (mouse, joystick) para celulares, un celular con Android y una sencilla pirámide de acrílico.

Pasos: 
-colocar guante y conectar al celular
-abrir aplicación
-colocar piramide de forma invertida sobre la pantalla con el dispositivo reposado.

**Se recomienda poca luz ambiente*

Y listo, ahora tendrás que comenzar a realizar gesto intuitivos con la mano para por ejmplo pasar de holograma a holograma, achicarlos, agrandarlos y darle a "play" en caso de tener movimiento.

## Descripción detallada del sistema y diseño incremental (Embebido)

### Arduino Mega2560
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Arduino%20Mega2560.jpeg" alt="drawing" width="300"/>

El hardware de Arduino consiste en una placa con un microcontrolador generalmente Atmel con puertos de comunicación y puertos de entrada/salida.

Arduino Mega2560 es una placa electrónica basada en ATmega2560. Tiene 54 pines de entrada / salida digital (de los cuales 15 se pueden usar como salidas PWM), 16 entradas analógicas, 4 UART (puertos serie de hardware), oscilador de cristal de 16 MHz, una conexión USB, un conector de alimentación, un encabezado ICSP y un botón de reinicio.

### Bluetooth HC-06
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Bluetooth%20HC-06.jpeg" alt="drawing" width="300"/>

El módulo de bluetooth HC-06 es un módulo esclavo, quiere decir que puede recibir conexiones desde un celular, PC, Tablet, pero no es capaz de generar conexiones hacia otros dispositivos bluetooth.

Tiene un modo de comandos AT que debe activarse. Una vez que estamos en el modo de comandos AT, podemos configurar el módulo bluetooth y cambiar parámetros como el nombre del dispositivo, contraseña, entre otras. 

Las conexiones para realizar con Arduino son bastante sencillas. Solamente requerimos colocar como mínimo la alimentación y conectar los pines de transmisión y recepción serial (TX y RX). Hay que recordar que en este caso los pines se deben conectar TX Bluetooth -> TX de Arduino y RX Bluetooth -> RX de Arduino.

### MPU-9250 (Acelerómetro).
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Acelerómetro%20MPU-9250.jpeg" alt="drawing" width="300"/>

Este módulo está basado en el sensor MPU9250 y contiene todo lo necesario para realizar rastreo de movimiento de 9 ejes (9 DoF). Combina un giroscopio de 3 ejes, un acelerómetro de 3 ejes y un magnetómetro de 3 ejes en un mismo chip. Integra un DMP (Procesador digital de movimiento) capaz de realizar complejos algoritmos de captura de movimiento de 9 ejes.

Se comunica con microcontroladores a través de una interfaz I2C y posee una librería muy difundida para su uso inmediato. Este sensor puede entregar 9 grados de libertad e incorpora un regulador de tensión a 3.3V y resistencias pull-up para su uso directo por I2C.

Para una captura precisa de movimiento rápido y lento, posee un rango de escala programable de 250/500/1000/2000 grados/seg para el giroscopio, 2g/4g/8g/16g para el acelerómetro y ±4800µT para el magnetómetro.

### Flex
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Flex1.jpeg" alt="drawing" width="150"/>  <img src="https://github.com/Marvix8/X1/blob/master/Flex2.jpeg" alt="drawing" width="150"/>

Este sensor aumenta su resistencia al ser flexionado. El conector tiene dos cables para conectarse directamente al protoboard.
Las dimensiones son 10 cm. x 3 cm. aunque la parte útil que medirá la resistencia doblado es de 8 cm.

### Led Digital
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/led_digital.png" alt="drawing" width="300"/>

Los LEDs de 5mm se presentan encapsulados en una cúpula de resina de color que tiene dos patas. La pata más larga es la de polaridad negativa y se denomina ánodo, la más corta es de polaridad positiva.

-	Color: Azul (455-465nm)
-	Ángulo de apertura: 120 grados
-	Diámetro: 5mm
-	Intensidad lumínica: 4000-5000 mcd
-	Tensión de entrada: 3 V-3,2 V
-	Corriente: 20 mA

### Led Analógico RGB
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Led%20RGB%20Analógico.PNG" alt="drawing" width="300"/>

Los LEDs de 5mm se presentan encapsulados en una cúpula de resina de color transparente que tiene cuatro patas: la más larga es de polaridad negativa y se denomina ánodo, las más cortas son de polaridad positiva y se denominan cátodo, en este caso representan el cátodo de R, G y B. Mediante estas patas se indicará la forma en la que el LED debe ser conectado al circuito.

-	Color: RGB
-	Ángulo de apertura: 30 grados
-	Diámetro: 5 mm
-	Longitud de onda: R:625 - G:525 - B:465 nM
-	Tensión de entrada: R:2,1 - G:3,1 - B:3,1 V
-	Intensidad lumínica R:2000 - G:4000 - B:2000 mcd
-	Dimerizable

### Diagrama Físico

<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Diagrama%20Físico.png" alt="drawing" width="600"/>


### Diagrama Funcional

<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Diagrama%20Funcional.png" alt="drawing" width="600"/>

### Armado de sensor Flex
Se hicieron pruebas con los distintos materiales (papel y grafito). Se cortaron pedazos de 110 mm. x 25 mm. de papel de dibujo artístico (de muy bajo gramaje), papel de hoja de impresión (gramaje intermedio) y en hoja canson (gramaje alto). 
Se pintó con un lápiz 9B, a mayor cantidad de B, mayor cantidad de grafito posee, y la mina más blanda es.
Se realizaron mediciones con el multímetro sobre los distintos pedazos de papel:
*  realizando el trazo del lápiz de forma vertical y horizontal, se descartó el trazo de forma horizontal dado que presentaba mayor variación de resistencias. 
* se descartó el que estaba hecho con hoja de impresora puesto que presentaba resistencia muy alta.

Se plantearon dos formas de disponer el conductor de cobre de 100 mm. x 5mm.:
* con dos tiras, una del lado de adelante y otra por detrás de la hoja.
* con dos tiras, ambas de un mismo lado de la hoja.

Se plantearon dos materiales distintos como aislante, plástico de radiografía y plástico de carpeta.

El armado de cada uno de los sensores mencionados se realiza siguiendo los siguientes pasos:

 1. Cortar el papel con tamaño 110 mm. x 25 mm.
 2. Cortar las dos tiras de conductor con tamaño 100 mm. x 5 mm.
 3. Cortar dos cables.
 4. Cortar dos pedazos de aislante (plástico) con tamaño 115 mm. x 25 mm.
 5. Soldar un cable por cada tira de conductor a uno de los extremos del mismo.
 6. Dependiendo del tipo de distribución de las tiras de conductor.
--  En la misma cara 
--- Pintar solo una cara del papel con grafito.
--- Pegar con pegamento las dos tiras de conductor sobre uno de los aislantes en la misma cara.
--- Pegar con pegamento la lámina de papel sobre el otro aislante, quedando el grafito del lado opuesto al mismo.
--- Enfrentar ambas caras (conductores contra papel) y unir con cinta adhesiva.
-- En distintas caras.
--- Pintar ambas caras del papel con grafito.
--- Pegar con pegamento cada una de las tiras de conductor sobre cada una de las láminas de aislante.
--- Colocar el papel entre medio de los dos aislantes (los conductores deben quedar presionando sobre el papel).
--- Unir con cinta adhesiva.

#### Armado en imágenes
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/1.%20Papeles%20con%20grafito.jpeg" alt="drawing" width="250"/> <img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/2.%20Aislante.jpeg" alt="drawing" width="250"/> <img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/3.%20Conductor%20con%20cable%20soldado.jpeg" alt="drawing" width="250"/>

<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/4.%20Conductor%20pegado%20a%20aislante%20y%20papel%20pegado%20a%20aislante.jpeg" alt="drawing" width="250"/> <img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/5.%20Cinta%20adhesiva%20a%20aislantes.jpeg" alt="drawing" width="250"/> <img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/6.%20Flex%20finalizado.jpeg" alt="drawing" width="250"/>

<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/7.%20Prueba%20con%20guante.jpeg" alt="drawing" width="250"/>

#### Prototipos
##### Prototipo 1: Ambos conductores en un mismo aislante de plástico de radiografía con papel de dibujo artístico
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Flex%20-%20opcion%201.jpeg" alt="drawing" width="300"/>

##### Prototipo 2: Un conductor en cada aislante de plástico de radiografía con papel canson
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Flex%20-%20opcion%202.jpeg" alt="drawing" width="300"/>

##### Prototipo 3: Un conductor en cada aislante de plástico de carpeta con papel canson
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Flex%20-%20opcion%203.jpeg" alt="drawing" width="300"/>

##### Prototipo 4: Ambos conductores en un mismo aislante de plástico de carpeta con papel de dibujo artístico
<img src="https://github.com/Marvix8/X1/blob/master/Sistema-Embebido/images/Flex%20-%20opcion%204.jpeg" alt="drawing" width="300"/>
