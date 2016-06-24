FLAGS=-Wint-to-pointer-cast -g
INC=-I./../include
LIB=-L/usr/local/lib
LINK=-pthread ./../lib/libNEMA.a

all:
	find ./ -iname "*.c" -exec gcc {} $(LIB) $(INC) $(FLAGS)  -o {}.bin $(LINK) \;

clean:
	rm *.bin
