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
    kAdamChannelSuccess = 0,           //�ɹ�
    kAdamChannelNoLink =  -1,          //������·δ���ӣ�����wifi��4g��bleδ����
    kAdamChannelDisconnected =  -2,    //Channel�ѶϿ�
    kAdamChannelTimeout =  -3,         //��ʱ
    kAdamChannelCancel =  -4,          //���÷�ȡ��
    //��������ʵ�ַ�����������
};

#endif /* adam_channel_common_define_h */

