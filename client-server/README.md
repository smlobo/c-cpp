# HTTP Client & Server with poco

* Download and build dependencies
```commandline
conan install . --output-folder=build-debug --build=missing --profile=debug
```
Note: May need to use the default `Release` profile on Linux due to `armv8` libunwind compile error.

* CMake configure
```commandline
cmake --preset conan-debug
```

* CMake build
```commandline
cmake --build --preset conan-debug 
```

* Example run
```commandline
./build-debug/build/Debug/server [port]
./build-debug/build/Debug/client [start-port[:end-port]]
```
