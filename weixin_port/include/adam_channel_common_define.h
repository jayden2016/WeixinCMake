//
//  adam_channel_common_define.h
//  adam_ipc
//
//  Created by zorrowu on 2021/9/27.
//

#ifndef adam_channel_common_define_h
#define adam_channel_common_define_h

enum AdamChannelErrorCode
{
    kAdamChannelSuccess = 0,           //成功
    kAdamChannelNoLink =  -1,          //物理链路未连接，例如wifi，4g，ble未连接
    kAdamChannelDisconnected =  -2,    //Channel已断开
    kAdamChannelTimeout =  -3,         //超时
    kAdamChannelCancel =  -4,          //调用方取消
    //其他根据实现方具体情况添加
};

#endif /* adam_channel_common_define_h */

