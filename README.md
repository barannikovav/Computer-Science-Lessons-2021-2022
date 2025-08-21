# Computer Science MIPT course. 3-rd semester - Programming for Linux.
This repository contains small programs that repeat the work of some Linux utilities for educational purposes.

## Building with make
To build everything:
```
make
```

To build particular task:
```
make 01_stat
```

## Building with cmake
To build everything:
```
cmake -S . -B build
cmake --build build
```

To build particular task:
```
cmake -S . -B build
cd build
make 01_stat.exe
```