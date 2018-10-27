# How to set up a new project

## Create a folder structure

Setting up a new project is fairly simple. Let's start by creating the following folder structure:

```
myproject
|-- lib
`-- src
```

`lib` is where we will put our libraries.

`src` is where we will put our source code.

## Clone piksel from GitHub

Let's continue by cloning the piksel repository from GitHub. For this we will use a terminal. Navigate to the `lib` directory and enter

```bash
git clone --recursive https://github.com/bernhardfritz/piksel.git
```

`piksel` does not require you to install any dependencies to your system. All required dependencies are self-contained within `piksel`.

## Write code

Now we can start writing code. Create and populate the following files inside of the `src` directory:

**app.hpp**

In the code below we create a new class `App` that extends [piksel::BaseApp](reference/baseapp.md). Among others, [piksel::BaseApp](reference/baseapp.md) offers virtual functions [setup](reference/baseapp.md#setup) and [draw](reference/baseapp.md#draw) which we declare here and implement in `app.cpp`. The constructor of [piksel::BaseApp](reference/baseapp.md) allows us to set the canvas size to 640x480 pixels.

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

**app.cpp**

This is where we implement [setup](reference/baseapp.md#setup) and [draw](reference/baseapp.md#draw). [setup](reference/baseapp.md#setup) will only be called once when the app starts, [draw](reference/baseapp.md#draw) will be called in a loop once per frame, immediately after setup. The parameter `g` of the [draw](reference/baseapp.md#draw) function is a reference to an object of type [piksel::Graphics](reference/graphics.md) which is used to define what should be rendered. Here we simply clear the canvas with a gray background color and render a rectangle on top.

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

**main.cpp**

Here we include, define and start the app.

```cpp
#include "app.hpp"

int main() {
    App app;
    app.start();
}
```

Our code is now ready to be compiled and executed. We will use [CMake](https://cmake.org/) to generate a Makefile that we can then use to compile our code. [CMake](https://cmake.org/) expects us to write a `CMakeLists.txt` file where we should provide all information needed to perform a build. For this we will create a new file called `CMakeLists.txt` in the project's root directory (`myproject`).

## Create a CMakeLists.txt file

**CMakeLists.txt**

Without going into to much detail, you will need to edit this file whenever you intend to e.g. add new source files or new libraries. For now we will just copy over the content below:

```
cmake_minimum_required(VERSION 3.12)
project(myproject)
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

`piksel` is set up in a way that allows you to use the same `CMakeLists.txt` file no matter if you plan to target a native platform or the web. 

Before we continue, let's verify that everything is in place. If you followed the instructions above you will end up with this folder structure:

```
myproject
|-- lib
|  `-- piksel
|-- src
|  |-- app.cpp
|  |-- app.hpp
|  `-- main.cpp 
`-- CMakeLists.txt
```

Let's continue by [building our project](gettingstarted/buildproject.md)!
