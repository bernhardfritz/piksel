<h1>piksel::DrawMode</h1>

`piksel::DrawMode` is an enum class used by [ellipseMode](reference/graphics.md#ellipseMode), [imageMode](reference/graphics.md#imageMode) and [rectMode](reference/graphics.md#rectMode) in order to change the way in which parameters given to the shape functions [ellipse](reference/graphics.md#ellipse), [image](reference/graphics.md#image) and [rect](reference/graphics.md#rect) are interpreted.

## CENTER

Interprets the first and second parameter of the corresponding shape function as the shape's center point and the third and fourth parameter as its width and height.

```cpp
piksel::DrawMode::CENTER
```

## CORNER

Interprets the first and second parameter of the corresponding shape function as the upper-left corner of the shape and the third and fourth parameter as its width and height.

```cpp
piksel::DrawMode::CORNER
```

## CORNERS

Interprets the first and second parameter of the corresponding shape function as the location of one corner, and the third and fourth parameter as the opposite corner.

```cpp
piksel::DrawMode::CORNERS
```

## RADIUS

Also uses the first and second parameter of the corresponding shape function as the shape's center point, but uses the third and fourth parameter to specify half of the shape's width and height.

```cpp
piksel::DrawMode::RADIUS
```
