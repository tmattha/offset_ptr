namespace optr
{
  template<typename T>
  class offset_ptr
  {
  public:
    offset_ptr(T* base_, size_t offset_):
      base(base_),
      offset(offset_){

    }
  protected:
    T* base;
    size_t offset;
  };
    
} // optr
