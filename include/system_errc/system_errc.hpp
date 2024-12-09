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

namespace sys_errc
{

using ErrType =
#if SYSERRC_WIN_IMPL
  decltype(GetLastError());
#elif SYSERRC_POSIX_IMPL
  decltype(errno);
#endif

struct ErrorCode
{
  explicit constexpr ErrorCode(ErrType v) noexcept : m_value_(v) {}

  constexpr bool operator==(ErrorCode ec) const noexcept { return m_value_ == ec.m_value_; }
  constexpr bool operator!=(ErrorCode ec) const noexcept { return m_value_ != ec.m_value_; }

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
