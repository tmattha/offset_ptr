# offset_ptr

[![Build Status](https://travis-ci.com/tmattha/offset_ptr.svg?token=so1zazHeqvKVsxzCcNb7&branch=master)](https://travis-ci.com/tmattha/offset_ptr)

## Usage
offset_ptr is expected to behave just like a regular pointer. Offsetted data will be written every time `flush()` is called or when the object is destructed. Check out test/offset_ptr_test.cpp for more examples.

### Example
```cpp
uint8_t raw[2];
raw[0] = 0x00;
raw[1] = 0x00;
const size_t offset = 2;
const uint8_t value = 0xAE; //1010 1110
optr::offset_ptr<uint8_t> int_ptr(raw, offset);
*int_ptr = value;
//raw will look like this: 00{10 1011 10}00 0000 (0x2B80).
```
### Including
As offset_ptr is a template class all implementation is within its .hpp file. Just copy src/offset_ptr.hpp anywhere and use it. offset_ptr requires a compiler supporting at least the C++11 standard.

## Use Case
offset_ptr is aimed to streamline the process of reading data from and writing data to bit-shifted raw data. The most common use case would be working with highly fixed-layout bus messages (like CAN frames).

## Contributing
File an issue and submit a pull request if you want to.
