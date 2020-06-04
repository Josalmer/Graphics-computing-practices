TARGETS= practica0
EJECUTABLE= practica0
MODULOS=  $(EJECUTABLE).o

#  CFLAGS= Parametros para el compilador de C/C++       
CFLAGS=  -g -c -DXWINDOWS -I/usr/include -I     

#  LDFLAGS= Parametros para el linkador
LDFLAGS=   -L/usr/lib  

#  LIBS= librerias a usar.
LIBS=    -lglut -lGLU  -lGL -lX11 -lXext  -lc -lm

#  CC= orden de UNIX que invoca al compilador de C/C++ de GNU
CC=        g++

# archivo ejecutable a generar
$(EJECUTABLE): $(MODULOS)	
	$(CC)  -o$(EJECUTABLE) $(LDFLAGS) $(MODULOS)  $(LIBS)   

clean:
	rm -f *.o
	rm -f $(TARGETS)

superclean:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)