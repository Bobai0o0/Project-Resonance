#pragma once
#include "VectorHaptics.h"
/**
 * @defgroup convert Converters
 * @ingroup tools
 * @brief Convert Audio
 * You can add a converter as argument to the StreamCopy::copy() or better use
 * is with a ConverterStream.
 */
#define INT24_MAX 0x7FFFFF
namespace VH
{

  class int24_t  {
    public:
     int24_t() {
       value = 0;
     }
   
     int24_t(void *ptr) {
       memcpy(&value, ptr, 4);
     }
   
     int24_t(const int16_t in) {
       set(in) ;
     }
   
     int24_t(const int32_t in) {
       set(in);
     }
   
     int24_t(const int64_t in) {
       set((int32_t)in) ;
     }
   
     int24_t(const int24_t &in) {
       value = in.value;
     }
   
     int24_t(const float in) {
       set((int32_t)in);
     }
   
   #if defined(USE_INT24_FROM_INT) 
   
     int24_t(const int in) {
       set(in);
     }
   
   #endif
   
     /// values are clipped and shifted by 1 byte
     inline void set(const int32_t &in) {
       if (in>INT24_MAX){
         value = INT24_MAX << 8;
       } else if (in<-INT24_MAX){
         value = -(INT24_MAX << 8);
       } else {
         value = in << 8;
       }
     }
   
     int24_t& operator=(const int24_t& other){
       value = other.value;
       return *this;
     }
   
     int24_t& operator=(const float& other){
       set(((int32_t)other));
       return *this;
     }
   
     operator int()  {
       return toInt();
     }  
   
     explicit operator float()  {
       return toInt();
     }  
   
     explicit operator int64_t()   {
       return toInt();
     }  
   
     int24_t& operator +=(int32_t valueA){
       int32_t temp = toInt();
       temp += valueA;
       set(temp);
       return *this;
     }
   
     int24_t& operator -=(int32_t valueA){
       int32_t temp = toInt();
       temp -= valueA;
       set(temp);
       return *this;
     }
   
     /// Standard Conversion to Int 
     int toInt() const {
       return value >> 8;
     }  
   
     /// convert to float
     float toFloat() const { return (float)toInt(); }
   
     /// provides value between -32767 and 32767
     int16_t scale16() const {
       return static_cast<int16_t>(toInt() >> 8) ;
     }
   
     /// provides value between -2,147,483,647 and 2,147,483,647
     int32_t scale32() const {
       return static_cast<int32_t>(toInt() << 8);
     }
   
     /// provides value between -1.0 and 1.0
     float scaleFloat() const { return toFloat() / INT24_MAX; }
   
     void setAndScale16(int16_t i16) {
       value = i16;
       value = value << 16;
     }
     
     int16_t getAndScale16() {
       return static_cast<int16_t>(value >> 16);
     }
   
    private:
     // store 24 bit value shifted by 1 byte to the left
     int32_t value;
   };
   

  /**
   * @brief Abstract Base class for Converters
   * A converter is processing the data in the indicated array
   * @ingroup convert
   * @author Phil Schatzmann
   * @copyright GPLv3
   * @tparam T
   */
  class BaseConverter
  {
  public:
    BaseConverter() = default;
    BaseConverter(BaseConverter const &) = delete;
    virtual ~BaseConverter() = default;

    BaseConverter &operator=(BaseConverter const &) = delete;

    virtual size_t convert(uint8_t *src, size_t size) = 0;
  };

  /**
   * @brief Provides reduced sampling rates
   * @ingroup convert
   */
  template <typename T>
  class DecimateT : public BaseConverter
  {
  public:
    DecimateT(int factor, int channels)
    {
      setChannels(channels);
      setFactor(factor);
      count = 0; // Initialize count to 0
    }

    /// Defines the number of channels
    void setChannels(int channels) { this->channels = channels; }

    /// Sets the factor: e.g. with 4 we keep every fourth sample
    void setFactor(int factor) { this->factor = factor; }

    size_t convert(uint8_t *src, size_t size) { return convert(src, src, size); }

    size_t convert(uint8_t *target, uint8_t *src, size_t size)
    {

      if (size % (sizeof(T) * channels) > 0)
      {
        //LOGE("Buffer size %d is not a multiple of the number of channels %d", (int)size, channels);
        return 0;
      }

      int frame_count = size / (sizeof(T) * channels);
      T *p_target = (T *)target;
      T *p_source = (T *)src;
      size_t result_size = 0;

      for (int i = 0; i < frame_count; i++)
      {
        if (++count == factor)
        {
          count = 0;
          // Only keep every "factor" samples
          for (int ch = 0; ch < channels; ch++)
          {
            *p_target++ = p_source[i * channels + ch]; // Corrected indexing
            result_size += sizeof(T);
          }
        }
      }

      //LOGD("decimate %d: %d -> %d bytes", factor, (int)size, (int)result_size);
      return result_size;
    }

    operator bool() { return factor > 1; }

  protected:
    int channels = 2;
    int factor = 1;
    uint16_t count;
  };

  /**
   * @brief Provides a reduced sampling rate by taking a sample at every factor location (ingoring factor-1 samples)
   * @ingroup convert
   */

  class Decimate : public BaseConverter
  {
  public:
    Decimate() = default;
    Decimate(int factor, int channels, int bits_per_sample)
    {
      setFactor(factor);
      setChannels(channels);
      setBits(bits_per_sample);
    }
    /// Defines the number of channels
    void setChannels(int channels) { this->channels = channels; }
    void setBits(int bits) { this->bits = bits; }
    /// Sets the factor: e.g. with 4 we keep every forth sample
    void setFactor(int factor) { this->factor = factor; }

    size_t convert(uint8_t *src, size_t size) { return convert(src, src, size); }
    size_t convert(uint8_t *target, uint8_t *src, size_t size)
    {
      switch (bits)
      {
      case 8:
      {
        DecimateT<int8_t> dec8(factor, channels);
        return dec8.convert(target, src, size);
      }
      case 16:
      {
        DecimateT<int16_t> dec16(factor, channels);
        return dec16.convert(target, src, size);
      }
      case 24:
      {
        DecimateT<int24_t> dec24(factor, channels);
        return dec24.convert(target, src, size);
      }
      case 32:
      {
        DecimateT<int32_t> dec32(factor, channels);
        return dec32.convert(target, src, size);
      }
      }
      return 0;
    }

    operator bool() { return factor > 1; };

  protected:
    int channels = 2;
    int bits = 16;
    int factor = 1;
  };

  /**
   * @brief We reduce the number of samples in a datastream by summing (binning) or averaging.
   * This will result in the same number of channels but binSize times less samples.
   * If Average is true the sum is divided by binSize.
   * @author Urs Utzinger
   * @ingroup convert
   * @tparam T
   */

  // Helper template to define the integer type for the summation based on input
  // data type T
  template <typename T>
  struct AppropriateSumType
  {
    using type = T;
  };

  template <>
  struct AppropriateSumType<int8_t>
  {
    using type = int16_t;
  };

  template <>
  struct AppropriateSumType<int16_t>
  {
    using type = int32_t;
  };

  template <>
  struct AppropriateSumType<int24_t>
  {
    using type = int32_t; // Assuming int24_t is a custom 24-bit integer type
  };

  template <>
  struct AppropriateSumType<int32_t>
  {
    using type = int64_t;
  };
}