# How to build your project

`piksel` allows you to build your project natively or for the web through the power of WebAssembly. Native builds are probably more preferable during development since they are far easier to debug. Once you feel like trying out your app in a browser, go ahead and build for the web. Native and web builds do not interfere with each other, you are safe to target both at the same time.

## Build natively ![linux](_media/linux.svg) ![macos](_media/macos.svg)

#### Generate the Makefile using cmake

In your project's root directory (e.g. `myproject`), create a new folder `build_native`. Open a terminal, navigate to `build_native` and enter:

```bash
cmake ..
```

[CMake](https://cmake.org/) will do it's thing, generating all kinds of files, including a new file called `Makefile`.

#### Compile using make

Make is a tool that will build your app based on the instructions provided by the so called Makefile. It is as simple as entering a single command:

```bash
make
```

The first time you execute make it will take some time until it's finished. Consequent builds will be much faster!

From now on, everytime you modify your sourcecode, you merely need to call `make` to rebuild your app.

#### Launch your app

```bash
./index
```

## Build natively ![windows](_media/windows.svg)

*Credit goes to **Tobias Kallmeier** for finding a way to make native builds happen on **Windows**.*

>For native builds on **Windows**, [mingw-w64](https://mingw-w64.org/) is required to be installed on your machine. In the downloads section there is a link called `MingW-W64-builds` leading to the latest release on sourceforge.

#### Generate the Makefile using cmake

In your project's root directory (e.g. `myproject`), create a new folder `build_native`. Open a terminal, navigate to `build_native` and enter:

```cmd
cmake .. -G "MinGW Makefiles"
```

[CMake](https://cmake.org/) will do it's thing, generating all kinds of files, including a new file called `Makefile`.

#### Compile using mingw32-make

Make is a tool that will build your app based on the instructions provided by the so called Makefile. It is as simple as entering a single command:

```cmd
mingw32-make
```

The first time you execute make it will take some time until it's finished. Consequent builds will be much faster!

From now on, everytime you modify your sourcecode, you merely need to call `mingw32-make` to rebuild your app.

#### Launch your app

```cmd
.\index.exe
```

## Build for the web ![linux](_media/linux.svg) ![macos](_media/macos.svg) ![windows](_media/windows.svg)

>If you are a **Windows** user, please refer to [this guide](gettingstarted/windowsaddendum.md) before you continue.

To build for the web, you first need to install Emscripten by following the [WebAssembly developer's guide](https://webassembly.org/getting-started/developers-guide/).

#### Generate the Makefile using emcmake

In your project's root directory (e.g. `myproject`), create a new folder `build_web`. Open a terminal, navigate to `build_web` and enter:

```bash
emcmake cmake ..
```

[CMake](https://cmake.org/) will do it's thing, generating all kinds of files, including a new file called `Makefile`.

#### Compile using emmake

```bash
emmake make
```

The first time you execute emmake it will take some time until it's finished. Consequent builds will be much faster!

From now on, everytime you modify your sourcecode, you merely need to call `emmake make` to rebuild your app.

#### Launch your app with emrun

```bash
emrun --no_browser --port 8080 .
```

This will launch a local web server on port 8080, which hosts your app `index.html`. To access your app, open a browser and navigate to http://localhost:8080/
