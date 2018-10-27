# piksel::Rng

## getInstance

```cpp
    static piksel::Rng& getInstance();
```

**Returns** a singleton random number generator.

## random

```cpp
    float random(float min, float max);
```

|Parameter|Description|
|-|-|
|`float min`|the lower bound (inclusive)|
|`float max`|the upper bound (exclusive)|

**Returns** a random floating-point number from the first argument up to (but not including) the second argument.

## randomSeed

Sets the seed value for [random](reference/rng.md#random). By default, [random](reference/rng.md#random) produces different results each time the program is run. Set the seed parameter to a constant to return the same pseudo-random numbers each time the software is run.

```cpp
    void randomSeed(unsigned int seed);
```

|Parameter|Description|
|-|-|
|`unsigned int seed`|seed value|
