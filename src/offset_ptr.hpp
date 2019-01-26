#if !defined(OFFSET_PTR)
#define OFFSET_PTR

#include <vector>

namespace optr
{
  template<typename T>
  class offset_ptr
  {
  public:
    offset_ptr(T* ptr, size_t offset)
    {
      static_assert(1 == sizeof(uint8_t));
      
      byte_offset = offset / byte_size;
      bit_offset = offset % byte_size;
      base = reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(ptr) + byte_offset);
      make_copy();
    }

    T get(){
      return copy;
    }
  protected:
    T* base;
    size_t offset;
  private:
    static constexpr unsigned int byte_size = 8;
    size_t byte_offset;
    size_t bit_offset;
    T copy;

    void make_copy(){
      std::vector<uint8_t> data_copy (reinterpret_cast<uint8_t*>(base), reinterpret_cast<uint8_t*>(base) + sizeof(T)+1);
      
      //Assumes that there actually is data to extract.
      static_assert(sizeof(T) > 0, "Cannot extract data from an empty data structure.");

      const uint8_t full_byte = 0xFF;

      //first bit_offset bits of uint8_t
      const uint8_t left = full_byte << (byte_size - bit_offset);
      //all other bits
      const uint8_t right = full_byte >> bit_offset;

      //Anything within offset is uninteresting.
      data_copy[0] &= right;

      for(size_t i = 0; i < sizeof(T); i++){
        //Part that is already in the correct byte.
        data_copy[i] = (data_copy[i] & right) << bit_offset;
        //Part from the next byte that gets transferred.
        data_copy[i] += (data_copy[i+1] & left) >> (byte_size - bit_offset);
      }

      //Truncate last bit.
      data_copy.resize(sizeof(T));

      copy = *reinterpret_cast<T*>(data_copy.data());
    }
  };
    
} // optr

#endif // OFFSET_PTR

