# BUSCAMINAS

***Documentacion***

[Documentacion SDL](https://www.cs.pu.edu.tw/~tsay/course/gameprog/doc/ebooks/Focus%20on%20SDL%20%20(2003).pdf)

[Pdf sobre el Buscaminas ](https://dash.harvard.edu/bitstream/handle/1/14398552/BECERRA-SENIORTHESIS-2015.pdf)
	
***Video***

### SDL Lista de reproduccion de CodingMadeEasy
	https://www.youtube.com/playlist?list=PLHJE4y54mpC5_eEz9gCqIkNpU-n_2eyNt

### SDL Lista de reproduccion de CoderGopher
    https://www.youtube.com/watch?v=KsG6dJlLBDw&list=PL2RPjWnJduNmXHRYwdtublIPdlqocBoLS
	
### Buscaminas hexagonal (Posibilidad)
	
	https://gamedevelopment.tutsplus.com/es/tutorials/creating-hexagonal-minesweeper--cms-28655

## Lógica Juego:
Un algoritmo que ponga minas en una matriz, al principio “randomizado”.
El primer movimiento DEBE ocurrir sobre una casilla vacía.
Si se revela una casilla vacía, esta mostrará sus casillas adyacentes incluyendo esquinas.


## **Algo de historia:**

  ***Buscaminas fue uno de los primeros juegos que se instalaron en la interfaz de Windows. Fue desarrollado por el ingeniero Robert Doner en 1989 y se hizo popular a raíz de las versiones para Windows. A pesar de lo adictivo de tener que despejar un campo de minas, el juego no se inventó como fuente de entretenimiento.
  Fue lanzado por primera vez como parte del Microsoft Entertainment Pack. Sin embargo, luego se integró de forma definitiva en Windows 3.x, reemplazando al juego Reversi. El juego continuó con el Sistema operativo hasta Windows 8.***

# Reglas:

El juego consiste en despejar todas las casillas de una pantalla que no oculten una mina.
Algunas casillas tienen un número, el cual indica la cantidad de minas que hay en las casillas circundantes. Así, si una casilla tiene el número 3, significa que de las ocho casillas que hay alrededor (si no es en una esquina o borde) hay 3 con minas y 5 sin minas. Si se descubre una casilla sin número indica que ninguna de las casillas vecinas tiene mina y éstas se descubren automáticamente.
Si se descubre una casilla con una mina se pierde la partida.
Se puede poner una marca en las casillas que el jugador piensa que hay minas para ayudar a descubrir las que están cerca.
 
# Niveles : 
El juego también posee un sistema de récords para cada uno de los 4 niveles en el que se indica el menor tiempo en terminar el juego. Los niveles son (para las nuevas versiones):

Nivel principiante: 8 × 8 casillas y 10 minas.

Nivel intermedio: 16 × 16 casillas y 40 minas.

Nivel experto: 16 × 30 casillas y 99 minas.

Nivel personalizado: en este caso el usuario personaliza su juego eligiendo el número de minas y el tamaño de la cuadrícula.
>>>>>>> daeed6945148438a1716d96849e99fc198bb488d
