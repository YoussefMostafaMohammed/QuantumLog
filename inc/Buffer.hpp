#pragma once
#include <queue>
#include <optional>

template <typename T>
class Buffer {
public:

    Buffer() = default;

    virtual ~Buffer() = default;
    
    Buffer(const Buffer& other) 
        : buffer(other.buffer) {}

    Buffer(Buffer&& other) noexcept 
        : buffer(std::move(other.buffer)) {}
    
    Buffer& operator=(const Buffer& other){
        if(&other!=this){
            buffer=other.buffer;
        }
        return *this;
    }

    Buffer& operator=(Buffer&& other) noexcept {
        if(&other!=this){
            buffer = std::move(other.buffer);
        }
        return *this;
    }
    
    bool push(const T& value) {
        buffer.push(value);
        return true;
    }

    std::optional<T> pop() {
        if (buffer.empty()) {
            return std::nullopt;
        }

        T value = buffer.front();
        buffer.pop();
        return value;
    }

    bool empty() const {
        return buffer.empty();
    }

    size_t size() const {
        return buffer.size();
    }

private:
    std::queue<T> buffer;
};
