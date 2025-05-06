#ifndef LASER_DETECTOR
#define LASER_DETECTOR


/***
 * �ж�ָ��λ���Ƿ���ھ�̬�ϰ���
 * @param x, y Ŀ��λ�õ�����
 * @param width, length ���䳤��
 * ע�������̶�Ϊ10
 */
bool mapping_detect(int x, int y, int width, int length);


/***
 * �ж�ָ��λ���Ƿ���ڶ�̬�ϰ���
 * @param x, y Ŀ��λ�õ�����
 * @param width, length ���䳤��
 * ע�������̶�Ϊ5
 */
bool sweeping_detect(int x, int y, int width, int length);

/***
 * �ж�Ŀ��λ�õ��ϰ����Ƿ���ʧ
 * @param x, y Ŀ��λ�õ�����
 */
bool targeted_detect(int x, int y);

#endif
