FLAGS=-Wint-to-pointer-cast
INC=-I./../include
LIB=-L/usr/local/lib
LINK=-pthread -lNEMA

all:
	find ./ -iname "*.c" -exec gcc {} $(LIB) $(INC) -o {}.bin $(LINK) \;

clean:
	rm *.bin
