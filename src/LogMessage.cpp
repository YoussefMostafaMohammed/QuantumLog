#include "LogMessage.hpp"
#include <chrono>
#include <sstream>    
#include <iomanip>      
#include <ctime>      

// --- Name ---
std::string LogMessage::getName() const {
    return name;
}

void LogMessage::setName(std::string name) {
    this->name = name;
}

// --- Text ---
std::string LogMessage::getText() const {
    return text;
}

void LogMessage::setText(std::string text) {
    this->text = text;
}

// --- Context ---
std::string LogMessage::getContext() const {
    return context;
}

void LogMessage::setContext(std::string context) {
    this->context = context;
}

// --- Time ---
std::string LogMessage::getTime() const {
    return time;
}

void LogMessage::setTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S"); // HH:MM:SS format
        this->time= ss.str();
}

// --- Severity ---
std::string LogMessage::getSeverity() const {
    return severity;
}

void LogMessage::setSeverity(std::string severity) {
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
    out << "[" << msg.getTime() << "] "     // timestamp
        << msg.getName() << " | "           // logger name
        << msg.getContext() << " ("         // context
        << msg.getUnit() << ") "            // unit
        << msg.getSeverity() << " : "       // severity
        << msg.getText();                   // actual message
    return out;
}