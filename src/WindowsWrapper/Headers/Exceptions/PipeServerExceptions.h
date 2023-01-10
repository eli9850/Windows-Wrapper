#pragma once

#include "BaseException.h"

CREATE_EXCEPTION(PipeServerException, BaseException);

CREATE_EXCEPTION(PipeServerOverlappedException, PipeServerException);

CREATE_EXCEPTION(PipeServerConnectException, PipeServerException);

CREATE_EXCEPTION(PipeServerDisconnectException, PipeServerException);

CREATE_EXCEPTION(PipeServerWriteException, PipeServerException);

CREATE_EXCEPTION(PipeServerReadException, PipeServerException);

CREATE_EXCEPTION(PipeServerPeekException, PipeServerException);
