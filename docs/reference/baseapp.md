<h1>piksel::BaseApp</h1>

This base class is intended to be inherited by a derived class provided by you.

## BaseApp

When deriving `BaseApp` you can specify a constructor other than the default constructor to specify properties like width, height, title and whether to use fullscreen. Otherwise width, heigh, title and fullscreen default to 100, 100, "piksel" and false respectively.

```cpp
BaseApp(int width, int height, std::string title, fullscreen = false)
BaseApp(int width, int height, fullscreen = false)
BaseApp(std::string title, fullscreen = false)
BaseApp(fullscreen = false)
```

## draw

Called directly after `setup`, the `draw` function continuously executes the lines of code contained inside its block until the program is stopped.

```cpp
void draw(piksel::Graphics& g);
```

|Parameter|Description|
|-|-|
|`piksel::Graphics& g`|this reference enables access to all graphics-related functions|

## keyPressed

The `keyPressed` function is called every time a key is pressed.

```cpp
void keyPressed(int key);
```

|Parameter|Description|
|-|-|
|`int key`|the keycode|

## keyReleased

The `keyReleased` function is called every time a key is released.

```cpp
void keyReleased(int key)
```

|Parameter|Description|
|-|-|
|`int key`|the keycode|

## millis

Returns the number of milliseconds (thousandths of a second) since starting the program. This information is often used for timing events and animation sequences.

```cpp
int millis();
```

**Returns** `int` the number of milliseconds since starting the program

## mouseMoved

The `mouseMoved` function is called every time the mouse moves.

```cpp
void mouseMoved(int x, int y);
```

|Parameter|Description|
|-|-|
|`int x`|the mouse x coordinate|
|`int y`|the mouse y coordinate|

## mousePressed

The `mousePressed` function is called every time a mouse button is pressed.

```cpp
void mousePressed(int button);
```

|Parameter|Description|
|-|-|
|`int button`|the mouse button identifier|

## mouseReleased

The `mouseReleased` function is called every time a mouse button is released.

```cpp
void mouseReleased(int button);
```

|Parameter|Description|
|-|-|
|`int button`|the mouse button identifier|

## mouseWheel

The `mouseWheel` function is called every time a vertical mouse event is detected, either triggered by an actual mouse or by a touchpad.

```cpp
void mouseWheel(int delta);
```

|Parameter|Description|
|-|-|
|`int delta`|the amount the mouse wheel has scrolled|

## setup

The `setup` function is called once when the program starts. It's used to load media such as images and fonts or to perform any other kind of expensive initial operation.

```cpp
void setup();
```

## start

The `start` function starts the app.

```cpp
void start()
```

## width

Member variable that stores the width of the app.

```cpp
width
```

## height

Member variable that stores the height of the app.

```cpp
height
```
