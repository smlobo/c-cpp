# Emscripten with SDL

* Use conan to generate user presets
```commandline
conan install . --output-folder=build-debug --profile=debug
```

* CMake configure with `emcmake` wrapper
```commandline
emcmake cmake --preset conan-debug
```

* CMake build
```commandline
cmake --build --preset conan-debug 
```

* Start a webserver & navigate to manual.html
```commandline
python3 -m http.server 9000
```
http://localhost:9000/manual.html

