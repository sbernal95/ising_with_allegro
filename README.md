# ising_with_allegro
A real time Ising model with MonteCarlo method in C!

#Introducción

El algoritmo Metropolis-Hasting es un método de Monte Carlo para muestrear distribuciones de probabilidad en donde el problema sea obtener una muestra aleatoria desde una distribución conocida. Es una forma no determinista de aproximar una ecuación costosa computacionalmente hablando.
El modelo de Ising consiste en una red bidimensional en donde cada unidad de la red representa un spin. La idea de este modelo es que se pueda generar una secuencia de estados de esta red de tal manera que se distribuyan según la distribución de Boltzmann.
  
#Implementación 
En el archivo adjunto llamado ising1.c se implementó un código secuencial el cual genera fotogramas como vistas de la matriz 2D que contiene los spines como elementos, con valores de -1 o 1.
Primero se inicializan las variables relacionadas con la pantalla (librería Allegro), segundo se inicializa aleatoriamente la matriz de spines. Luego, al ingresar al bucle que genera las vistas de la matriz se inicia el algoritmo: se selecciona aleatoriamente un spin, se calcula la energía que rodea a este spin y se decide el cambio de estado. Si la energía circundante es menor que 0 o si un numero aleatorio entre 0 y 1 es menor a la distribución de Boltzmann evaluado en la temperatura del sistema se cambia el estado, en caso contrario se mantiene.

#Algunas definiciones:

• L: Este valor representa el tamaño de la matriz o cuadrícula de tal manera que sus dimensiones sean de LxL. Para esta ocasión se estableció L = 128.

• STEPS: Número de pasos de Monte Carlo para el algoritmo Metropolis-Hasting. Este valor regula la cantidad de consultas aleatorias que se realizan por cada fotograma.

• SCALE: Este factor es para escalar la vista de la matriz a un valor pixelfriendly. Cada
valor en la matriz equivale a SCALExSCALE pixeles en la pantalla.

• temp: Esta variable se inicializa al principio de la función main y toma el valor del
primer argumento pasado por consola. Representa al valor de la temperatura y su valor
default es 1.

• mag: Esta variable contiene el valor de la magnetización total de la matriz. S

• t1, t2, t_total, dt: Estas variables permiten calcular el tiempo de de ejecución de cada
vez que se aplica el algoritmo. t1 y t2 guardan el valor inicial del ciclo y el final respectivamente. t_total acumula la diferencia t2-t1. dt es el resultado de t_total dividido en la cantidad de ciclos que han ocurrido. dt puede interpretarse como el tiempo medio actual que toma cada ciclo en completarse.

Para compilar el código en la terminal se utiliza la siguiente línea (previa instalación de las librerías de Allegro para la visualización):

gcc -g ising1.c -o ising1.x -lallegro -lallegro_primitives -lallegro_main

Para ejecutarlo se utiliza la siguiente línea:
./ising1.x 2

El argumento que acompaña al ejecutable es la temperatura de la simulación (en el ejemplo es 2). Este valor puede omitirse y la ejecución se realizará con temp = 1 como default.
Con este código se generó las 3 grabaciones con T = {0.5, 2, 4} llamadas ising_t05.mov, ising_t2.mov y ising_t4.mov respectivamente, adjuntas en los archivos del trabajo.
