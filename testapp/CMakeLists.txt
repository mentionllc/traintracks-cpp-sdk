cmake_minimum_required(VERSION 2.6)

project(SERVERCONTROLLER)

SET(CORELIBS ACE boost_regex boost_filesystem boost_system boost_thread pthread crypto cryptopp enet log4cplus odbc tcmalloc bfd iberty m)

add_definitions(-g -rdynamic -fpermissive -Wno-deprecated -msse2 -mfpmath=sse)

INCLUDE_DIRECTORIES(..
../common 
$ENV{TT_ROOT}/common/util
$ENV{TT_ROOT}/common/include
/usr/include/cryptopp
/usr/local/include 
/usr/include
/usr/local/opt/log4cplus/include
)

add_executable(TestApp 
testapp.cpp
stdafx.cpp
)

target_link_libraries(TestApp 
NetworkBase
ClientAPI
Reactor  
RawStream  
SignalHandler 
RawStream
Reactor
DebugHandlers
Util  
${CORELIBS}
)

