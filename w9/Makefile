TARGETS=time_reads write_test_file

all: $(TARGETS)

%: %.c
	gcc -Wall -g -o $@ $^

clean:
	rm -f $(TARGETS)
