#include "offset_ptr_test.hpp"
#include "src/offset_ptr.hpp"

using namespace optr::tests;
TEST_F(OffsetPtrTest, it_instantiates){
  int value = 10;
  optr::offset_ptr<int> int_ptr(&value, 0);
}