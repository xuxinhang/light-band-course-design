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
  // Prase string data.
  frame_list_length = parse_string(frame_list_string);
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

  static float ratio[CHANNEL_NUMBER];
  static int curt_light[CHANNEL_NUMBER];
  int i;
  int time_duration;
  
  if (frame_index_counter <= frame_list->time) {
    led1_light = (int)(frame_list->channels[0]);
    led2_light = (int)(frame_list->channels[1]);
    led3_light = (int)(frame_list->channels[2]);
  }
  
  if (frame_next_item_pointer == NULL) {
    frame_next_item_pointer = frame_item_pointer;
  }
  
  if (frame_index_counter >= frame_next_item_pointer->time) {
    frame_item_pointer = frame_next_item_pointer;
    
    if (frame_item_pointer->_next == NULL) {  // end of frame list
      time_duration = 1;
      frame_next_item_pointer = frame_array;
      frame_index_counter = frame_array->time;
    } else {
      frame_next_item_pointer = frame_item_pointer->_next;
      time_duration = frame_next_item_pointer->time - frame_item_pointer->time;
    }
    
    for (i = 0; i < CHANNEL_NUMBER; i++) {
      ratio[i] = ((int)(frame_next_item_pointer->channels[i]) - (int)(frame_item_pointer->channels[i])) *1.0 / time_duration;
    }
  }
  
  for (i = 0; i < CHANNEL_NUMBER; i++) {
    curt_light[i] = (int)(frame_item_pointer->channels[i]) + ratio[i] * ((int)frame_index_counter - (int)(frame_item_pointer->time));
  }
  
  // [TODO] Update common value
  led1_light = (int)curt_light[0];
  led2_light = (int)curt_light[1];
  led3_light = (int)curt_light[2];

  
  frame_index_counter += 61;
}