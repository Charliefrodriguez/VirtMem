all : test
test: ../my_vm.h
	gcc test.c -L../ -lmy_vm -g -lpthread -m32 -o test

clean:
	rm -rf test
