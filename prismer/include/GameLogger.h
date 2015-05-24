#pragma once
#include "common/Logger.h"

#define GameLogError    (gameLogger << Arya::L_ERROR << __func__ << "():" << __LINE__ << "  ")
#define GameLogWarning  (gameLogger << Arya::L_WARNING << __func__ << "():" << __LINE__ << "  ")
#define GameLogInfo     (gameLogger << Arya::L_INFO << __func__ << "():" << __LINE__ << "  ")
#define GameLogDebug    (gameLogger << Arya::L_DEBUG << __func__ << "():" << __LINE__ << "  ")

using Arya::endLog;

//Instance is in GameLogger.cpp
extern Arya::Logger gameLogger;
