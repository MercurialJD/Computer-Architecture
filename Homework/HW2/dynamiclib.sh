gcc vector.c -shared -o libvector.so -fPIC
gcc test.c -o dynamicvector -L rpath ./libvector.so