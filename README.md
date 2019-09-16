# VoltNet
  VoltNet is aimed at being a lightweight TCP networking library that thinly
  wraps around UNIX TCP sockets and should be both compatible with *NIX-based
  and Windows systems using Cygwin and MSYS2.

  This library is very early in development and therefore isn't industry
  ready.

  VoltNet aims to provide a message-layer over the top of TCP's stream-based
  connections.

# Planned
- Add unit tests for endianness handler
- Write up user-manual
- Comment all source code
- Provide examples
- More tests
- Ensure full compatibility with Windows on Cygwin and MSYS2
- Code coverage generation
- Add direct support for WIN32 and Win sockets
- Add network serialization support for floats/doubles.
