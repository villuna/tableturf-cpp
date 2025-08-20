// A simple generic channel implementation. Basically just a queue stored on the heap with a mutex
// protecting it.

#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

// Forward declarations
template<typename T>
class Sender;

template<typename T>
class Reciever;

template<typename T>
std::pair<Reciever<T>, Sender<T>> channel();

// The internal queue and lock for the channel. The sender and reciever will both have a shared
// pointer to this structure.
template<typename T>
class ChannelInner {
    std::mutex m_m {};
    std::queue<T> m_queue {};

    ChannelInner() {}

    friend std::pair<Reciever<T>, Sender<T>> channel<T>();
    friend class Sender<T>;
    friend class Reciever<T>;
};

template<typename T>
class Reciever {
    std::shared_ptr<ChannelInner<T>> m_inner;

    Reciever(std::shared_ptr<ChannelInner<T>> inner) : m_inner(inner) {}
    friend std::pair<Reciever<T>, Sender<T>> channel<T>();

public:
    // Returns the next item in the channel, if any.
    std::optional<T> recv() {
        std::scoped_lock lock(m_inner->m_m);

        if (m_inner->m_queue.empty()) {
            return std::nullopt;
        } else {
            T res = m_inner->m_queue.front();
            m_inner->m_queue.pop();
            return res;
        }
    }
};

template<typename T>
class Sender {
    std::shared_ptr<ChannelInner<T>> m_inner;

    Sender(std::shared_ptr<ChannelInner<T>> inner) : m_inner(inner) {}
    friend std::pair<Reciever<T>, Sender<T>> channel<T>();

public:
    // Pushes an item to the channel.
    void send(T value) {
        std::scoped_lock lock(m_inner->m_m);
        m_inner->m_queue.push(value);
    }
};

// Creates a new channel and returns a pair containing the reciever and the sender.
template<typename T>
std::pair<Reciever<T>, Sender<T>> channel() {
    std::shared_ptr<ChannelInner<T>> inner = std::shared_ptr<ChannelInner<T>>(new ChannelInner<T>());

    Reciever<T> rx = Reciever(inner);
    Sender<T> tx = Sender(inner);

    return { rx, tx };
}
