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
* En reducción de bits damos la opcion mediante un select con la cantidad de bits que se quieren reducir de la imagen (de 1 a 7 bits), esto se hace haciendo una mascara de bits desplazando la cantidad de bits que se quieren reducir, en este caso descartariamos los bits menos significativos, y a su vez se le suma una costante (2^n/2) de esta manera los valores finales no empezarían en 0. 
* En umbralización automática tenemos OTSU y To zero inv (THRESH_TOZERO_INV). En el caso de umbralización to zero, cuando el valor del pixel es mayor a la del umbral se mantiene el mismo gris de la imagen original, mientras que cuando el valor del pixel es menor al umbral se asigna 0 (negro). Para nuestro proyecto elegimos la version invertida, ya que visualmente se aprecian mejor los objetos separados del fondo.
* Ecualización del Histograma. Aplicamos la funcion de opencv equalizeHist, en cada canal de la imagen (usando split) y despues usamos merge para unirlos y ese es el resultado que se muestra en pantalla.
* Morfología (Erosión, dilatación, apertura y cierre). El usuario puede elegir el tipo de elemento estructurante que desee (tenemos las opciones de: Morph_rect, Morph_cross o Morph_ellipse) junto con un n para el tamaño del kernel que viene dado en (2n + 1). Y también puede dar un kernel arbitrario, elegir cantidad de filas y columnas, y poner el valor de cada una de las casillas.
* En crecimiento de regiones tenemos dos checkbox, uno que dice "Vecindad de 8" en caso de no estar activo la vecindad sería de 4, y el segundo dice "Rango Flotante" que de no estar seleccionado el rango sería fijo. El color en que se quiere la región, se elige la posición en X y Y del pixel donde estaría la seed.
* En la aplicación de la Transformada de Fourier se abre un modal en el cual por defecto tenemos la previsualización del filtro paso bajo aplicado, si quitamos el check a la preview podremos ver la image de Magnitud centrada.* Nuestro historial se basa en dos listas que contendrán las imagenes (guardar hasta 4 cambios).
* Traslado (Panning) de la imagen en cualquier dirección, esto se hace medianteuas barras, la interfaz te permitirá ver una vista previa antes de hacer la operación.
* Rotación en ángulo arbitrario (Clockwise y Anticlockwise). Saldrá un modal con un input para colocar los grados que se quieran rotar y unos radio buttons para seleccionar si se rota clockwise o anticlockwise.
* Zoom nos ayudamos de los parametros de imgui con una textura con los siguientes parametros: GL_TEXTURE_MIN_FILTER y GL_TEXTURE_MAG_FILTER.