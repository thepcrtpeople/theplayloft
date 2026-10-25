#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "launcher.h"

static const char *allowed_games[MAX_ALLOWED_GAMES] = {
    "game1",
    "game2",
    "race-demo",
    "arcade-01",
    NULL
};

static int current_session_active = 0;
static char current_game[64] = {0};

int is_game_allowed(const char *game_id) {
    if (game_id == NULL) {
        return 0;
    }

    for (int i = 0; allowed_games[i] != NULL; ++i) {
        if (strcmp(game_id, allowed_games[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

static void parse_game_and_minutes(const char *request_body, char *game_id, size_t game_size, int *minutes) {
    const char *game_marker = strstr(request_body, "\"game\"");
    const char *minutes_marker = strstr(request_body, "\"minutes\"");

    *minutes = 30;
    game_id[0] = '\0';

    if (game_marker != NULL) {
        const char *colon = strchr(game_marker, ':');
        if (colon != NULL) {
            const char *quote1 = strchr(colon + 1, '"');
            if (quote1 != NULL) {
                const char *quote2 = strchr(quote1 + 1, '"');
                if (quote2 != NULL && quote2 > quote1 + 1) {
                    size_t length = (size_t)(quote2 - (quote1 + 1));
                    if (length >= game_size) {
                        length = game_size - 1;
                    }
                    memcpy(game_id, quote1 + 1, length);
                    game_id[length] = '\0';
                }
            }
        }
    }

    if (minutes_marker != NULL) {
        const char *colon = strchr(minutes_marker, ':');
        if (colon != NULL) {
            *minutes = atoi(colon + 1);
            if (*minutes <= 0) {
                *minutes = 30;
            }
        }
    }
}

launch_result_t launch_game_from_request(const char *request_body) {
    launch_result_t result;
    memset(&result, 0, sizeof(result));

    char game_id[64];
    int minutes = 30;
    parse_game_and_minutes(request_body, game_id, sizeof(game_id), &minutes);

    if (!is_game_allowed(game_id)) {
        snprintf(result.message, sizeof(result.message), "Game '%s' is not allowed", game_id);
        result.success = 0;
        return result;
    }

    snprintf(current_game, sizeof(current_game), "%s", game_id);
    current_session_active = 1;

    printf("Launching game '%s' for %d minutes\n", current_game, minutes);

    snprintf(result.message, sizeof(result.message), "Launching %s for %d minutes", current_game, minutes);
    result.success = 1;
    return result;
}

void stop_current_session(void) {
    current_session_active = 0;
    current_game[0] = '\0';
    printf("Session stopped\n");
}
