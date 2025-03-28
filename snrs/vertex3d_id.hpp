#ifndef SNRS_VERTEX3D_ID_HPP
#define SNRS_VERTEX3D_ID_HPP

// Standard Library:
#include <iostream>
#include <limits>
#include <cstdint>

namespace snrs {
  
  class vertex3d_id
  {
  public:
  
    static constexpr uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
    static constexpr uint32_t ANY_INDEX     = std::numeric_limits<uint32_t>::max() - 1;
 
    vertex3d_id() = default;

    vertex3d_id(uint32_t i_, uint32_t j_, uint32_t k_);
  
    bool is_valid() const;

    void invalidate();

    bool match(uint32_t i_, uint32_t j_, uint32_t k_) const;
  
    friend std::ostream & operator<<(std::ostream & out_, const vertex3d_id & vid_);
 
    bool operator==(const vertex3d_id & vid_) const;
  
    bool operator<(const vertex3d_id & vid_) const;

    void set(uint32_t i_, uint32_t j_, uint32_t k_);
  
    uint32_t get_i() const;

    uint32_t get_j() const;
 
    uint32_t get_k() const;

    void store(std::ostream & out_) const;

    void load(std::istream & in_);  
 
  private:
  
    uint32_t _i_ = INVALID_INDEX; // X-axis
    uint32_t _j_ = INVALID_INDEX; // Y-axis
    uint32_t _k_ = INVALID_INDEX; // Z-axis
   
  };

} // namespace snrs
 
#endif // SNRS_VERTEX3D_ID_HPP
