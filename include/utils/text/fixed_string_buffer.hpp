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
      newline_count_ = 0;
      setp(pbase(), epptr());
    }

    int_type overflow( int_type ch ) override {
      // just reset
      m_ol();
      reset();
      return ch;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override {
        std::streamsize avail = epptr() - pptr();
        if (n > avail) n = avail;

        char* dst = pptr();
        for (std::streamsize i = 0; i < n; ++i) {
            char c = s[i];
            dst[i] = c;
            process_char(c);
        }

        pbump(static_cast<int>(n));
        return n;
    }

    const char* data() const { return pbase(); }
    std::size_t size() const { return pptr() - pbase(); }

    // iterator pointers
    const char* start() const { return pbase(); }
    const char* end() const { return pptr(); }

    // get new lines count
    int getNewLineCount() const { return newline_count_; }

    // get max line lenght
    int getMaxLineLen() const { return max_line_len_; }
  private:
    void process_char(char c) {
        if (c == '\n') {
            ++newline_count_;
            if (current_line_len_ > max_line_len_) {
                max_line_len_ = current_line_len_;
            }
            current_line_len_ = 0;
        } else {
            ++current_line_len_;
        }
    }

    overflow_listener_type m_ol;
    int newline_count_ = 0;
    int current_line_len_ = 0;
    int max_line_len_ = 0;
};


#endif
