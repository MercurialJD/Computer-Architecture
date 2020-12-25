gcc vector.c -c -o vector.o
ar -cvr libvector.a vector.o
gcc test.c -o staticvector -L . -l vector