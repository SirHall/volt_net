#pragma once
#ifndef VOLT_NET_ASYNCSTATEHANDLER_HPP
#define VOLT_NET_ASYNCSTATEHANDLER_HPP

#include <atomic>
#include <memory>

namespace volt::net
{

    template <typename EnumT>
    class AsyncStateHandler
    {
        // friend std::shared_ptr<AsyncStateHandler<EnumT>>;

    private:
        std::atomic_uint8_t status;
        AsyncStateHandler() { status = 0; }

    public:
        static std::shared_ptr<AsyncStateHandler<EnumT>> Create()
        {
            return std::shared_ptr<AsyncStateHandler>(new AsyncStateHandler());
        }

        AsyncStateHandler(const AsyncStateHandler &other) = delete;
        AsyncStateHandler(AsyncStateHandler &&other)      = delete;
        AsyncStateHandler &operator=(const AsyncStateHandler &other) = delete;
        AsyncStateHandler &operator=(AsyncStateHandler &&other) = delete;

        ~AsyncStateHandler() {}
        void  SetState(EnumT state) { this->status = (std::uint8_t)state; }
        EnumT GetState() { return static_cast<EnumT>(this->status.load()); }
    };
} // namespace volt::net

#endif
