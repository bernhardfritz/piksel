<!-- TODO logo here -->

## piksel

### What is it?

piksel is a simple 2D graphics library for C++.

piksel applications can be deployed natively or on the web through the power of WebAssembly. :zap:

### Documentation

*in the works* :muscle:

### Quickstart

> These are preliminary instructions and will be further elaborated as development progresses

```
quickstart
|-- data
|-- lib
|  `-- piksel
|-- src
|  |-- app.cpp
|  |-- app.hpp
|  `-- main.cpp 
`-- CMakeLists.txt
```
`data` is a directory which is destined for font and image resources.

`piksel` is the cloned repository.

**app.cpp**
```cpp
#include "app.hpp"

void App::setup() {
    // load fonts and images here
}

void App::draw(piksel::Graphics& g) {
    g.background(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    g.rect(50, 50, 100, 100);
}
```

**app.hpp**
```cpp
#ifndef APP_HPP
#define APP_HPP

#include <piksel/baseapp.hpp>

class App : public piksel::BaseApp {
public:
    App() : piksel::BaseApp(640, 480) {}
    void setup();
    void draw(piksel::Graphics& g);
};
#endif /* APP_HPP */
```

**main.cpp**
```cpp
#include "app.hpp"

int main() {
    App app;
    app.start();
}
```

**CMakeLists.txt**
```cpp
cmake_minimum_required(VERSION 3.12)
project(quickstart)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/piksel)

set(SOURCES
    src/main.cpp
    src/app.cpp
)
add_executable(index ${SOURCES})
target_include_directories(index
    PUBLIC src
    PUBLIC lib/piksel/src
)
target_link_libraries(index
    piksel
)
```

#### Build natively

Open a terminal in the `quickstart` working directory
```bash
mkdir build_native
cd build_native
cmake ..
make
./index
```

#### Build for the web

Install Emscripten by following the [WebAssembly developer's guide](https://webassembly.org/getting-started/developers-guide/)

Open a terminal in the `quickstart` working directory
```bash
mkdir build_web
cd build_web
emcmake cmake ..
make
emrun --no_browser --port 8080 .
```
Open your browser and navigate to http://localhost:8080/
