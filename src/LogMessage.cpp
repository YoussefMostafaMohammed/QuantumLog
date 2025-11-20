#include "LogMessage.hpp"

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

void LogMessage::setTime(std::string time) {
    this->time = time;
}

// --- Severity ---
std::string LogMessage::getSeverity() const {
    return severity;
}

void LogMessage::setSeverity(std::string severity) {
    this->severity = severity;
}

// --- ostream overload ---
std::ostream& operator<<(std::ostream& out, const LogMessage& msg) {
    out << "Name: "     << msg.name
        << ", Text: "   << msg.text
        << ", Context: "<< msg.context
        << ", Time: "   << msg.time
        << ", Severity: "<< msg.severity;
    return out;
}
