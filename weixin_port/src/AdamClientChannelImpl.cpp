#include "AdamClientChannelImpl.h"
#include "adam_channel_common_define.h"
#include "weixin_channel.h"

namespace adam {

AdamClientChannelCallback AdamClientChannelImpl::*pAdamClientChannelCb = nullptr;

void AdamClientChannelImpl::SetCallback(AdamClientChannelCallback* callback)
{
    pAdamClientChannelCb = callback;
}

void AdamClientChannelImpl::Init()
{
    TChannelRegInfo regInfo = {nullptr, nullptr, nullptr};

	regInfo.onChannelConnEvent = onChannelConnEvent;
	regInfo.onChannelLinkStateChange = onChannelLinkStateChange;
	regInfo.onDataRecv = onDataRecv;
    ChannelRegisterCb(&regInfo);
}

int32_t AdamClientChannelImpl::Connect(uint32_t timeout_seconds)
{
    return ChannelConnect(timeout_seconds);
}

void AdamClientChannelImpl::Disconnect()
{
    return ChannelDisconnect();
}

int32_t AdamClientChannelImpl::SendData(const std::string& data_bytes)
{
    return ChannelSendData((uint8_t*)data_bytes.data(), data_bytes.size());
}

bool AdamClientChannelImpl::IsPhysicalLinkConnected()
{
    return bPhysicalLinkConnected();
}

uint32_t AdamClientChannelImpl::GetChannelMTU()
{
    return ChannelMtuGet();
}

void AdamClientChannelImpl::onChannelConnEvent(int bChannelConn)
{
    if (pAdamClientChannelCb && !bChannelConn)
	{
	    pAdamClientChannelCb->OnChannelDisconnectedEvent();
	}
}

void AdamClientChannelImpl::onChannelLinkStateChange(int bLinkState)
{
    if (pAdamClientChannelCb)
	{
	    pAdamClientChannelCb->OnPhysicalLinkStateChangedEvent((bool)bLinkState);
	}
}

uint32_t AdamClientChannelImpl::onDataRecv(uint8_t *data, uint32_t len)
{
    if (pAdamClientChannelCb && (data != nullptr) && (len != 0))
    {
        std::string str((char*)data, len);

		pAdamClientChannelCb->OnReceiveDataEvent((const std::string&)str);
		return 0;
    }
	else
	{
	    return kAdamChannelNoLink;
	}
}

AdamClientChannel* CreateAdamClientChannel(const std::string& service_name)
{
    return new AdamClientChannelImpl(service_name);
}

void DestroyAdamClientChannel(AdamClientChannel* adam_client_channel)
{
    delete adam_client_channel;
}

}
