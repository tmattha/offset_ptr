#include "offset_ptr_test.hpp"
#include "src/offset_ptr.hpp"
#include "NonPrimitive.hpp"

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
  raw[0] = 0x2B;
  raw[1] = 0x80;
  const size_t offset = 2;
  const uint8_t value = 0xAE;
  optr::offset_ptr<uint8_t> int_ptr(raw, offset);
  EXPECT_EQ(value, int_ptr.get());
}

TEST_F(OffsetPtrTest, it_gets_value_offset_greater_1_byte){
  //Test data is 0000 0000 00{10 1011 10}00 0000
  uint8_t raw[3];
  raw[0] = 0;
  raw[1] = 0x2B;
  raw[2] = 0x80;
  const size_t offset = 10;
  const uint8_t value = 0xAE;
  optr::offset_ptr<uint8_t> int_ptr(raw, offset);
  EXPECT_EQ(value, int_ptr.get());
}


TEST_F(OffsetPtrTest, it_sets_and_gets_the_same_no_offset){
  uint32_t value = 10;
  uint32_t other_value = 20;
  optr::offset_ptr<uint32_t> int_ptr(&value, 0);
  int_ptr.set(other_value);
  EXPECT_EQ(other_value, int_ptr.get());
}

TEST_F(OffsetPtrTest, it_flushes_on_request_no_offset){
  uint32_t value = 10;
  uint32_t other_value = 20;
  optr::offset_ptr<uint32_t> int_ptr(&value, 0);
  int_ptr.set(other_value);
  ASSERT_EQ(value, 10);
  int_ptr.flush();
  ASSERT_EQ(value, 20);
}

TEST_F(OffsetPtrTest, it_flushes_on_request_offset){
  //Test data is 00{10 1011 10}00 0000
  uint8_t raw[2];
  raw[0] = 0x00;
  raw[1] = 0x00;
  const size_t offset = 2;
  const uint8_t value = 0xAE;
  optr::offset_ptr<uint8_t> int_ptr(raw, offset);
  int_ptr.set(value);
  int_ptr.flush();
  EXPECT_EQ(raw[0], 0x2B);
  EXPECT_EQ(raw[1], 0x80);
}

TEST_F(OffsetPtrTest, it_flushes_on_multi_byte_type_request_offset){
  //Test data is 0000 {1010 0010 1011 1000} 0000 0000 0000
  uint8_t raw[3];
  raw[0] = 0x00;
  raw[1] = 0x00;
  raw[2] = 0x00;
  const size_t offset = 4;
  uint16_t value;
  
  //prevent endianess
  *(reinterpret_cast<uint8_t*>(&value)) = 0xA2;
  *(reinterpret_cast<uint8_t*>(&value)+1) = 0xB8;

  optr::offset_ptr<uint16_t> int_ptr(reinterpret_cast<uint16_t*>(raw), offset);
  int_ptr.set(value);
  int_ptr.flush();
  EXPECT_EQ(raw[0], 0x0A);
  EXPECT_EQ(raw[1], 0x2B);
  EXPECT_EQ(raw[2], 0x80);
}

TEST_F(OffsetPtrTest, it_can_get_ref_by_asterisk_op){
  uint32_t value = 10;
  optr::offset_ptr<uint32_t> int_ptr(&value, 0);
  EXPECT_EQ(*int_ptr, value);
}

TEST_F(OffsetPtrTest, it_derefs_by_arrow_op){
  NonPrimitive john;
  john.name = "John Doe";
  john.age = 45;
  optr::offset_ptr<NonPrimitive>john_ptr(&john, 0);
  EXPECT_EQ(john_ptr->name, "John Doe");
  EXPECT_EQ(john_ptr->age, 45);
}

TEST_F(OffsetPtrTest, it_flushes_on_destruction){
  uint32_t value = 10;
  uint32_t other_value = 20;
  auto* int_ptr = new  optr::offset_ptr<uint32_t>(&value, 0);
  **int_ptr = other_value;
  ASSERT_EQ(value, 10);
  delete int_ptr;
  ASSERT_EQ(value, 20);
}