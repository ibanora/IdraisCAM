#pragma once
#include "network.hpp"
#include "video_decoder.hpp"
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <obs-module.h>

namespace idraiscam {
class IdraisCamSource {
public:
    IdraisCamSource(obs_source_t* source, obs_data_t* settings);
    ~IdraisCamSource();
    void update(obs_data_t* settings);
    void enqueueControl(std::string json);
    static obs_properties_t* properties(void* data);
    static void defaults(obs_data_t* settings);
private:
    obs_source_t* source_ = nullptr;
    std::atomic<bool> running_{false};
    std::thread worker_;
    std::mutex clientMutex_;
    std::shared_ptr<TcpClient> client_;
    std::mutex commandMutex_;
    std::vector<std::string> commands_;
    VideoDecoder decoder_;
    std::string mode_ = "wifi";
    std::string host_ = "127.0.0.1";
    uint16_t port_ = 48484;
    uint32_t sampleRate_ = 48000;
    uint32_t channels_ = 1;
    bool clockSynced_ = false;
    int64_t clockOffsetNs_ = 0;
    uint64_t lastPingNs_ = 0;

    void start();
    void stop();
    void run();
    void process(const Packet& packet);
    void outputAudio(const Packet& packet);
    void flushCommands(TcpClient& client);
    uint64_t mapTimestamp(uint64_t remoteNs);
};
}
