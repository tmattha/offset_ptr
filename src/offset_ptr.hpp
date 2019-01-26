#if !defined(OFFSET_PTR)
#define OFFSET_PTR

#include <vector>

namespace optr
{
  template<typename T>
  class offset_ptr
  {
  public:
    /**
     *  @brief Constructs an pointer with an offset from its base address and offset.
     * 
     *  @param ptr The pointer to the base address of the object. If offset is greater than 8, bits within the offset will not be acessed.
     *  @param offset The offset in bits before the actual object starts.
     */
    offset_ptr(T* ptr, size_t offset)
    {
      static_assert(1 == sizeof(uint8_t));
      
      byte_offset = offset / byte_size;
      bit_offset = offset % byte_size;
      base = reinterpret_cast<uint8_t*>(ptr) + byte_offset;
      //first bit_offset bits of uint8_t
      left = full_byte << (byte_size - bit_offset);
      //all other bits
      right = full_byte >> bit_offset;
      make_copy();
    }

    /**
     *  @brief Writes temporary changes back to the offsetted data.
     */
    void flush(){
      auto copy_ptr = reinterpret_cast<uint8_t*>(&copy);
      const uint8_t copy_right = full_byte >> (byte_size + bit_offset);
      const uint8_t copy_left = full_byte << (bit_offset);
      printf("copy - left: %#02x right: %#02x\n", copy_left, copy_right);

      *base = (*base & left) + ((*copy_ptr & copy_left) >> bit_offset);
      printf("base[0] = %#02x\n", *base);
      for(size_t i = 1; i < sizeof(T); i++){
        *(base + i) = ((*(copy_ptr + (i-1)) & copy_right) << bit_offset);
        *(base + i) += ((*(copy_ptr + i) & copy_left) >> (byte_size - bit_offset));
        printf("base[%2lu] = %#02x\n", i, *(base + i));
      }

      if(extra_byte()){
        *(base + sizeof(T) + 1) = (*(copy_ptr + sizeof(T)) & copy_right) + (*(base + sizeof(T) + 1) & right);
        printf("base[%2lu] = %#02x\n", (sizeof(T) + 1), *(base + sizeof(T) + 1));
      }
    }

    /**
     *  @brief Overwrites the value in an temporary object.
     * 
     *  Changes will only be written to the offsetted data after calling flush or by destroying the object.
     */
    void set(const T& value){
      copy = value;
    }

    /**
     * @brief Gets a copy of the object the offset_ptr points to.
     */
    T get(){
      return copy;
    }
  protected:
    uint8_t* base;
    size_t offset;
  private:
    static constexpr uint8_t full_byte = 0xFF;
    static constexpr unsigned int byte_size = 8;
    size_t byte_offset;
    size_t bit_offset;
    uint8_t left;
    uint8_t right;
    T copy;

    void make_copy(){
      
      std::vector<uint8_t> data_copy (base, base + sizeof(T)+extra_byte());
      
      //Assumes that there actually is data to extract.
      static_assert(sizeof(T) > 0, "Cannot extract data from an empty data structure.");

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

    bool extra_byte(){
      if(bit_offset == 0){
        return 0;
      }else{
        return 1;
      }
    }
  }; 
  
} // optr

#endif // OFFSET_PTR

