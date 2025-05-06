#ifndef LASER_DETECTOR
#define LASER_DETECTOR

//�ж�(x,y)�����Ƿ��о�̬�ϰ�����е�width��length�ֱ��ʾ����Ŀ�Ⱥͳ��ȡ�
//��̬�ϰ������ɵ�λ��������ģ������̶�Ϊ10����
bool mapping_detect(int x, int y, int width, int length);

//�ж�(x,y)�����Ƿ��ж�̬�ϰ�����е�width��length�ֱ��ʾ����Ŀ�Ⱥͳ��ȡ�
//��̬�ϰ������ɵ�λ��������ģ������̶�Ϊ5����
bool sweeping_detect(int x, int y, int width, int length);

//�ж�(x,y)����ԭ�еĶ�̬�ϰ����Ƿ���ʧ��
bool targeted_detect(int x, int y);

#endif
