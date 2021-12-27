#ifndef __ADAM_CLIENT_CHANNEL_IMPL_H__
#define __ADAM_CLIENT_CHANNEL_IMPL_H__

#include "adam_client_channel_zidl.h"

namespace adam {
class AdamClientChannelImpl : public AdamClientChannel {

public:
    AdamClientChannelImpl(const std::string& service_name) {}
    AdamClientChannelImpl() {} 
    virtual ~AdamClientChannelImpl() {}

    virtual void SetCallback(AdamClientChannelCallback* callback);

    virtual void Init();

    virtual int32_t Connect(uint32_t timeout_seconds);

    virtual void Disconnect();

    virtual int32_t SendData(const std::string& data_bytes);

    virtual bool IsPhysicalLinkConnected();

    virtual uint32_t GetChannelMTU();

private:
    static void onChannelConnEvent(bool bChannelConn);

    static void onChannelLinkStateChange(bool bLinkState);

    static uint32_t onDataRecv(uint8_t *data, uint32_t len);

    static AdamClientChannelCallback *pAdamClientChannelCb;
};

}

#endif
