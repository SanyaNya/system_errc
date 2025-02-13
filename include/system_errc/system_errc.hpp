#pragma once

#include <hpp/define.hpp>

#if HPP_WIN_IMPL
  #include <WinError.h>
  #include <errhandlingapi.h>
#elif HPP_POSIX_IMPL
  #include <cerrno>
#endif

#include <cstddef>
#include <cstring>
#include <type_traits>

namespace sys_errc
{

using ErrType =
#if HPP_WIN_IMPL
  decltype(GetLastError());
#elif HPP_POSIX_IMPL
  decltype(errno);
#endif

struct ErrorCode
{
#if HPP_WIN_IMPL
  using char_type = wchar_t;
#elif HPP_POSIX_IMPL
  using char_type = char;
#endif

  constexpr ErrorCode(ErrType v) noexcept : m_value_(v) {}

  constexpr bool operator==(ErrorCode ec) const noexcept { return m_value_ == ec.m_value_; }
  constexpr bool operator!=(ErrorCode ec) const noexcept { return m_value_ != ec.m_value_; }

  const char_type* message() const noexcept
  {
    static thread_local char_type msg[256];

  #if HPP_WIN_IMPL
	  const int f = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;
	  FormatMessageW(f, 0, m_value_, 0, msg, sizeof(msg), 0);
  #elif HPP_POSIX_IMPL
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
#if HPP_WIN_IMPL
  return ErrorCode(GetLastError());
#elif HPP_POSIX_IMPL
  return ErrorCode(errno);
#endif
}

inline void set_error(ErrType e) noexcept
{
#if HPP_WIN_IMPL
  SetLastError(e);
#elif HPP_POSIX_IMPL
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
      address_family_not_supported       = HPP_IFE(HPP_WIN_IMPL)(WSAEAFNOSUPPORT)(EAFNOSUPPORT),
      address_in_use                     = HPP_IFE(HPP_WIN_IMPL)(WSAEADDRINUSE)(EADDRINUSE),
      address_not_available              = HPP_IFE(HPP_WIN_IMPL)(WSAEADDRNOTAVAIL)(EADDRNOTAVAIL),
      already_connected                  = HPP_IFE(HPP_WIN_IMPL)(WSAEISCONN)(EISCONN),
      bad_address                        = HPP_IFE(HPP_WIN_IMPL)(WSAEFAULT)(EFAULT),
      bad_file_descriptor                = HPP_IFE(HPP_WIN_IMPL)(WSAEBADF)(EBADF),
      connection_aborted                 = HPP_IFE(HPP_WIN_IMPL)(WSAECONNABORTED)(ECONNABORTED),
      connection_already_in_progress     = HPP_IFE(HPP_WIN_IMPL)(WSAEALREADY)(EALREADY),
      connection_refused                 = HPP_IFE(HPP_WIN_IMPL)(WSAECONNREFUSED)(ECONNREFUSED),
      connection_reset                   = HPP_IFE(HPP_WIN_IMPL)(WSAECONNRESET)(ECONNRESET),
      destination_address_required       = HPP_IFE(HPP_WIN_IMPL)(WSAEDESTADDRREQ)(EDESTADDRREQ),
      filename_too_long                  = HPP_IFE(HPP_WIN_IMPL)(WSAENAMETOOLONG)(ENAMETOOLONG),
      host_unreachable                   = HPP_IFE(HPP_WIN_IMPL)(WSAEHOSTUNREACH)(EHOSTUNREACH),
      interrupted                        = HPP_IFE(HPP_WIN_IMPL)(WSAEINTR)(EINTR),
      invalid_argument                   = HPP_IFE(HPP_WIN_IMPL)(WSAEINVAL)(EINVAL),
      message_size                       = HPP_IFE(HPP_WIN_IMPL)(WSAEMSGSIZE)(EMSGSIZE),
      network_down                       = HPP_IFE(HPP_WIN_IMPL)(WSAENETDOWN)(ENETDOWN),
      network_reset                      = HPP_IFE(HPP_WIN_IMPL)(WSAENETRESET)(ENETRESET),
      network_unreachable                = HPP_IFE(HPP_WIN_IMPL)(WSAENETUNREACH)(ENETUNREACH),
      no_buffer_space                    = HPP_IFE(HPP_WIN_IMPL)(WSAENOBUFS)(ENOBUFS),
      no_protocol_option                 = HPP_IFE(HPP_WIN_IMPL)(WSAENOPROTOOPT)(ENOPROTOOPT),
      not_a_socket                       = HPP_IFE(HPP_WIN_IMPL)(WSAENOTSOCK)(ENOTSOCK),
      not_connected                      = HPP_IFE(HPP_WIN_IMPL)(WSAENOTCONN)(ENOTCONN),
      operation_in_progress              = HPP_IFE(HPP_WIN_IMPL)(WSAEINPROGRESS)(EINPROGRESS),
      operation_not_supported            = HPP_IFE(HPP_WIN_IMPL)(WSAEOPNOTSUPP)(EOPNOTSUPP),
      operation_would_block              = HPP_IFE(HPP_WIN_IMPL)(WSAEWOULDBLOCK)(EWOULDBLOCK),
      permission_denied                  = HPP_IFE(HPP_WIN_IMPL)(WSAEACCES)(EACCES),
      protocol_not_supported             = HPP_IFE(HPP_WIN_IMPL)(WSAEPROTONOSUPPORT)(EPROTONOSUPPORT),
      timed_out                          = HPP_IFE(HPP_WIN_IMPL)(WSAETIMEDOUT)(ETIMEDOUT),
      too_many_files_open                = HPP_IFE(HPP_WIN_IMPL)(WSAEMFILE)(EMFILE),
      wrong_protocol_type                = HPP_IFE(HPP_WIN_IMPL)(WSAEPROTOTYPE)(EPROTOTYPE)
    };
  } //namespace sockets

  //TODO other errors

} //namespace common

} //namespace sys_errc
