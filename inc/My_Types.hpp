#pragma once
#include <iostream>

enum class LogSinkType_enum : unsigned char {
    CONSOLE,
    FILE,
    SOCKET
}

enum class SeverityLvl_enum : unsigned char {
    CRITICAL,
    WARNING,
    INFO
}

enum class TelemetrySrc_enum : unsigned char{
    CPU,
    GPU,
    RAM
}