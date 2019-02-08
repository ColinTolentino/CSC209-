FLAGS = -Wall -Werror -std=gnu99 



DEPENDENCIES = helpers.h

all: deserialize serialize

deserialize: test_print.o ptree.o
	gcc ${FLAGS} -o $@ $^

deserialize: print_ptree.o ptree.o
	gcc ${FLAGS} -o $@ $^

basic_test: serialize deserialize
	$(call deserialize,serialize)
	$(call serialize.test.dat)

	if [-a serialize && -a deserialize ]; then
		echo "Basic test passed"
	else
		echo "Basic test failed"
	fi

%.o: %.c ${DEPENDENCIES}
	gcc ${FLAGS} -c $<

clean: 
	rm -f *.o deserialize serialize
