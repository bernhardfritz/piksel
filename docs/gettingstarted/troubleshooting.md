# Troubleshooting

## Emscripten is missing dependencies

Emscripten requires `python2.7`, `python-pip` as well as `nodejs` to be installed on you system. To install these dependencies, type:

**Linux/WSL:**
```bash
sudo apt update
sudo apt install python2.7 python-pip nodejs
```

**macOS:**
```bash
brew install python@2 nodejs
```

## Emscripten commands are not recognized

To work with Emscripten you need to enter an Emscripten compiler environment. You can do so by navigating to your `emsdk` directory and typing:

**Linux/WSL:**
```bash
source ./emsdk_env.sh
```

**macOS:**
```bash
source emsdk_env.sh
```

## CMake has a lower version than is required

If you have installed CMake using a package manager, it might be that the version provided by the package manager was too low. piksel requires at least CMake 3.12. You can manually install a more current version of CMake by typing:

```bash
version=3.12
build=4
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/
./bootstrap
make -j4
sudo make install
```

## The RandR headers were not found

This error can happen during `Makefile` generation using `cmake ..` as part of the native compilation process on Linux. Some distributions do not include packages like `libgl1-mesa-dev` or `xorg-dev` per default. You can install them by typing:

**Linux:**
```bash
sudo apt update
sudo apt install libgl1-mesa-dev
sudo apt install xorg-dev
```

After cleaning your build directory, you should be able to generate a `Makefile` using `cmake ..`.
