#include <stdio.h>
#include "command_prompt.h"

#define MAX_PROMPT_LENGTH 100

static char command_prompt[MAX_PROMPT_LENGTH];

void set_command_prompt(char * url) {
    snprintf(command_prompt, MAX_PROMPT_LENGTH, "HTTP %s> ", url);
}

char * get_command_prompt() {
    return command_prompt;
}

