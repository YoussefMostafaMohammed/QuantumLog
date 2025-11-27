#pragma once
#include <iostream>
#include <string_view>
#include <cstdint>
namespace Enums{

    enum class LogSinkType : unsigned char {
        CONSOLE,
        FILE,
        SOCKET
    };

    enum class SeverityLvl : unsigned char {
        CRITICAL,
        WARNING,
        INFO,
        DEBUG
    };

    enum class TelemetrySrc : unsigned char{
        CPU,
        GPU,
        RAM,
        SENSOR
    };
}


namespace Polices{

    struct CPU{
        static constexpr ::Enums::TelemetrySrc context=::Enums::TelemetrySrc::CPU;
        static constexpr std::string_view unit = "%";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl::WARNING: ::Enums::SeverityLvl::INFO;
        }
    };
    
    struct GPU{
        static constexpr ::Enums::TelemetrySrc context=::Enums::TelemetrySrc::GPU;
        static constexpr ::std::string_view unit = "%";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl::WARNING: ::Enums::SeverityLvl::INFO;
        }
    };

    struct RAM{
        static constexpr ::Enums::TelemetrySrc context=::Enums::TelemetrySrc::RAM;
        static constexpr ::std::string_view unit = "MB";
        static constexpr float WARNING=75.0f;
        static constexpr float CRITICAL=90.0f;
        static constexpr ::Enums::SeverityLvl inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl::WARNING: ::Enums::SeverityLvl::INFO;
        }
    };
    
    struct SENSOR{
        static constexpr ::Enums::TelemetrySrc context=::Enums::TelemetrySrc::SENSOR;
        static constexpr ::std::string_view unit = "°C";
        static constexpr float WARNING=70.0f;
        static constexpr float CRITICAL=85.0f;
        static constexpr ::Enums::SeverityLvl inferSeverity(float val) noexcept {
            return (val >= CRITICAL)? ::Enums::SeverityLvl::CRITICAL:(val >= WARNING)?::Enums::SeverityLvl::WARNING: ::Enums::SeverityLvl::INFO;
        }
    };
}