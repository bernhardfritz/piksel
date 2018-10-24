# piksel::Font

## load

Loads an opentype font file (.otf, .ttf).

Font files should be placed in the `data` directory for them to be available for both, native and web builds.

This function is expected to be called in `setup`.

```cpp
int load(std::string filename);
```

|Parameter|Description|
|-|-|
|`std::string filename`|name of the file to be loaded|

**Returns** 1 if the font was loaded successfully, 0 otherwise.
