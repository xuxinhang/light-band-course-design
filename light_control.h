#ifndef _INCLUDE_LIGHT_CONTROL_H
#define _INCLUDE_LIGHT_CONTROL_H

/**
 * ������Ҫ��h�ļ��ж��壬������������ʹ��extern��
 * ���⣺static �� const ����������ͷ�ļ��ж��� 
 */

int light_control_update (char frame_list_string[]);
int light_control_abort ();

void interrupt_update_led_lights (void);

#endif