#include "command_parser.h"

char example_string[] = "0, 99,123,44,; 700,12,66,128,; 1200, 134,255,80,;";

extern LIGHT_FRAME *frame_list = NULL;

static LIGHT_FRAME *list_append (LIGHT_FRAME *list_head) {
    LIGHT_FRAME *new_item, *temp_pointer;
    
    new_item = (LIGHT_FRAME*)malloc(sizeof(LIGHT_FRAME));
    new_item->_next = NULL;
    
    if (frame_list == NULL) {
        frame_list = new_item;
    } else {
        temp_pointer = list_head == NULL ? frame_list : list_head;
        while (temp_pointer->_next) temp_pointer = temp_pointer->_next;
        temp_pointer->_next = new_item;
    }
    
    return new_item;
}

static LIGHT_FRAME *list_erase (LIGHT_FRAME *list_head) {
    LIGHT_FRAME *curt_pointer, *next_pointer;
    curt_pointer = list_head;
    
    while (curt_pointer != NULL) {
        next_pointer = curt_pointer->_next;
        free(curt_pointer);
        curt_pointer = next_pointer;
    }
    
    return NULL;
}


int parse_frame_string (char *input_string) {
    char *left_pos;
    char *semi_pos, *semi_data_pos, *com_pos, *com_data_pos, *end_p;
    unsigned int com_data[ITEM_DATA_LENGTH];
    // char channel_data_str[20];
    int i, item_counter = 0;
    
    left_pos = input_string;
    semi_pos = semi_data_pos = left_pos;
    
    char *last_char_pointer = input_string + strlen(input_string) - 1;
    
    // Pointer / Chains
    LIGHT_FRAME *list_c; //, *list_prev, *list_new;
    list_c = frame_list;
    
    while (semi_pos = strchr(semi_data_pos, ';')) {

        // printf("1");
        com_pos = semi_data_pos - 1;
        for (i = 0; i < ITEM_DATA_LENGTH; i++) {
            // printf("2");
            com_data_pos = com_pos + 1;
            // printf("semi_pos :: %x \n", semi_pos);
            // printf("com_data_pos :: %x \n", com_data_pos);
            // printf("Diff: %d \n ", (int)(semi_pos - com_data_pos));
            // fflush(stdout);
            com_pos = memchr(com_data_pos, ',', (int)(semi_pos - com_data_pos));
            if (com_pos == NULL) {
                break;
            }
            com_data[i] = strtoul(com_data_pos, &end_p, 0);
        }
        semi_data_pos = semi_pos + 1;
         
        // Allocate space for a list item
        list_c = list_append(list_c);
        item_counter++;
        list_c->time = com_data[0];
        list_c->channels[0] = com_data[1];
        list_c->channels[1] = com_data[2];
        list_c->channels[2] = com_data[3];
        list_c->extra_info = com_data[4];
    }
    
    return item_counter;
}

int frame_list_clear() {
  frame_list = list_erase(frame_list);
  return 0;
}


enum COMMAND_TYPE parse_command (char *command, char **payload) {
  *payload = command;
  return MODE_FRAME;
}


int main_test_for_parser () {
    parse_frame_string(example_string);
    
    LIGHT_FRAME *list_c;
    // Loop the whole list.
    printf("[+] Time\tC_1\tC_2\tC_3\tInfo\n");
    for (list_c = frame_list; list_c; list_c = list_c->_next) {
        printf(
            "[-] %5d\t %d\t %d\t %d\t %d\n",
            list_c->time,
            list_c->channels[0],
            list_c->channels[1],
            list_c->channels[2],
            list_c->extra_info
        );
    }
    
    list_erase(frame_list);
    
    return 0;
}

/* using namespace std;
string example_str = "0, 99,123,44; 700,12,66,128; 1200, 134,255,80";

int main () {
    unsigned frame_time, channel_0, channel_1, channel_2, extra_param;
    unsigned sep_array[ITEM_DATA_LENGTH];

    string raw_str = example_str;
    string item_str;
    int semi_pos = 0, item_pos = 0, colon_pos = 0, colon_pos_prev;
    semi_pos = raw_str.find(';', semi_pos);
    item_str = raw_str.substr(item_pos, semi_pos - item_pos);

    int i = 0;
    int com_pos_prev = -1, com_pos = -1;

    for (i = 0; i <= ITEM_DATA_LENGTH; i++) {
        com_pos_prev = com_pos;
        com_pos = item_str.find(',', com_pos_prev+1);
        if (com_pos == string::npos) {
            break;
        }
        sep_array[i] = std::stoi(item_str.substr(colon_pos_prev+1, colon_pos));
    }

    printf("[0]: %d", sep_array[0]);
    printf("[1]: %d", sep_array[1]);
    printf("[2]: %d", sep_array[2]);
    printf("[3]: %d", sep_array[3]);
    printf("[4]: %d", sep_array[4]);
}
*/




