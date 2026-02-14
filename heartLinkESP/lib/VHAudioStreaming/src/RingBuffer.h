#pragma once

#include <cstdint>
#include <cstddef>
#include <atomic>
namespace VH
{
    template <typename T>
    class RingBuffer
    {
    public:
        RingBuffer(T *buffer, size_t size);

        bool push(const T &item);
        bool pop(T &item);

        bool isEmpty() const;
        bool isFull() const;
        void reset();

        size_t size() const;
        size_t capacity() const;

        T *dmaReadPtr(size_t &outLen);
        void dmaReadAdvance(size_t count);

        T *dmaWritePtr(size_t &outLen);
        void dmaWriteAdvance(size_t count);

    private:
        T *buffer_;
        size_t size_;
        std::atomic<size_t> head_;
        std::atomic<size_t> tail_;
    };
}

namespace VH
{
    template <typename T>
    RingBuffer<T>::RingBuffer(T *buffer, size_t size)
        : buffer_(buffer), size_(size), head_(0), tail_(0) {}

    template <typename T>
    bool RingBuffer<T>::push(const T &item)
    {
        size_t head = head_.load(std::memory_order_relaxed);
        size_t next_head = (head + 1) % size_;

        if (next_head == tail_.load(std::memory_order_acquire))
        {
            return false;
        }

        buffer_[head] = item;
        head_.store(next_head, std::memory_order_release);
        return true;
    }

    template <typename T>
    bool RingBuffer<T>::pop(T &item)
    {
        size_t tail = tail_.load(std::memory_order_relaxed);

        if (tail == head_.load(std::memory_order_acquire))
        {
            return false;
        }

        item = buffer_[tail];
        tail_.store((tail + 1) % size_, std::memory_order_release);
        return true;
    }

    template <typename T>
    bool RingBuffer<T>::isEmpty() const
    {
        return head_.load() == tail_.load();
    }

    template <typename T>
    bool RingBuffer<T>::isFull() const
    {
        size_t next_head = (head_.load() + 1) % size_;
        return next_head == tail_.load();
    }

    template <typename T>
    void RingBuffer<T>::reset()
    {
        head_.store(0);
        tail_.store(0);
    }

    template <typename T>
    size_t RingBuffer<T>::size() const
    {
        size_t head = head_.load();
        size_t tail = tail_.load();
        return (head >= tail) ? (head - tail) : (size_ - tail + head);
    }

    template <typename T>
    size_t RingBuffer<T>::capacity() const
    {
        return size_ - 1;
    }

    template <typename T>
    T *RingBuffer<T>::dmaReadPtr(size_t &outLen)
    {
        size_t head = head_.load();
        size_t tail = tail_.load();

        if (tail == head)
        {
            outLen = 0;
            return nullptr;
        }

        if (head > tail)
        {
            outLen = head - tail;
        }
        else
        {
            outLen = size_ - tail;
        }

        return &buffer_[tail];
    }

    template <typename T>
    void RingBuffer<T>::dmaReadAdvance(size_t count)
    {
        size_t tail = tail_.load();
        tail_.store((tail + count) % size_);
    }

    template <typename T>
    T *RingBuffer<T>::dmaWritePtr(size_t &outLen)
    {
        size_t head = head_.load();
        size_t tail = tail_.load();

        if (((head + 1) % size_) == tail)
        {
            outLen = 0;
            return nullptr;
        }

        if (tail > head)
        {
            outLen = tail - head - 1;
        }
        else
        {
            outLen = size_ - head;
            if (tail == 0)
                outLen -= 1;
        }

        return &buffer_[head];
    }

    template <typename T>
    void RingBuffer<T>::dmaWriteAdvance(size_t count)
    {
        size_t head = head_.load();
        head_.store((head + count) % size_);
    }
}

template<typename T>
using VHRingBuffer = VH::RingBuffer<T>;