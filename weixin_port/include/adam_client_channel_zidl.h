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
     * @description: ��ʼ���ӿڣ�����ʵ������õĵ�һ���ӿ�
     */
    virtual void Init() = 0;

    /**
     * @description: ���ӷ����ܱ�DeleteAdamClientChannel��ϲ�����
     * @param timeout_seconds ��ʱʱ�䣬��Ϊ��λ��0xFFFFFFFF��ʾ��������
     * @return 0�����ӳɹ�������ֵ����������AdamChannelErrorCode�ж���
     */
    virtual int32_t Connect(uint32_t timeout_seconds) = 0;

    /**
     * @description: �Ͽ�����
     */
    virtual void Disconnect() = 0;

    /**
     * @description: �������ݣ�ʵ���ڲ���Ҫ��buffer������ȫ�����գ��������첽���ͣ������������Ҫ���ܱ�DeleteAdamClientChannel��Ϸ��أ��������غ�data�ᱻ�ͷ�
     * @param data Ҫ���͵���������
     * @return 0�����óɹ�������ֵ����������AdamChannelErrorCode�ж��岢�������˵��
     */
    virtual int32_t SendData(const std::string& data_bytes) = 0;

    /**
     * @description: ��ѯ������·�Ƿ������ӣ�����wifi��4g��ble�Ƿ������ӣ�������·�����ṩ��ʵ���������
     * @return true�������ӣ�false��δ���ӡ�����
     */
    virtual bool IsPhysicalLinkConnected() = 0;

    /**
     * @description: ��ȡ��ǰchannel��mtu��ֵ���ýӿ���Connect�ɹ�������һ�Σ�sdk�ڲ��ᱣ֤ÿ�ε���SendData��ֵС�ڵ��ڸ�mtu��ֵ
     * @return channel_mtu����ǰchannel��mtu��ֵ
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
