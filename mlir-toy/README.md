# MLIR Toy Language

## Configure
```
conan install . --output-folder=build-debug --profile=debug --build=missing
cmake --preset conan-debug
```

## Build
```
cmake --build --preset conan-debug
```

## Run
```
./build-debug/mlir-toyc ch1.mt -d -a
```

## Test
```
ctest --test-dir build-debug
```
