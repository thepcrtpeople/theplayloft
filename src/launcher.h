#ifndef LAUNCHER_H
#define LAUNCHER_H

#define MAX_ALLOWED_GAMES 16

typedef struct {
    int success;
    char message[128];
} launch_result_t;

int is_game_allowed(const char *game_id);
launch_result_t approve_session_from_request(const char *request_body);
void stop_current_session(void);
void reset_to_wii_menu(void);

#endif
