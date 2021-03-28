include_directories(.)

set(PollSources
  DebugLog.h
  Poll.h
  Poll.cpp
  PollObj.h
  Socket.h
  Socket.cpp
  TcpSocket.h
  TcpSocket.cpp
  TcpConn.h
  TcpConn.cpp
  TcpListener.h
  TcpListener.cpp
  Buffer.h
  Buffer.cpp
  Utils.h
)

Message(STATUS ${PollSources})    
add_library(libloop STATIC ${PollSources})

    
