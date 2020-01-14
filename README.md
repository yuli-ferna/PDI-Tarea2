# CG3-P1

cuando clickeas el script genproject te saldrá el proyecto de visual studio.

No añadas librerías usando la interfaz gráfica de visual studio porque esas dependencias se perderan.

Todas esas librerías deben incluirse editando el archivo premake5.lua


## El proyecto 

# EasyDIPAPI
Contiene todo el código relacionado a la API del proyecto y es donde haremos el procesamiento de las imágenes

# EasyDIPBenchmark
Contiene todas las pruebas de la app y las mediciones del tiempo, debería exportar un archivo csv para que podamos hacer las gráficas en excel del proyecto

# EasyDIPClient
Interfaz gráfica de la app que permite hacer un preview de todas las funciones de la API.

Acá solo se utiliza la API de EasyDIPAPI para trabajar en la app

