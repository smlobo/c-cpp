# Emscripten with SDL2 user input

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

* Start a webserver & navigate to the *binary* dir manual.html
```commandline
python3 -m http.server 9000
```
http://localhost:9000/build-debug/manual.html

### Credit
* https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-5-move-owl/
