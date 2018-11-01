<h1>piksel::Graphics</h1>

## applyMatrix

Multiplies the current matrix by the one specified through the parameters. This is a powerful operation that can perform the equivalent of translate, scale, shear and rotate all at once. You can learn more about transformation matrices on [Wikipedia](https://en.wikipedia.org/wiki/Transformation_matrix).

```cpp
void applyMatrix(float a, float b, float c, float d, float e, float f);
```

The parameters correspond to a transformation matrix in the form of:

![applymatrix](_media/applymatrix.svg)

## background

Clears the background with the specified color.

```cpp
void background(glm::vec4 color);
```

|Parameter|Description|
|-|-|
|glm::vec4 color|the color to clear the background with|

## ellipse

Draws an ellipse (oval) to the screen. An ellipse with equal width and height is a circle. By default, the first two parameters set the location, and the third and fourth parameters set the shape's width and height. The origin may be changed with the [ellipseMode](reference/graphics.md#ellipseMode) function.

```cpp
void ellipse(float x, float y, float w, float h);
```

|Parameter|Description|
|-|-|
|`float x`|x-coordinate of the ellipse|
|`float y`|y-coordinate of the ellipse|
|`float w`|width of the ellipse|
|`float h`|height of the ellipse|

## ellipseMode

Modifies the location from which ellipses are drawn by changing the way in which parameters given to [ellipse](reference/graphics.md#ellipse) are interpreted.

The default mode is [`piksel::DrawMode::CENTER`](reference/drawmode.md#CENTER), which interprets the first two parameters of [ellipse](reference/graphics.md#ellipse) as the shape's center point, while the third and fourth parameters are its width and height.

[`piksel::DrawMode::RADIUS`](reference/drawmode.md#RADIUS) also uses the first two parameters of [ellipse](reference/graphics.md#ellipse) as the shape's center point, but uses the third and fourth parameters to specify half of the shapes's width and height.

[`piksel::DrawMode::CORNER`](reference/drawmode.md#CORNER) interprets the first two parameters of [ellipse](reference/graphics.md#ellipse) as the upper-left corner of the shape, while the third and fourth parameters are its width and height.

[`piksel::DrawMode::CORNERS`](reference/drawmode.md#CORNERS) interprets the first two parameters of [ellipse](reference/graphics.md#ellipse) as the location of one corner of the ellipse's bounding box, and the third and fourth parameters as the location of the opposite corner.

The parameter must be written in ALL CAPS because C++ is a case-sensitive language.

```cpp
void ellipseMode(piksel::DrawMode ellipseMode);
```

|Parameter|Description|
|-|-|
|`piksel::DrawMode ellipseMode`|the [piksel::DrawMode](reference/drawmode.md) to use when drawing ellipses|

## fill

Sets the color used to fill shapes. For example, if you run `fill(glm::vec4(0.8f, 0.4f, 0.0f, 1.0f))`, all subsequent shapes will be filled with orange.

```cpp
void fill(glm::vec4 color);
```

|Parameter|Description|
|-|-|
|`glm::vec4 color`|the color to fill shapes with|

## image

Draws an image.

This function can be used with different numbers of parameters.

The basic version of `image` uses 5 parameters and behaves similarly to [rect](reference/graphics.md#rect).

```cpp
void image(piksel::Image& img, float x, float y, float w, float h);
```

|Parameter|Description|
|-|-|
|`piksel::Image& img`|the image to display|
|`float x`|the x-coordinate of the top-left corner of the image|
|`float y`|the y-coordinate of the top-left corner of the image|
|`float w`|the width to draw the image|
|`float h`|the height to draw the image|

The more advanced version of `image` uses 9 parameters and should be used whenever you only want to display a subimage instead of the whole image. When using this function, think of the terms "destination rectangle" (which corresponds to "dx", "dy", etc.) and "source image" (which corresponds to "sx", "sy", etc.) and remember this illustration:

![image](_media/image.png)

_The illustration above has been shamelessly repurposed from the [p5.js image reference](https://p5js.org/reference/#/p5/image)._

```cpp
void image(piksel::Image& img, float dx, float dy, float dWidth, float dHeight, float sx, float sy, float sWidth, float sHeight);
```

|Parameter|Description|
|-|-|
|`piksel::Image& img`|the image to be drawn|
|`float dx`|the x-coordinate of the destination rectangle in which to draw the source image|
|`float dy`|the y-coordinate of the destination rectangle in which to draw the source image|
|`float dWidth`|the width of the destination rectangle|
|`float dHeight`|the height of the destination rectangle|
|`float sx`|the x-coordinate of the subsection of the source image to draw into the destination rectangle|
|`float sy`|the y-coordinate of the subsection of the source image to draw into the destination rectangle|
|`float sWidth`|the width of the subsection of the source image to draw into the destination rectangle|
|`float sHeight`|the height of the subsection of the source image to draw into the destination rectangle|

## imageMode

Modifies the location from which images are drawn by changing the way in which parameters given to [image](reference/graphics.md#image) are interpreted.

The default mode is [`piksel::DrawMode::CORNER`](reference/drawmode.md#CORNER), which interprets the second and third parameters of image() as the upper-left corner of the image, while the fourth and fifth parameters are its width and height.

[`piksel::DrawMode::CORNERS`](reference/drawmode.md#CORNERS) interprets the second and third parameters of [image](reference/graphics.md#image) as the location of one corner, and the fourth and fifth parameters as the opposite corner.

[`piksel::DrawMode::CENTER`](reference/drawmode.md#CENTER) interprets the second and third parameters of [image](reference/graphics.md#image) as the image's center point, while the fourth and fifth parameters are its width and height.

[`piksel::DrawMode::RADIUS`](reference/drawmode.md#RADIUS) also uses the second and third parameters of [image](reference/graphics.md#image) as the image's center point, but uses the fourth and fifth parameters to specify half of the image's width and height.

The parameter must be written in ALL CAPS because C++ is a case-sensitive language.

```cpp
void imageMode(piksel::DrawMode imageMode);
```

|Parameter|Description|
|-|-|
|`piksel::DrawMode imageMode`|the [piksel::DrawMode](reference/drawmode.md) to use when drawing images|

## line

Draws a line (a direct path between two points) to the screen. To color a line, use the [stroke](reference/graphics.md#stroke) function. A line cannot be filled, therefore the `fill` function will not affect the color of a line. Lines are drawn with a width of one pixel by default, but this can be changed with the [strokeWeight](reference/graphics.md#strokeWeight) function.

```cpp
void line(float x1, float y1, float x2, float y2);
```

|Parameter|Description|
|-|-|
|`float x1`|x-coordinate of the first point|
|`float y1`|y-coordinate of the first point|
|`float x2`|x-coordinate of the second point|
|`float y2`|y-coordinate of the second point|

## noFill

Disables filling geometry. If both [noStroke](reference/graphics.md#noStroke) and `noFill` are called, nothing will be drawn to the screen.

```cpp
void noFill();
```

## noStroke

Disables drawing the stroke (outline). If both `noStroke` and [noFill](reference/graphics.md#noFill) are called, nothing will be drawn to the screen.

```cpp
void noStroke();
```

## noTint

Removes the current fill value for displaying images and reverts to displaying images with their original hues.

```cpp
void noTint();
```

## point

Draws a point (a single pixel) to the screen. To color a point, use the [stroke](reference/graphics.md#stroke) function. A point cannot be filled, therefore the [fill](reference/graphics.md#fill) function will not affect the color of a point.

```cpp
void point(float x, float y);
```

|Parameter|Description|
|-|-|
|`float x`|x-coordinate of the point|
|`float y`|y-coordinate of the point|

## pop

The [push](reference/graphics.md#push) function saves the current drawing style settings and transformations, while `pop` restores these settings. Note that these functions are always used together. They allow you to change the style and transformation settings and later return to what you had. When a new state is started with [push](reference/graphics.md#push), it builds on the current style and transform information. The [push](reference/graphics.md#push) and `pop` functions can be embedded to provide more control.

[push](reference/graphics.md#push) stores information related to the current transformation state and style settings controlled by the following functions: [fill](reference/graphics.md#fill), [stroke](reference/graphics.md#stroke), [strokeWeight](reference/graphics.md#strokeWeight), [tint](reference/graphics.md#tint), [imageMode](reference/graphics.md#imageMode), [rectMode](reference/graphics.md#rectMode), [ellipseMode](reference/graphics.md#ellipseMode).

```cpp
void pop();
```

## push

The `push` function saves the current drawing style settings and transformations, while [pop](reference/graphics.md#pop) restores these settings. Note that these functions are always used together. They allow you to change the style and transformation settings and later return to what you had. When a new state is started with `push`, it builds on the current style and transform information. The `push` and [pop](reference/graphics.md#pop) functions can be embedded to provide more control.

`push` stores information related to the current transformation state and style settings controlled by the following functions: [fill](reference/graphics.md#fill), [stroke](reference/graphics.md#stroke), [strokeWeight](reference/graphics.md#strokeWeight), [tint](reference/graphics.md#tint), [imageMode](reference/graphics.md#imageMode), [rectMode](reference/graphics.md#rectMode), [ellipseMode](reference/graphics.md#ellipseMode).

```cpp
void push();
```

## rect

Draws a rectangle to the screen. A rectangle is a four-sided shape with every angle at ninety degrees. By default, the first two parameters set the location of the upper-left corner, the third sets the width, and the fourth sets the height. The way these parameters are interpreted, however, may be changed with the [rectMode](reference/graphics.md#rectMode) function.

```cpp
void rect(float x, float y, float w, float h);
```

|Parameter|Description|
|-|-|
|`float x`|x-coordinate of the rectangle|
|`float y`|y-coordinate of the rectangle|
|`float w`|width of the rectangle|
|`float h`|height of the rectangle|

## rectMode

Modifies the location from which rectangles are drawn by changing the way in which parameters given to [rect](reference/graphics.md#rectMode) are interpreted.

The default mode is [`piksel::DrawMode::CORNER`](reference/drawmode.md#CORNER), which interprets the first two parameters of rect() as the upper-left corner of the shape, while the third and fourth parameters are its width and height.

[`piksel::DrawMode::CORNERS`](reference/drawmode.md#CORNERS) interprets the first two parameters of [rect](reference/graphics.md#rect) as the location of one corner, and the third and fourth parameters as the location of the opposite corner.

[`piksel::DrawMode::CENTER`](reference/drawmode.md#CENTER) interprets the first two parameters of [rect](reference/graphics.md#rect) as the shape's center point, while the third and fourth parameters are its width and height.

[`piksel::DrawMode::RADIUS`](reference/drawmode.md#RADIUS) also uses the first two parameters of [rect](reference/graphics.md#rect) as the shape's center point, but uses the third and fourth parameters to specify half of the shapes's width and height.

The parameter must be written in ALL CAPS because C++ is a case-sensitive language.

```cpp
void rectMode(piksel::DrawMode rectMode);
```

|Parameter|Description|
|-|-|
|`piksel::DrawMode rectMode`|the [piksel::DrawMode](reference/drawmode.md) to use when drawing rectangles|

## resetMatrix

Replaces the current matrix with the identity matrix.

```cpp
void resetMatrix();
```

## rotate

Rotates a shape the amount specified by the parameter. Angles have to be entered in radians.

Objects are always rotated around their relative position to the origin and positive numbers rotate objects in a clockwise direction. Transformations apply to everything that happens after and subsequent calls to the function accumulates the effect. For example, calling `g.rotate(piksel::HALF_PI)` and then `g.rotate(piksel::HALF_PI)` is the same as `g.rotate(piksel::PI)`. All tranformations are reset when [draw](reference/baseapp.md#draw) begins again.

Technically, `rotate` multiplies the current transformation matrix by a rotation matrix. This function can be further controlled by [push](reference/graphics.md#push) and [pop](reference/graphics.md#pop).

```cpp
void rotate(float rad);
```

|Parameter|Description|
|-|-|
|`float rad`|the angle of rotation, specified in radians|

## scale

Increases or decreases the size of a shape by expanding and contracting vertices. Objects always scale from their relative origin to the coordinate system. Scale values are specified as decimal percentages. For example, the function call `g.scale(2.0f)` increases the dimension of a shape by 200%.

Transformations apply to everything that happens after and subsequent calls to the function multiply the effect. For example, calling `g.scale(2.0f)` and then `g.scale(1.5f)` is the same as `g.scale(3.0f)`. All transformations are reset when [draw](reference/baseapp.md#draw) begins again.

```cpp
void scale(float x, float y);
```

|Parameter|Description|
|-|-|
|`float x`|percent to scale the object in the x-axis|
|`float y`|percent to scale the object in the y-axis|

## stroke

Sets the color used to draw lines and borders around shapes.

```cpp
void stroke(glm::vec4 color);
```

|Parameter|Description|
|-|-|
|`glm::vec4 color`|the color to outline shapes with|

## strokeWeight

Sets the width of the stroke used for lines, points, and the border around shapes. All widths are set in units of pixels.

```cpp
void strokeWeight(float weight);
```

|Parameter|Description|
|-|-|
|`float weight`|the weight (in pixels) of the stroke|

## text

Draws text to the screen. Displays the information specified in the first parameter on the screen in the position specified by the additional parameters. A default font will be used unless a font is set with the [textFont](reference/graphics.md#textFont) function and a default size will be used unless a font is set with [textSize](reference/graphics.md#textSize). Change the color of the text with the [fill](reference/graphics.md#fill) function. Change the outline of the text with the [stroke](reference/graphics.md#stroke) and [strokeWeight](reference/graphics.md#strokeWeight) functions.

```cpp
void text(std::string str, float x, float y);
```

|Parameter|Description|
|-|-|
|`std::string str`|the alphanumeric symbols to be displayed|
|`float x`|x-coordinate of text|
|`float y`|y-coordinate of text|

## textFont

Sets the current font. This font will be used in all subsequent calls to the [text](reference/graphics.md#text) function.

```cpp
void textFont(piksel::Font& font);
```

|Parameter|Description|
|-|-|
|`piksel::Font& font`|the font to be used|

## textSize

Sets the current font size. This size will be used in all subsequent calls to the [text](reference/graphics.md#text) function. Font size is measured in pixels.

```cpp
void textSize(float size);
```

|Parameter|Description|
|-|-|
|`float size`|the size of the letters in units of pixels|

## tint

Sets the fill value for displaying images.

```cpp
void tint(glm::vec4 color);
```

|Parameter|Description|
|-|-|
|`glm::vec4 color`|the color to tint images with|

## translate

Specifies an amount to displace objects within the display window. The x parameter specifies left/right translation, the y parameter specifies up/down translation.

Transformations are cumulative and apply to everything that happens after and subsequent calls to the function accumulates the effect. For example, calling `g.translate(50.0f, 0.0f)` and then `g.translate(20.0f, 0.0f)` is the same as `g.translate(70.0f, 0.0f)`. All transformations are reset when [draw](reference/baseapp.md#draw) begins again.

```cpp
void translate(float x, float y);
```

|Parameter|Description|
|-|-|
|`float x`|left/right translation|
|`float y`|up/down translation|

## triangle

A triangle is a plane created by connecting three points. The first two arguments specify the first point, the middle two arguments specify the second point, and the last two arguments specify the third point.

```cpp
void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
```

|Parameter|Description|
|-|-|
|`float x1`|x-coordinate of the first point|
|`float y1`|y-coordinate of the first point|
|`float x2`|x-coordinate of the second point|
|`float y2`|y-coordinate of the second point|
|`float x3`|x-coordinate of the third point|
|`float y3`|y-coordinate of the third point|