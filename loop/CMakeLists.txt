include_directories(.)

set(libepollsource
  Config.h
  Config.cpp
  Debug.h
  Loop.h
  Loop.cpp
  Poll.h
  Poll.cpp
  PollObj.h
  Socket.h
  Socket.cpp
  TcpSocket.h
  TcpSocket.cpp
  Utils.h
)

Message(STATUS ${libepollsource})    
add_library(libloop STATIC ${libepollsource})

    
