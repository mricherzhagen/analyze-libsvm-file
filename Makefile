all: analyze_libsvm_file

analyze_libsvm_file: analyze_libsvm_file.c
	$(CC) $(CFLAGS) -Wall analyze_libsvm_file.c -o analyze_libsvm_file -lm

clean:
	rm analyze_libsvm_file
