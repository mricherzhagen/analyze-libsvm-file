all: analyze_libsvm_file

analyze_libsvm_file: analyze_libsvm_file.c
	$(CC) analyze_libsvm_file.c -o analyze_libsvm_file -lm
