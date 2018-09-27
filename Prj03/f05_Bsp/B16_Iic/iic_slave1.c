#define START_STATE     0 //��ʼ
#define CONTROL_STATE   1//��������
#define ACK_STATE       2 //ACKӦ��
#define NOACK_STATE     3 //��ACKӦ��
#define WRITE_STATE     4 //д�ӻ�
#define READ_STATE      5//���ӻ�
#define STOP_STATE      6//ֹͣ

BYTE STATE=0;
BYTE address=0;//���յ��Ĵӻ���ַ
BYTE count=0;//���յ�һλ����������һ���ֽڵļ���
BYTE receive_BYTE;//�ӻ��������������ֽ�
BYTE receive_buf[20]={0x00};//�ӻ������������ݵ�buf
BYTE write_buf[20]={0x47,0x55,0x11};//�ӻ��������ݸ�������buf
BYTE receive_len=0;//�ӻ������������ֽ�ʱ�ĳ���
BYTE send_len=0;//�ӻ�Ҫ���͸������ֽڵĳ���
BYTE write_end=0;//�����Ƿ�Դӻ�д��
BYTE read_end=0;//�����Ƿ������ӻ����͵�����
BYTE length_ACK=0;//�ӻ��Ƿ��յ���Ҫ�������ݸ������ĳ���
BYTE read_num=0;//
BYTE Temp=0;//

void PORT1_InterruptInit(void)
{
    EA=1;
    P1DIR&=0XFB;//p1.2���룬SCL
    IEN2|=0X10;//p1�ж�ʹ�ܣ�scl
    P1IEN|=0X04;//p1.2�ж�ʹ�ܣ�scl
    PICTL&=~0X02;//p1�������ж�0:rising edge
    P1IFG&=~0X04;    
}

#pragma vector = P1INT_VECTOR
__interrupt void P1_ISR(void)
{
    if(P1IFG>0)//�����ж�
    {
        P1IFG=0;
        LED1=~LED1;
    }        
    P1IF=0;//���жϱ�־

    if(P1IFG>0)
    {
        P1IFG=0;
        switch(STATE)
        {
            case START_STATE:
                SDA_(IN);
                if(SDA)
                {
                    while(SDA);
                    while(SCL);
                    STATE=CONTROL_STATE;
                }
                break;
             case CONTROL_STATE:
                address <<=1;
                if(1==SDA)
                {
                    address |= 0x01;
                }
                else
                {
                    address |= 0x00;
                }
                count++;
                if((address&0xfc)==0xa4)
                {
                    STATE = ACK_STATE;
                }
                else
                {
                    STATE = NOACK_STATE;
                }
                break;
            case ACK_STATE:
                SDA_(OUT);//SDA����Ϊ���
                SDA=0;
                if((write_end==1)||(read_end==1))//�Ѿ��������д��
                {
                    STATE = STOP_STATE;
                    write_end = 0;
                    read_end  = 0;
                }
                else
                {
                    if((address & 0x01)==0x00)//����дslave
                    {
                        STATE = WRITE_STATE;
                    }
                    else
                    {
                        STATE = READ_STATE;
                    }
                }
                break;
            case NOACK_STATE:
                SDA_(OUT);//SDA����Ϊ���
                SDA=1;
                address = 0;
                STATE = START_STATE;
                break;
            case WRITE_STATE://����д�ӻ�
                SDA_(IN);//SDA����Ϊ����,��Ϊ����ACKʱ����Ϊ�����
                if((address & 0x02)==0x00)//д���ֽ�
                {
                    receive_BYTE <<=1;
                    if(1==SDA)
                    {
                        receive_BYTE |= 0X01;
                    }
                    else
                    {
                        receive_BYTE |= 0X00;
                    }
                    count++;
                    if(8==count)
                    {
                        STATE = ACK_STATE;
                        count = 0;
                        write_end = 1;
                    }
                }
                else
                {
                    receive_buf[receive_len]<<=1;
                    if(1==SDA)
                    {
                        receive_buf[receive_len] |= 0X01;
                    }
                    else
                    {
                        receive_buf[receive_len] |= 0X00;
                    }
                    count++;
                    if(8==count)
                    {
                        count = 0;
                        receive_len++;
                        UART1_Send_BYTE(receive_buf[receive_len-1]);
                        if(receive_len>=receive_buf[0]+1)
                        {
                            write_end=1;
                            receive_len=0;
                        }
                        STATE = ACK_STATE;
                    }
                }
                break;
            case READ_STATE://�������ӻ�
                if(!length_ACK)//�������͹��ӻ�����
                {
                    SDA_(IN);
                    send_len<<=1;
                    if(1==SDA)
                        send_len |= 0x01;
                    else
                        send_len |= 0x00;
                    count++;
                    if(8==count)
                    {
                        length_ACK = 1;//�������ͳ��ȸ��ӻ�
                        LED0=1;
                        count=0;
                        STATE = ACK_STATE;
                    }
                }
                else
                {
                    SDA_(OUT);
                    Temp = write_buf[read_num];
                    Temp<<=count;
                    UART1_Send_BYTE(Temp);
                    if((Temp&0x80)==0x80)
                        SDA=1;
                    else
                        SDA=0;
                    count++;

                    if(8==count)//����7λ�����ö�һ���ֽ�
                    {
                        count = 0;
                        read_num++;
                        if(read_num>=send_len)//��������������
                        {
                            read_num=0;
                            read_end=1;
                            length_ACK = 0;
                        }
                        STATE = ACK_STATE;
                    }
                }
                break;
            case STOP_STATE:
                SDA_(IN);
                while(!SDA);
                address=0;
//                receive_BYTE=0;
                receive_len=0;
                send_len =0;
                LED1=~LED1;
                STATE = START_STATE;
                break;
            default:
                break;
        }
    }
    P1IF = 0;
}

