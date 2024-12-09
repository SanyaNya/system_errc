#pragma once

#if defined(WIN32) || defined(__MINGW32__)
  #define SYSERRC_WIN_IMPL 1
  #define SYSERRC_POSIX_IMPL 0
#else
  #define SYSERRC_WIN_IMPL 0
  #define SYSERRC_POSIX_IMPL 1
#endif

#if SYSERRC_WIN_IMPL
  #include <errhandlingapi.h>
#elif SYSERRC_POSIX_IMPL
  #include <cerrno>
#endif

#include <cstdint>
#include <cstring>

namespace sys_errc
{

using ErrType =
#if SYSERRC_WIN_IMPL
  decltype(GetLastError());
#elif SYSERRC_POSIX_IMPL
  decltype(errno);
#endif

template<typename CharT, std::size_t Size>
struct cstring
{
  constexpr CharT* data() noexcept { return m_str_; }
  constexpr const CharT* data() const noexcept { return m_str_; }

  constexpr std::size_t capacity() const noexcept { return Size; }

private:
  CharT m_str_[Size];
};

struct ErrorCode
{
#if SYSERRC_WIN_IMPL
  using msg_type = cstring<wchar_t, 256>;
#elif SYSERRC_POSIX_IMPL
  using msg_type = cstring<char, 256>;
#endif

  explicit constexpr ErrorCode(ErrType v) noexcept : m_value_(v) {}

  constexpr bool operator==(ErrorCode ec) const noexcept { return m_value_ == ec.m_value_; }
  constexpr bool operator!=(ErrorCode ec) const noexcept { return m_value_ != ec.m_value_; }

  msg_type message() const noexcept
  {
    msg_type msg;

  #if SYSERRC_WIN_IMPL
	  const int f = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;
	  FormatMessageW(f, 0, m_value_, 0, msg.data(), msg.capacity(), 0);
  #elif SYSERRC_POSIX_IMPL
	  strerror_r(m_value_, msg.data(), msg.capacity());
  #endif

    return msg;
  }

  ErrType m_value_;
};

[[nodiscard]] inline ErrorCode last_error() noexcept
{
#if SYSERRC_WIN_IMPL
  return ErrorCode(GetLastError());
#elif SYSERRC_POSIX_IMPL
  return ErrorCode(errno);
#endif
}

inline void set_error(ErrType e) noexcept
{
#if SYSERRC_WIN_IMPL
  SetLastError(e);
#elif SYSERRC_POSIX_IMPL
  errno = e;
#endif
}

} //namespace sys_errc

#undef SYSERRC_WIN_IMPL
#undef SYSERRC_POSIX_IMPL
