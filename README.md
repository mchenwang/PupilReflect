# PupilReflect
A compile time static reflection library with C++20 for Pupil Engine.

## Reference

- https://github.com/Ubpa/USRefl

## Feature

- compile time
- with code generator

## Tested environment

- Windows 10.0.19041.0
- MSVC 19.29.30146.0
- CMake 3.22.0
- C++20
- PupilReflTool v0.1

## Usage

1. download generator from https://github.com/mchenwang/PupilReflTool/releases/tag/basic.
2. decompress it and copy `exe` file to `./tool/` directory.
3. use `run.bat` to test all examples in `test` directory.

Example code:

```c++
class [[META]] TestClass {
public:
    [[META, RANGE(1, 10), STEP(1), INFO("this is a info")]]
    int testVar;
};
```

Use `META` annotate to tell the generator which class need to be reflect. And the generator will create a `PRefl::ReflData<TestClass>` class for the target classes. Then the fields information can be used by `PRefl::Refl<TestClass>::ForEachFields`.

