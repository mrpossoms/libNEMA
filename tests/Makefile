FLAGS=-Wint-to-pointer-cast
SRC=./src/*.c
INC=./../include
LIB=./../lib/libNEMA.a
LINK=-pthread -string

all:
	find ./ -iname "*.c" -exec gcc {} $(LIB) -I$(INC) -o {}.bin \;

clean:
	rm *.bin
