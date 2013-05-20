#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <histedit.h>
#include <curl/curl.h>

#define ERROR(msg) fprintf(stderr, msg); ret = -1; goto CLEANUP;

#define HIST_SIZE 200;
#define MAX_PROMPT_LENGTH 100

char command_prompt[MAX_PROMPT_LENGTH];

static void set_command_prompt(char * url) {
    snprintf(command_prompt, MAX_PROMPT_LENGTH, "HTTP %s> ", url);
}

static char * get_command_prompt() {
    return command_prompt;
}

char * prompt(EditLine *e) {
    return get_command_prompt();
}

int main(int argc, char *argv[]) {
    int ret = 0;
    EditLine *el = NULL;
    History *hist = NULL;
    HistEvent hist_event;
    CURL *curl = NULL;

    set_command_prompt("<none>");

    //INIT CURL
    if (0 != curl_global_init(CURL_GLOBAL_ALL)) {
        ERROR("Error init libcurl");
    }

    //INIT EDITLINE
    el = el_init(argv[0], stdin, stdout, stderr);
    if (!el) {
        ERROR("Error in el_init");
    }

    hist = history_init();
    if (!hist) {
        ERROR("Error in history_init");
    }

    //SETUP editline and history
    el_set(el, EL_PROMPT, &prompt);
    el_set(el, EL_EDITOR, "emacs");
    history(hist, &hist_event, H_SETSIZE, 200);
    el_set(el, EL_HIST, history, hist);

    //main loop
    int count;
    const char *line;
    char *base_url = "http://www.miketill.com";
    int base_len = strlen(base_url);

    curl = curl_easy_init();
    while ((line = el_gets(el, &count))) {
        if (count > 1) {
            history(hist, &hist_event, H_ENTER, line);
            char *url = malloc(sizeof(char)*(base_len+count));
            if (!url) {
                ERROR("malloc for url");
            }
            strcpy(url, base_url);
            strcat(url, line);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_perform(curl);
            free(url);
        }
    }
    putc('\n', stdout);
CLEANUP:
    if (curl) curl_easy_cleanup(curl);
    if (hist) history_end(hist);
    if (el) el_end(el);
    curl_global_cleanup();
    return ret;
}
