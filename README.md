## App

para poder correr el proyecto debes seguir los siguientes pasos 
1. Clonar el proyecto
2. Clickear el script genproject.bat
3. [Descargar opencv2](https://sourceforge.net/projects/opencvlibrary/)
4. Hacer doble click en el archivo descargado para extraerlo
5. Copiar los dll de la carpeta `opencv/build/x64/vc1/bin` dentro de la carpeta `PDI-Template/App/bin/Debug/` del proyecto


<!-- ## Notas

* Sientanse libres de hacer las preguntas o solicitar cambios al template por medio de issues
* [Aquí está la documentacion de opencv2](https://docs.opencv.org/4.4.0/)
* Para construir el proyecto se usa [premake5](https://premake.github.io). Para agregar librerías se debe modificar el script premake5.lua y correr el script genproject.bat  -->

## Asunciones


* Cuantización/Reducción de colores de una imagen de 24 bits con al menos tres (3) métodos. Por ejemplo: reducción de bits por pixel, conversión a paleta (Popularity Algorithm, Median Cut), k-media, entre otros.
* En umbralización automática tenemos OTSU y To zero inv (THRESH_TOZERO_INV). En el caso de umbralización to zero, cuando el valor del pixel es mayor a la del umbral se mantiene el mismo gris de la imagen original, mientras que cuando el valor del pixel es menor al umbral se asigna 0 (negro). Para nuestro proyecto elegimos la version invertida, ya que visualmente se aprecian mejor los objetos separados del fondo.
* Ecualización del Histograma.
* Morfología (Erosión, dilatación, apertura y cierre). Se deben definir elementos estructurantes por defecto y dar la opción al usuario de crear uno arbitrario.
* Crecimiento de regiones con opción para escoger rango fijo o flotante, además de vecindad a 4 u 8.
* Aplicación de la Transformada de Fourier para obtener la imagen de Magnitud (Centrada o de Espectro). Luego proveer la opción para aplicar algún filtro paso bajo en el Dominio Frecuencial y aplicar la Transformada de Fourier Inversa para ver el resultado en el Dominio Espacial.
* Nuestro historial se basa en dos listas que contendrán las imagenes (guardar hasta 4 cambios).
* Traslado (Panning) de la imagen en cualquier dirección.
* Rotación en ángulo arbitrario (Clockwise y Anticlockwise).
* Zoom nos ayudamos de los parametros de imgui con una textura con los siguientes parametros: