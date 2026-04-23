# 2D-Rigid-Body

```
 git clone https://github.com/libsdl-org/SDL.git vendored/SDL 
 cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

## Build with debug symbols and run with debugger
```
 cmake --build build --target debug
```

## Run normally with either
```
 cmake --build build
 ./build/debug/main
```
or
```
 cmake --build build --target run
```
