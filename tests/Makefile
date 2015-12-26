FLAGS=-Wint-to-pointer-cast
INC=./../include
LIB=-lNEMA
LINK=-pthread -string

all:
	find ./ -iname "*.c" -exec gcc {} $(LIB) -I$(INC) -o {}.bin \;

clean:
	rm *.bin
