# offset_ptr

## Usage
offset_ptr is expected to behave just like a regular pointer. Offsetted data will be written every time `flush()` is called or when the object is destructed.

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
