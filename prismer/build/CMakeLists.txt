CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

SET(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wextra -g -O3")

SET(
    PROJECT_NAME
    "Prismer"
)

SET(
    PROJECT_SOURCES
    "../src/Shapes/Circle.cpp"
    "../src/Abilities/AGather.cpp"
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
)

IF(APPLE)
    SET(
        PROJECT_LIBRARIES
        "Arya"
    )
ELSE()
    SET(
        PROJECT_LIBRARIES
        "Arya"
    )
ENDIF()

ADD_DEFINITIONS(-DPOCO_NO_AUTOMATIC_LIBS)
ADD_DEFINITIONS(-DPOCO_STATIC)

INCLUDE_DIRECTORIES( ${PROJECT_INCLUDES} )
ADD_EXECUTABLE( ${PROJECT_NAME} ${PROJECT_SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${PROJECT_LIBRARIES} )

