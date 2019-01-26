#include "offset_ptr_test.hpp"
#include "src/offset_ptr.hpp"

using namespace optr::tests;

TEST_F(OffsetPtrTest, it_instantiates){
  uint32_t value = 10;
  optr::offset_ptr<uint32_t> int_ptr(&value, 0);
}

TEST_F(OffsetPtrTest, it_gets_value_no_offset){
  uint32_t value = 10;
  optr::offset_ptr<uint32_t> int_ptr(&value, 0);
  EXPECT_EQ(value, int_ptr.get());
}

TEST_F(OffsetPtrTest, it_gets_value_offset){
  //Test data is 00{10 1011 10}00 0000
  uint8_t raw[2];
  raw [0] = 0x2B;
  raw [1] = 0x80;
  const size_t offset = 2;
  const uint8_t value = 0xAE;
  optr::offset_ptr<uint8_t> int_ptr(raw, offset);
  EXPECT_EQ(value, int_ptr.get());
}