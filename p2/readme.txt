Informática Gráfica
Practica 2

Adicional:
El vector de puntos de la generatriz puede meterse en cualquier sentido, de arriba-abajo, de abajo-arriba, de izquierda-derecha, de derecha-izquierda
La curva de la generatriz se puede definir para cualquier eje x, y o z

Función de rotación <-- Explicación
void  parametros(vector<_vertex3f> perfil1, int num1, string ejeGiro="y", string tapas="2_tapas"); // tapas = [2_tapas, tapa_inf, tapa_sup, no_tapas] ejeGiro = [x, y, z]
La función rota el perifl que se pasa como parametro perfil1
num1 es el número de caras de revolución que se van a generar 
En ejeGiro se define el eje de revolución, valores admitidos: x, y o z, por defecto si no se pone nada se rota sobre el eje Y
En tapas se indica si se quieren tapas, las opciones son: 2_tapas, tapa_inf, tapa_sup o no_tapas, por defecto si no se pone nada pintara las 2 tapas
Este atributo esta por si queremos hacer un cilindro sin tapas, si se pasa un objeto con puntos terminales en el eje de giro no se pintaran las tapas aunque se haya puesto 2 tapas
Es decir una esfera no hara caso de este atributo por que sus dos puntos terminales estan en el eje de giro

Instrucciones de funcionamiento (Normal Keys)
Tecla Q: Salir
Tecla O (de objeto): Cambiar de modelo: 
    OBJETO_PLY_REVOLUTION: objeto ply generado por revolución, desde archivo se importa perfil de revolución
    OBJETO_PLY: objeto ply con vertices y caras importadas desde archivo
    ESFERA
    REVOL_X: objeto generador po revolución respecto del eje X
    REVOL_Y: objeto generador po revolución respecto del eje Y
    REVOL_Z: objeto generador po revolución respecto del eje Z
    CONO
    CILINDRO
Tecla T o M (de tipo o modo de visualización): Cambiar de tipo de visualización: Puntos -> Aristas -> Solido Unicolor -> Solido damero
