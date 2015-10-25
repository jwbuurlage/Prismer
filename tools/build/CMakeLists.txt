CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

INCLUDE_DIRECTORIES( )
ADD_EXECUTABLE( "md2toarya" "../md2toarya.cpp" )
TARGET_LINK_LIBRARIES( "md2toarya" )

ADD_EXECUTABLE( "generateprimitives" "../generateprimitives.cpp" )
TARGET_LINK_LIBRARIES ( "generateprimitives" )
