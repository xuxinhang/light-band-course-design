#include "msp430.h"
#include "light_control.h"

#include "command_parser.h"
#include "led_pwm.h"

int frame_list_length = -1;

LIGHT_FRAME *frame_array;

unsigned frame_index_counter = 0;  // the index of the current frame.
LIGHT_FRAME *frame_item_pointer = NULL;
LIGHT_FRAME *frame_next_item_pointer = NULL;


int light_control_update (char frame_list_string[]) {
  light_control_abort();
  // Prase string data.
  frame_list_length = parse_frame_string(frame_list_string);
  // init variables
  frame_array = frame_list;
  frame_item_pointer = frame_list;
  return frame_list_length;
}

int light_control_abort () {
  frame_list_length = -1;
  frame_list_clear(frame_list);
  // Reset variables
  frame_item_pointer = frame_next_item_pointer = NULL;
  return 0;
}


void interrupt_update_led_lights (void) {
  if (frame_list_length <= 0 || frame_item_pointer == NULL) return;
  if (frame_next_item_pointer == NULL) {
    frame_next_item_pointer = frame_item_pointer;
  }
  static float ratio[CHANNEL_NUMBER];
  static float curt_light[CHANNEL_NUMBER];
  int i;
  int time_duration;
  
  if (frame_index_counter <= frame_list->time) {
    led1_light = (int)(frame_list->channels[0]);
    led2_light = (int)(frame_list->channels[1]);
    led3_light = (int)(frame_list->channels[2]);
    curt_light[0] = led1_light;
    curt_light[1] = led2_light;
    curt_light[2] = led3_light;
  }
  
  if (frame_index_counter >= frame_next_item_pointer->time) {
    frame_item_pointer = frame_next_item_pointer;
    
    if (frame_item_pointer->_next == NULL) {  // end of frame list
      time_duration = LIGHT_UPDATE_TIME_CHIP_MS;
      frame_next_item_pointer = frame_array;
      frame_index_counter = frame_array->time;
    } else {
      frame_next_item_pointer = frame_item_pointer->_next;
      time_duration = frame_next_item_pointer->time - frame_item_pointer->time;
    }
    
    float time_chip_diff = ((int)frame_index_counter - (int)(frame_item_pointer->time)) *1.0/ LIGHT_UPDATE_TIME_CHIP_MS;
    for (i = 0; i < CHANNEL_NUMBER; i++) {
      // Ratio[i] 是每 LIGHT_UPDATE_TIME_CHIP_MS 的变化量
      ratio[i] = (int)(frame_next_item_pointer->channels[i] - frame_item_pointer->channels[i]) * LIGHT_UPDATE_TIME_CHIP_MS *1.0 / time_duration;
      curt_light[i] = frame_item_pointer->channels[i] + ratio[i] * time_chip_diff;
    }
  
  } else {
    // 在区间内递推更新即可
    for (i = 0; i < CHANNEL_NUMBER; i++) {
      curt_light[i] += ratio[i];
    }
  }
  
  // 更新变量值
  led1_light = (int)curt_light[0];
  led2_light = (int)curt_light[1];
  led3_light = (int)curt_light[2];
  frame_index_counter += LIGHT_UPDATE_TIME_CHIP_MS;
  update_led_light();
  
}