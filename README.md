# Tarea_Saltos_B41672_Alejandro_Cedeno
En este repositorio se guardan todos los archivos para poder correr la tarea de saltos. Para utilizar este programa y probar las funciones es recomendable usar el makefile adjunto. Para compilar el archivo solo se debe hacer "make", luego dependiendo del predictor que se desee se usa: 

-"make bim_compile" para correr el Bimodal 

-"make glob_compile" para correr el de historia global 

-"make priv_compile" para correr el de historia privada 

-"make torneo_compile" para correr el de torneo Los parametros de estos comandos son los mismos que los que incluye los resultados en el pdf. Excepto que tienen el parametro 

-o en 1 para que siempre genere el archivo stats.txt que da los primeros 5000 saltos.

Si se quiere imprimir diferentes se puede utilizar make compile y modificar sus parametros como se desee.
