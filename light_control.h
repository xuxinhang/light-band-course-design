#ifndef _INCLUDE_LIGHT_CONTROL_H
#define _INCLUDE_LIGHT_CONTROL_H

/**
 * 变量不要在h文件中定义，但可以声明（使用extern）
 * 例外：static 和 const 变量可以在头文件中定义 
 */

int light_control_update (char frame_list_string[]);
int light_control_abort ();

void interrupt_update_led_lights (void);

#endif