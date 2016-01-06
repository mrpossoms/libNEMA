FLAGS=-Wint-to-pointer-cast
SRC=./src/*.c
INC=./include
DST=./lib
LINK=-string
LIB=libNEMA.a

TSTINC=./tests/include
TST=./tests
TSTS=./tests/*.c

all:
	gcc -I$(INC) -c $(SRC)
	ar rcs $(DST)/$(LIB) *.o

testlib: $(SRC)
	gcc -I$(TSTINC) -c $(SRC)
	ar rcs $(TST)/lib/$(LIB) *.o
	rm *.o

tests: testlib
	$(foreach test, $(TSTS), gcc $(test) -o $(TST)/bin/$(test).bin
install:
	echo -n "Installing the library"
	cp ./$(INC)/*.h /usr/local/include
	cp $(DST)/*.a /usr/local/lib
clean:
	find ./ -iname "*.a" -exec rm {} \;
	find ./ -iname "*.o" -exec rm {} \;
	find ./ -iname "*.out" -exec rm {} \;
