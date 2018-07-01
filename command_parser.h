#ifndef _INCLUDE_COMMAND_PARSER_H
#define _INCLUDE_COMMAND_PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEM_DATA_LENGTH 5
#define CHANNEL_NUMBER 3

typedef struct light_frame_struct {
    unsigned time; // Time of this frame.
    unsigned short channels[CHANNEL_NUMBER]; // Brightness of each channel.
    int extra_info;  // Some meta info about this frame.
    struct light_frame_struct *_next;
} LIGHT_FRAME; 

enum COMMAND_TYPE {MODE_FRAME, MODE_VOCE};

extern LIGHT_FRAME *frame_list;

int parse_frame_string (char *input_string);
int frame_list_clear ();

static LIGHT_FRAME *list_erase (LIGHT_FRAME *list_head);
static LIGHT_FRAME *list_append (LIGHT_FRAME *list_head);
enum COMMAND_TYPE parse_command (char *command, char **payload);

int main_test_for_parser ();

#endif




