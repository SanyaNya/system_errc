#pragma once

#if defined(WIN32) || defined(__MINGW32__)
  #define SYSERRC_WIN_IMPL 1
  #define SYSERRC_POSIX_IMPL 0
#else
  #define SYSERRC_WIN_IMPL 0
  #define SYSERRC_POSIX_IMPL 1
#endif

#define DETAIL_PP_EMPTY(...)
#define DETAIL_PP_NON_EMPTY(...) __VA_ARGS__

#define DETAIL_PP_IFE0(...) DETAIL_PP_NON_EMPTY
#define DETAIL_PP_IFE1(...) __VA_ARGS__ DETAIL_PP_EMPTY
#define DETAIL_PP_IFE(COND) DETAIL_PP_IFE ## COND
#define PP_IFE(COND) DETAIL_PP_IFE(COND)

#if SYSERRC_WIN_IMPL
  #include <WinError.h>
  #include <errhandlingapi.h>
#elif SYSERRC_POSIX_IMPL
  #include <cerrno>
#endif

#include <cstddef>
#include <cstring>
#include <type_traits>

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
#if SYSERRC_WIN_IMPL
  using char_type = wchar_t;
#elif SYSERRC_POSIX_IMPL
  using char_type = char;
#endif

  constexpr ErrorCode(ErrType v) noexcept : m_value_(v) {}

  constexpr bool operator==(ErrorCode ec) const noexcept { return m_value_ == ec.m_value_; }
  constexpr bool operator!=(ErrorCode ec) const noexcept { return m_value_ != ec.m_value_; }

  const char_type* message() const noexcept
  {
    static thread_local char_type msg[256];

  #if SYSERRC_WIN_IMPL
	  const int f = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;
	  FormatMessageW(f, 0, m_value_, 0, msg, sizeof(msg), 0);
  #elif SYSERRC_POSIX_IMPL
	  auto r = strerror_r(m_value_, msg, sizeof(msg));
    if constexpr(std::is_same_v<decltype(r), char*>) //GNU variant
      return r;
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

//Errors from std::errc
namespace common
{
  namespace sockets
  {
    enum
    {
      address_family_not_supported       = PP_IFE(SYSERRC_WIN_IMPL)(WSAEAFNOSUPPORT)(EAFNOSUPPORT),
      address_in_use                     = PP_IFE(SYSERRC_WIN_IMPL)(WSAEADDRINUSE)(EADDRINUSE),
      address_not_available              = PP_IFE(SYSERRC_WIN_IMPL)(WSAEADDRNOTAVAIL)(EADDRNOTAVAIL),
      already_connected                  = PP_IFE(SYSERRC_WIN_IMPL)(WSAEISCONN)(EISCONN),
      bad_address                        = PP_IFE(SYSERRC_WIN_IMPL)(WSAEFAULT)(EFAULT),
      bad_file_descriptor                = PP_IFE(SYSERRC_WIN_IMPL)(WSAEBADF)(EBADF),
      connection_aborted                 = PP_IFE(SYSERRC_WIN_IMPL)(WSAECONNABORTED)(ECONNABORTED),
      connection_already_in_progress     = PP_IFE(SYSERRC_WIN_IMPL)(WSAEALREADY)(EALREADY),
      connection_refused                 = PP_IFE(SYSERRC_WIN_IMPL)(WSAECONNREFUSED)(ECONNREFUSED),
      connection_reset                   = PP_IFE(SYSERRC_WIN_IMPL)(WSAECONNRESET)(ECONNRESET),
      destination_address_required       = PP_IFE(SYSERRC_WIN_IMPL)(WSAEDESTADDRREQ)(EDESTADDRREQ),
      filename_too_long                  = PP_IFE(SYSERRC_WIN_IMPL)(WSAENAMETOOLONG)(ENAMETOOLONG),
      host_unreachable                   = PP_IFE(SYSERRC_WIN_IMPL)(WSAEHOSTUNREACH)(EHOSTUNREACH),
      interrupted                        = PP_IFE(SYSERRC_WIN_IMPL)(WSAEINTR)(EINTR),
      invalid_argument                   = PP_IFE(SYSERRC_WIN_IMPL)(WSAEINVAL)(EINVAL),
      message_size                       = PP_IFE(SYSERRC_WIN_IMPL)(WSAEMSGSIZE)(EMSGSIZE),
      network_down                       = PP_IFE(SYSERRC_WIN_IMPL)(WSAENETDOWN)(ENETDOWN),
      network_reset                      = PP_IFE(SYSERRC_WIN_IMPL)(WSAENETRESET)(ENETRESET),
      network_unreachable                = PP_IFE(SYSERRC_WIN_IMPL)(WSAENETUNREACH)(ENETUNREACH),
      no_buffer_space                    = PP_IFE(SYSERRC_WIN_IMPL)(WSAENOBUFS)(ENOBUFS),
      no_protocol_option                 = PP_IFE(SYSERRC_WIN_IMPL)(WSAENOPROTOOPT)(ENOPROTOOPT),
      not_a_socket                       = PP_IFE(SYSERRC_WIN_IMPL)(WSAENOTSOCK)(ENOTSOCK),
      not_connected                      = PP_IFE(SYSERRC_WIN_IMPL)(WSAENOTCONN)(ENOTCONN),
      operation_in_progress              = PP_IFE(SYSERRC_WIN_IMPL)(WSAEINPROGRESS)(EINPROGRESS),
      operation_not_supported            = PP_IFE(SYSERRC_WIN_IMPL)(WSAEOPNOTSUPP)(EOPNOTSUPP),
      operation_would_block              = PP_IFE(SYSERRC_WIN_IMPL)(WSAEWOULDBLOCK)(EWOULDBLOCK),
      permission_denied                  = PP_IFE(SYSERRC_WIN_IMPL)(WSAEACCES)(EACCES),
      protocol_not_supported             = PP_IFE(SYSERRC_WIN_IMPL)(WSAEPROTONOSUPPORT)(EPROTONOSUPPORT),
      timed_out                          = PP_IFE(SYSERRC_WIN_IMPL)(WSAETIMEDOUT)(ETIMEDOUT),
      too_many_files_open                = PP_IFE(SYSERRC_WIN_IMPL)(WSAEMFILE)(EMFILE),
      wrong_protocol_type                = PP_IFE(SYSERRC_WIN_IMPL)(WSAEPROTOTYPE)(EPROTOTYPE)
    };
  } //namespace sockets

  //TODO other errors

} //namespace common

} //namespace sys_errc

#undef SYSERRC_WIN_IMPL
#undef SYSERRC_POSIX_IMPL

#undef DETAIL_PP_EMPTY
#undef DETAIL_PP_NON_EMPTY
#undef DETAIL_PP_IFE0
#undef DETAIL_PP_IFE1
#undef DETAIL_PP_IFE
#undef PP_IFE
