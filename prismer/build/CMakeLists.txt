CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

SET(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wextra -g -O3")

SET(
    PROJECT_NAME
    "Prismer"
)

SET(
    PROJECT_SOURCES
    "../src/Game.cpp"
    "../src/GameLogger.cpp"
    "../src/GameSessionInput.cpp"
    "../src/GameSession.cpp"
    "../src/GameSessionClient.cpp"
    "../src/Unit.cpp"
    "../src/main.cpp"
)

SET(
    PROJECT_INCLUDES
    "../../include"
    "../include"
    # "/usr/include/lua5.1"
)

IF(APPLE)
    SET(
        PROJECT_LIBRARIES
        "Arya"
#       "PocoNet"
#       "PocoFoundation"
#       "pthread"
#       "luabind"
#       "lua"
    )
ELSE()
    SET(
        PROJECT_LIBRARIES
        "Arya"
#       "PocoNet"
#       "PocoFoundation"
#       "pthread"
#       "luabind"
#       "lua5.1"
    )
ENDIF()

ADD_DEFINITIONS(-DPOCO_NO_AUTOMATIC_LIBS)
ADD_DEFINITIONS(-DPOCO_STATIC)

INCLUDE_DIRECTORIES( ${PROJECT_INCLUDES} )
ADD_EXECUTABLE( ${PROJECT_NAME} ${PROJECT_SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${PROJECT_LIBRARIES} )

