#ifndef FONT_UTILS_FIXED_STRING_BUFFER_HPP
#define FONT_UTILS_FIXED_STRING_BUFFER_HPP

/** for std::stringbuf */
#include <iomanip>

/** for std::function */
#include <functional>

template <class BufferType>
class fixed_string_buffer : public std::stringbuf {
  public:
    using overflow_listener_type = std::function<void(void)>;

    fixed_string_buffer( BufferType* buffer, std::size_t size, overflow_listener_type ol ) : m_ol(ol) {
      setp( buffer, buffer + size );
    }
    
    void reset() {
      auto p = pbase();
      for( auto i = 0; i < size(); ++i ) p[i] = 0;
      setp(pbase(), epptr());
    }

    int_type overflow( int_type ch ) override {
      // just reset
      m_ol();
      reset();
      return ch;
    }

    const char* data() const { return pbase(); }
    std::size_t size() const { return pptr() - pbase(); }
  private:
    overflow_listener_type m_ol;
};


#endif
