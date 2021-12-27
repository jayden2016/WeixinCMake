#ifndef __WEIXIN_CHANNEL_H__
#define __WEIXIN_CHANNEL_H__

#if __cplusplus
extern "C"{
#endif

typedef struct
{
    void (*onChannelConnEvent)(bool bChannelConn);
    void (*onChannelLinkStateChange)(bool bLinkState);
    uint32_t (*onDataRecv)(uint8_t *data, uint32_t len);
}TChannelRegInfo;

int32_t ChannelRegisterCb(TChannelRegInfo *pChannelRegInfo);
int32_t ChannelConnect(uint32_t timeout_seconds);
void ChannelDisconnect();
int32_t ChannelSendData(uint8_t *data, uint32_t data_len);
bool bPhysicalLinkConnected();
uint32_t ChannelMtuGet();

#if __cplusplus
}
#endif

#endif

