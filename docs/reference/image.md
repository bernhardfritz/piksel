<h1>piksel::Image</h1>

## load

Loads an image file (.jpg, .png, .tga, .bmp, .psd, .gif, .hdr, .pic).

Image files should be placed in the `data` directory for them to be available for both, native and web builds.

This function is expected to be called in `setup`.

```cpp
int load(std::string filename);
```

|Parameter|Description|
|-|-|
|`std::string filename`|name of the file to be loaded|

**Returns** 1 if the image was loaded successfully, 0 otherwise.
