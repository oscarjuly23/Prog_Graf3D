## Description
El objetivo de este proyecto era crear un sistema solar en una aplicación de gráficos 3D. Se requería utilizar texturas de http://planetpixelemporium.com/ y, opcionalmente, cargar geometría adicional de otras fuentes. 
## Implementaciones
### Ecuación de Sombreado Phong Completa:
Se aplicó la ecuación de sombreado Phong en al menos uno de los planetas del sistema solar. Esto implica calcular la iluminación difusa, especular y ambiental en el fragment shader para simular la apariencia de un objeto 3D con materiales que reaccionan a la luz.

### Texturas de Planetas: 
Se utilizaron texturas de http://planetpixelemporium.com/ para mapear las superficies de los planetas. Cada planeta tenía su textura correspondiente aplicada.

### Movimiento de Cámara:
Se implementó la capacidad de mover la cámara para explorar el sistema solar desde diferentes ángulos y distancias. Esto implicaba cambios en la matriz de vista para controlar la posición y la orientación de la cámara.

### Rotación de Planetas:
Al menos un planeta se configuró para que rote alrededor de su propio eje. Esto proporcionaba una sensación de realismo al sistema solar.

### Movimiento de Planetas Jerárquico:
Se permitió que al menos un planeta, por ejemplo, una luna, rotara alrededor de otro planeta, como la Tierra. Esto representaba un movimiento jerárquico y orbitario.

## Futuras implementaciones:
### Milkyway Skybox:
Agregar un skybox que represente una vista realista de la Vía Láctea en el fondo del sistema solar para crear una atmósfera más inmersiva.
### Carga de Múltiples Geometrías:
Incorporar geometrías adicionales, como naves espaciales, satélites o UFOs, para enriquecer la escena y permitir la interacción con estos objetos.
### Rotación de Planetas: 
 Implementar la rotación de varios planetas alrededor de sus ejes, creando así una simulación más realista del sistema solar.
### Movimiento Jerárquico de Planetas:
Extender el movimiento jerárquico para varios planetas y sus lunas, creando un sistema solar completamente funcional con órbitas y movimientos precisos.
### Iluminación Realista:
Configurar una iluminación más realista donde la luz proviene directamente del sol y afecta a todos los objetos en el espacio. Esto incluye sombras y reflexiones realistas.
### Sombreado Blinn-Phong:
Utilizar el modelo de sombreado Blinn-Phong para obtener reflejos especulares más precisos y detallados en los objetos.
### Efectos de Transparencia: 
Agregar efectos de transparencia, como ventanas de naves espaciales que permiten ver el interior de las naves.
### Mapas de Normales y Especulares: 
Utilizar mapas de normales y especulares para mejorar la apariencia de las superficies y lograr detalles adicionales, como rugosidades o brillos.
### Textura de Día/Noche en la Tierra:
Implementar una textura de día y noche en la Tierra para simular el ciclo día/noche en el planeta.
### Líneas de Órbitas:
Agregar líneas de órbita para mostrar visualmente las trayectorias de los planetas y lunas alrededor del sol.
### Focos de Luz: 
Incluir focos de luz en naves espaciales o en la superficie de los planetas para iluminar áreas específicas.
### Transiciones de Escena: 
Crear transiciones de escena a medida que la cámara se acerca a un planeta o al aterrizar en su superficie.
##
<p align="center">
  <img src="https://github.com/oscarjuly23/Prog_Graf3D/assets/39187459/eb3f93ba-a2ac-4a8d-b9ed-915dbec33faa">
</p>
