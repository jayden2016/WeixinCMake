// Generated by zidl C++ generator. DO NOT EDIT!
// source: adam_client_channel.zidl

#ifndef ADAM_CLIENT_CHANNEL_ZIDL_H
#define ADAM_CLIENT_CHANNEL_ZIDL_H

#include <string>
#include <vector>
#include <cstdint>

namespace adam {

class AdamClientChannelCallback {

public:
    virtual ~AdamClientChannelCallback() {}

    virtual void OnReceiveDataEvent(const std::string& data_bytes) = 0;

    virtual void OnChannelDisconnectedEvent() = 0;

    virtual void OnPhysicalLinkStateChangedEvent(bool is_connected) = 0;
};

class AdamClientChannel {

public:
    virtual ~AdamClientChannel() {}

    virtual void SetCallback(AdamClientChannelCallback* callback) = 0;

    /**
     * @description: 初始化接口，创建实例后调用的第一个接口
     */
    virtual void Init() = 0;

    /**
     * @description: 连接服务，能被DeleteAdamClientChannel打断并返回
     * @param timeout_seconds 超时时间，秒为单位，0xFFFFFFFF表示永久阻塞
     * @return 0：连接成功；其他值：错误码在AdamChannelErrorCode中定义
     */
    virtual int32_t Connect(uint32_t timeout_seconds) = 0;

    /**
     * @description: 断开连接
     */
    virtual void Disconnect() = 0;

    /**
     * @description: 发送数据，实例内部需要把buffer的数据全部接收，建议做异步发送，如果阻塞发送要求能被DeleteAdamClientChannel打断返回，函数返回后data会被释放
     * @param data 要发送的数据内容
     * @return 0：调用成功；其他值：错误码在AdamChannelErrorCode中定义并添加相关说明
     */
    virtual int32_t SendData(const std::string& data_bytes) = 0;

    /**
     * @description: 查询物理链路是否已连接，例如wifi，4g，ble是否已连接，根据链路服务提供方实际载体决定
     * @return true：已连接；false：未连接、无网
     */
    virtual bool IsPhysicalLinkConnected() = 0;

    /**
     * @description: 获取当前channel的mtu数值，该接口在Connect成功后会调用一次，sdk内部会保证每次调用SendData的值小于等于该mtu数值
     * @return channel_mtu：当前channel的mtu数值
     */
    virtual uint32_t GetChannelMTU() = 0;
};

#ifdef _WIN32
#if defined(ADAM_EXPORTS)
#define ADAM_API extern "C" __declspec(dllexport)
#elif defined(ADAM_IMPORTS)
#define ADAM_API extern "C" __declspec(dllimport)
#else
#define ADAM_API extern "C"
#endif
#elif defined(__APPLE__) || defined(__ANDROID__) || defined(__linux__)
#define ADAM_API extern "C" __attribute__((visibility("default")))
#else
#define ADAM_API extern "C"
#endif

ADAM_API AdamClientChannel* CreateAdamClientChannel(const std::string& service_name);
ADAM_API void DestroyAdamClientChannel(AdamClientChannel* adam_client_channel);
} // namespace adam

#endif // ADAM_CLIENT_CHANNEL_ZIDL_H

