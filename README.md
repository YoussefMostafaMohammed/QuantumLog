# Phase One: Core Synchronous Logging Foundation

## Goals
1. Create a concrete skeleton for the project upon which everything will be built upon.
1. Provide a minimalist version of the overall functionality of the final product; following the practices of Agile Methodology.

## Requirements
1. Define the structure of a log message.
1. Create a simple class that manages/takes logs and routes them to different sinks.
1. Create log sinks that store/display logs.

## Topic
### C++
* `virtual`, `default`, `friend`, `public`, `protected`, `private` keywords.
* Classes, interfaces, and inheritance (public, protected, private, virtual).
* Virtual tables (virtual functions & virtual destructors).
* Operator overloading.
* Namespaces.
* Name Mangling

### Design Patterns
* Strategy Design Pattern.

## Deliverables
### Main
* `ILogSink` &rarr; contains a pure virtual function for `write` capability and a defaulted virtual destructor.
* `LogMessage` &rarr; contains fields for app name, context, time, severity, text.
* `LogManager` &rarr; a class that for now stores `LogMessage` objects into a private buffer and has a method that routes the messages into all sinks.
* `ConsoleSinkImpl` & `FileSinkImpl` &rarr; both implement the interface and override the `write` functionality as suited for each sink.
### Bonus
* Implement `<<` operator for `LogMessage` that returns a fully-formatted string that is ready for printing. **Hint: look up `std::ostream`.** 


