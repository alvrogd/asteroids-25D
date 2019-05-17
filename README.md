# Asteroids 2.5D

## ¿De qué trata?
El juego aquí disponible sirve como tributo al clásico __Asteroids__ (Atari, 1979).

El juego consiste en, al igual que la versión original, destruir todos los asteroides presentes en pantalla sin que la nave sea destruida.
Para ello, la nave cuenta con la posibilidad de disparar cargas de energía que permiten desintegrar los asteroides. Pero, __¡cuidado!__ Si
la nave choca con un asteroide, sus frágiles tanques de energía de desestabilizarán y provocarán que tanto el asteroide como ella se
desintegren.

## ¿Por qué 2.5D?

Como se trata de un tributo moderno al juego, se ha optado por crear un juego tridimensional que aporte una mayor sensación de inmersión,
frente a la opción de preservar la clásica vista de pájaro.

Sin embargo, sí se ha conservado la limitación de que la nave se pueda mover en un solo plano. El motivo de ello es que, si se permitiese
un movimiento en 3D, el jugador debería estar atento en todo momento a si hay asteroides que se le aproximen tanto por encima como por
debajo. Además, si los asteroides pudiesen moverse en las tres dimensiones, posiblemente se dificultaría el hecho de situar una cámara
relativamente alejada de la nave (como se ha optado aquí) porque sería muy probable que en todo momento se interpusiesen asteroides entre
la cámara y la nave, dificultando al jugador el control de esta.

Por todo ello, se ha decidido preservar el movimiento en un único plano de modo que, con ayuda de la cámara, el jugador pueda ser
consciente en todo momento de qué le rodea y no tener miedo a moverse todo lo rápido que pueda por la escena, por si pudiese chocar contra
un asteroide.

## ¿Y cómo se juega?

Una vez ejecutado el juego, este carga desde ficheros todos los recursos que necesita (probablemente tarde unos segundos). Tras ello,
prepara todo lo necesario para que el juego funcione y deja los mandos al jugador. Los controles son realmente sencillos:

* __W__: la nave acelera, moviéndose en la dirección hacia la que mira. ¡Ten presente que no hay botón para desacelerar, ya que nos
encontramos en el espacio!
* __A__: la nave modifica su rumbo, girando a la izquierda.
* __D__: la nave modifica su rumbo, girando a la derecha.
* __Espacio__: la nave dispara una ráfaga de cargas de energía que pueden destruir asteroides.

Además, se ha incorporado de todos modos la posibilidad de que la cámara se sitúe muy cercana a la nave, en lugar de guardar las distancias
con ella, por si el jugador prefiere arriegarse más y no conocer tan bien qué se puede encontrar en sus cercanías. Para alternar entre
ambos modos de la cámara tan solo es necesario usar dos botones:

* __F1__: la cámara se situará próxima a la nave.
* __F2__: la cámara se situará relativamente alejada de la nave.

## ¿Y qué pasa si choco con un asteroide?

En dicho caso, la nave se desintegrará tal y como se comentó anteriormente, por lo que el jugador habrá perdido la partida. ¡Pero no
te preocupes! Si quieres seguir jugando, puedes pulsar la __tecla R__ para reiniciar la partida. De hecho, es posible pulsar dicha tecla
para reiniciarla en cualquier momento del juego.

## Creo que tengo una posible idea para mejorar el juego

Efectivamente, por falta de tiempo, el juego no ha podido ser realizado completamente, tal y como se había planificado originalmente:

* Sería necesario añadir un sistema de puntuación, que se muestre idealmente dentro de la propia ventana del juego, en lugar de por la
consola.
* Una vez añadido dicho sistema de puntuación, sería intersante añadir la posibilidad de que la nave tenga una serie de vidas antes de
ser completamente destruida, y perder consecuentemente la partida. Sin dicho sistema de puntuación, el que el jugador no pierda al
instante no aporta nada de valor al juego, dado que no se está perdiendo realmente nada al acabarse la partida.
* Finalmente, quizá sería interesante añadir la posibilidad de que, de forma aleatoria, aparezcan aliens en la partida que intenten
eliminar al jugador, tal y como sucede en el juego original.

Por otra parte, también sería necesario efectuar importantes optimizaciones en el rendimiento global del juego:

* Por ejemplo, es necesario adoptar una estrategia _instancing_ para la representación de las partículas del juego, dado que la vía
actualmente implementada produce un notable cuello de botella en el rendimiento del juego. Algunos enlaces sobre dicha técnica:
  * http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
  * https://learnopengl.com/Advanced-OpenGL/Instancing
 
## ¿Qué recursos se han empleado en la elaboración del juego?

El desarrollo de este proyecto supone la síntesis de los conocimientos adquiridos por el autor sobre la computación gráfica a lo largo de
cuatro meses. Estos han provenido de una extensa lista de diversas fuentes, la cual sería imposible enumerar al completo ahora mismo.

De todos modos, cabe destacar que la sección correspondiente a la implementación de la carga de modelos en OpenGL se encuentra
fundamentalmente basada en los siguientes tutoriales:

* https://learnopengl.com/Model-Loading/Assimp
* https://learnopengl.com/Model-Loading/Mesh
* https://learnopengl.com/Model-Loading/Model

Por otra parte, tanto los modelos empleados para las naves y asteroides como los sonidos utilizados no han sido creados por el autor,
sino que fueron tomados de las siguientes fuentes:

* __Modelos__:
  * __Nave__: https://www.turbosquid.com/FullPreview/Index.cfm/ID/465782
  * __Asteroides__: https://www.cgtrader.com/free-3d-models/exterior/landmark/2600-lowpoly-rock
  
* __Efectos de sonido__: 
  *__Disparos de la nave__: https://www.zapsplat.com/music/laser-rapid-fire-3/
  *__Desintegración de los objetos__: https://www.youtube.com/watch?v=anBOuaxwDZI
  
* __Música__:
  *__De fondo__: https://nicoleproducer.itch.io/free-chiptune-song-galactik-funk

