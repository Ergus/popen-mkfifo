Readme
======

Simple code examples to emulate popen and mkfifo manually with other
system calls. All the code is implemented in C using the basic fork
and exec system calls.

All the important code is in `mypopen.c`. The `main_*` are test shown
how to use the library functions. The other `test.*` are example
applications. Finally the `Makefile` provides all the commands to test
the applications.

Usage
-----

Simply do:

```
make
```

or

```
make test
```

The build command creates some executables. These are ones needed to
test the functionalities.

```
./main_popen.x other command
```

for example, the tests execute:
```
./main_popen.x /usr/bin/ls -l
```
