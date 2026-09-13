#pragma once
#include "protocol.hpp"
#include <atomic>
#include <cstdint>
#include <string>
#include <vector>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#else
using SOCKET = int;
#endif

namespace idraiscam {

class NetworkRuntime {
public:
    NetworkRuntime();
    ~NetworkRuntime();
    bool ok() const { return initialized_; }
private:
    bool initialized_ = false;
};

class TcpClient {
public:
    TcpClient();
    ~TcpClient();
    bool connectTo(const std::string& host, uint16_t port);
    void close();
    bool receive(Packet& packet);
    bool send(uint16_t type, uint32_t flags, uint64_t timestampNs, const std::string& payload);
    bool isConnected() const;
private:
    std::atomic<SOCKET> socket_;
    std::mutex sendMutex_;
    bool recvAll(void* data, size_t len);
    bool sendAll(const void* data, size_t len);
};

std::vector<DeviceInfo> discoverDevices(int timeoutMs = 700);
bool ensureAdbForward(uint16_t port = 48484);

}
