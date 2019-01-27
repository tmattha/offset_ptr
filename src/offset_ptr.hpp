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
      make_aligned();
    }

    /**
     *  @brief Writes temporary changes back to the offsetted data.
     */
    void flush(){
      if(!has_bit_offset()){
        *(reinterpret_cast<T*>(base)) = aligned;
        return;
      }

      auto aligned_ptr = reinterpret_cast<const uint8_t * const>(&aligned);

      const uint8_t aligned_right = full_byte >> (byte_size - bit_offset);
      const uint8_t aligned_left = full_byte << (bit_offset);

      //First byte. Keep left part of base.
      *base = (*base & left);
      *base += ((*aligned_ptr & aligned_left) >> (bit_offset));

      //Middle bytes.
      for(size_t i = 1; i < sizeof(T); i++){
        *(base + i) = ((*(aligned_ptr + (i-1)) & aligned_right) << (byte_size - bit_offset));
        *(base + i) += ((*(aligned_ptr + i) & aligned_left) >> (bit_offset));
      }

      //Last byte. Keep right part of base.
      *(base + sizeof(T)) = *(base + sizeof(T)) & right;
      *(base + sizeof(T)) += ((*(aligned_ptr + (sizeof(T)-1)) & aligned_right) << (byte_size - bit_offset));
    }

    T& operator*(){
      return aligned;
    }

    T* operator->(){
      return &aligned;
    }

    /**
     *  @brief Overwrites the value in an temporary object.
     * 
     *  Changes will only be written to the offsetted data after calling flush or by destroying the object.
     */
    void set(const T& value){
      aligned = value;
      printf("%x\n", aligned);
    }

    /**
     * @brief Gets a aligned of the object the offset_ptr points to.
     */
    T get(){
      return aligned;
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
    T aligned;

    void make_aligned(){
      //Assumes that there actually is data to extract.
      static_assert(sizeof(T) > 0, "Cannot extract data from an empty data structure.");

      if(!has_bit_offset()){
        aligned = *(reinterpret_cast<T*>(base));
        return;
      }

      auto aligned_ptr = reinterpret_cast<uint8_t * const>(&aligned);

      for(size_t i = 0; i < sizeof(T); i++){
        //Part that is already in the correct byte.
        *(aligned_ptr + i) = (*(base + i) & right) << bit_offset;
        
        //Part from the next byte that gets transferred.
        *(aligned_ptr + i) += (*(base + i + 1) & left) >> (byte_size - bit_offset);
      }
    }

    bool has_bit_offset(){
      return bit_offset;
    }
  }; 
  
} // optr

#endif // OFFSET_PTR

