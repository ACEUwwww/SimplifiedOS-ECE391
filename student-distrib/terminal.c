#include "terminal.h"
#include "keyboard.h"
#include "lib.h"
#include "types.h"


static terminal_t terminal_array[MAX_TERMINAL_SIZE];
static int32_t current_terminal_id;
/* 
 * terminal_open
 *   DESCRIPTION: initializing the terminal for further use.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */

int32_t terminal_open(const char* filename)
{
    return 1 ;
}

/* 
 * terminal_close
 *   DESCRIPTION: cleaning terminal for further use.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */

int32_t terminal_close(const char* fname)
{
    clean_buffer();
    clean_terminal_buffer();
    return 1;
}

/* 
 * terminal_read
 *   DESCRIPTION: read from keyboard buffer to terminal buffer
 *   INPUTS: const char* fname,uint32_t fd, void* buf, int32_t nbytes)
 *   OUTPUTS: none
 *   RETURN VALUE: i - the length read
 *   SIDE EFFECTS: none
 */
int32_t terminal_read(const char* fname,uint32_t fd, void* buf, int32_t nbytes)
{
    char* t_buf = buf ;
    if (t_buf==NULL){return -1;}
    if (nbytes < 0){return -1;}
    // we have 1024 as the max buf size
    if (nbytes >= 1024){
        return -1;
    }   
    int i=0;
    while((check_enter() == 0)){}
    while(terminal_array[current_terminal_id].terminal_buffer[i] != '\n'){
        t_buf[i] = terminal_array[current_terminal_id].terminal_buffer[i];
        i++;
    }
    clean_buffer();
    terminal_array[current_terminal_id].buf_pos = 0;
    return i;
}

/* 
 * terminal_write
 *   DESCRIPTION: write from terminal to screen and clean terminal buffer.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: i - write length
 *   SIDE EFFECTS: none
 */

int32_t terminal_write(int32_t fd, const void* input_buf, int32_t nbytes)
{
    const char* buf = input_buf;
    if(buf == NULL){
        return -1;
    }
    int32_t count;
    for(count = 0; count < nbytes; count++){
        putc(buf[count]);
    }
    return count;
}

/* 
 * terminal_write
 *   DESCRIPTION: clean the terminal buffer.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void clean_terminal_buffer()
{
    int i;
    /*Clean the keyboard buffer*/
    for (i=0;i<MAX_TERMINAL_BUFFER_SIZE;i++)
    {
        terminal_array[current_terminal_id].terminal_buffer[i] = '\0' ;
    }
    return ;
}
/* 
 * terminal_switch
 *   DESCRIPTION: switch terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
int32_t terminal_switch(int32_t new_terminal_id)
{
    return 0;
}

/* 
 * terminal_init
 *   DESCRIPTION: init terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
int32_t terminal_init(void)
{
    int i,j;
    for (i=0;i<MAX_TERMINAL_SIZE;i++){
        terminal_array[i].terminal_id = i;
        terminal_array[i].current_process_id = -1;
        // terminal_array[i].current_process_ptr = NULL;
        terminal_array[i].screen_cursor_x = 0;
        terminal_array[i].screen_cursor_y = 0;
        terminal_array[i].buf_pos = 0;
        for (j=0;j<MAX_TERMINAL_BUFFER_SIZE;j++){
            terminal_array[i].terminal_buffer[j] = '\0';
        }
    }
    current_terminal_id = 0;
    terminal_array[0].video_buffer_addr = VEDIO_BUFFER_1;
    terminal_array[0].video_buffer_addr = VEDIO_BUFFER_2;
    terminal_array[0].video_buffer_addr = VEDIO_BUFFER_3;
    return execute((uint8_t*)"shell");    
}

/* 
 *  read to cur terminal buffer
 *   DESCRIPTION: clean the terminal buffer.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: void 
 *   SIDE EFFECTS: none
 */
void read_to_cur_terminal_buffer(int32_t index,unsigned char input)
{
    terminal_array[current_terminal_id].terminal_buffer[index] = input;
    return ;
}
