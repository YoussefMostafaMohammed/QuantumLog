#include "LogMessage.hpp"
#include <chrono>
#include <sstream>    
#include <iomanip>      
#include <ctime>      
#include "magic_enum.hpp" 
#include "telemetry.pb.h" 

// --- Name ---
std::string LogMessage::getName() const {
    return name;
}

void LogMessage::setAppName(std::string name) {
    this->name = name;
}

// --- Value ---
std::string LogMessage::getValue() const {
    return std::to_string(value);
}

void LogMessage::setValue(float value) {
    this->value = value;
}

// --- Context ---
std::string LogMessage::getContext() const {
    return std::string(magic_enum::enum_name(context));
}

void LogMessage::setContext(::Enums::TelemetrySrc context){
    this->context = context;
}

// --- Timestamp ---
void LogMessage::setTimeStamp() {
    auto now = std::chrono::system_clock::now();
    uint64_t epochSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    this->timeStamp = epochSeconds;
}

std::string LogMessage::getTimeStamp() const {
    // Convert uint64 → time_t
    std::time_t t = static_cast<std::time_t>(timeStamp);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%H:%M:%S"); // Format
    return ss.str();
}

// --- Severity ---
std::string LogMessage::getSeverity() const {
    return std::string(magic_enum::enum_name(severity));
}

void LogMessage::setSeverity(::Enums::SeverityLvl severity){
    this->severity = severity;
}

// --- Unit ---
std::string LogMessage::getUnit() const {
    return unit;
}

void LogMessage::setUnit(std::string unit){
    this->unit = unit;
}

// --- ostream overload ---
std::ostream& operator<<(std::ostream& out, const LogMessage& msg) {
    out << "[" << msg.getTimeStamp() << "] "     // timestamp
        << msg.getName() << " | "           // logger name
        << msg.getContext() << " ("         // context
        << msg.getUnit() << ") "            // unit
        << msg.getSeverity() << " : "       // severity
        << msg.getValue();                   // actual message
    return out;
}

// --- Protobuf Serialization ---
std::string LogMessage::serializeToProtobuf() const {
    telemetry::TelemetryMessage protoMsg;
    
    protoMsg.set_app_name(this->name);
    protoMsg.set_value(this->value);
    protoMsg.set_context(static_cast<telemetry::TelemetrySrc>(this->context));
    protoMsg.set_severity(static_cast<telemetry::SeverityLvl>(this->severity));
    protoMsg.set_unit(this->unit);
    protoMsg.set_timestamp_sec(this->timeStamp);

    std::string output;
    if (!protoMsg.SerializeToString(&output)) {
        std::cerr << "Failed to serialize LogMessage!" << std::endl;
        return "";
    }
    return output;
}