#pragma once
#include <iostream>
#include <string_view>

namespace Enums{

    enum class LogSinkType_enum : unsigned char {
        CONSOLE,
        FILE,
        SOCKET
    };

    enum class SeverityLvl_enum : unsigned char {
        CRITICAL,
        WARNING,
        INFO
    };

    enum class TelemetrySrc_enum : unsigned char{
        CPU,
        GPU,
        RAM
    };
}


namespace Polices{

    struct CPU{
        static constexpr ::Enums::TelemetrySrc_enum context=::Enums::TelemetrySrc_enum::CPU;
        static constexpr std::string_view unit = "%";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl_enum inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl_enum::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl_enum::WARNING: ::Enums::SeverityLvl_enum::INFO;
        }
    };
    
    struct GPU{
        static constexpr ::Enums::TelemetrySrc_enum context=::Enums::TelemetrySrc_enum::GPU;
        static constexpr ::std::string_view unit = "%";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl_enum inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl_enum::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl_enum::WARNING: ::Enums::SeverityLvl_enum::INFO;
        }
    };

    struct RAM{
        static constexpr ::Enums::TelemetrySrc_enum context=::Enums::TelemetrySrc_enum::RAM;
        static constexpr ::std::string_view unit = "MB";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl_enum inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl_enum::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl_enum::WARNING: ::Enums::SeverityLvl_enum::INFO;
        }
    };
}
