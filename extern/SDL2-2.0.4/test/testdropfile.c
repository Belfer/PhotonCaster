/*
  Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/

#include <stdlib.h>
#include <stdio.h>

#include "SDL_test_common.h"

static SDLTest_CommonState *state;

/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    SDLTest_CommonQuit(state);
    exit(rc);
}

int
main(int argc, char *argv[])
{
    int i, done;
    SDL_Event event;

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Initialize test framework */
    state = SDLTest_CommonCreateState(argv, SDL_INIT_VIDEO);
    if (!state) {
        return 1;
    }

    for (i = 1; i < argc;) {
        int consumed;

        consumed = SDLTest_CommonArg(state, i);
        /* needed voodoo to allow app to launch via OS X Finder */
        if (SDL_strncmp(argv[i], "-psn", 4)==0) {
            consumed = 1;
        }
        if (consumed == 0) {
            consumed = -1;
        }
        if (consumed < 0) {
            SDL_Log("Usage: %s %s\n", argv[0], SDLTest_CommonUsage(state));
            quit(1);
        }
        i += consumed;
    }
    if (!SDLTest_CommonInit(state)) {
        quit(2);
    }

    for (i = 0; i < state->num_windows; ++i) {
        SDL_Renderer *renderer = state->renderers[i];
        SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    /* Main render loop */
    done = 0;
    while (!done) {
        /* Check for events */
        while (SDL_PollEvent(&event)) {
            SDLTest_CommonEvent(state, &event, &done);

            if (event.type == SDL_DROPFILE) {
                char *dropped_filedir = event.drop.file;
                SDL_Log("File dropped on window: %s", dropped_filedir);
                SDL_free(dropped_filedir);
            }
        }
    }

    quit(0);
    /* keep the compiler happy ... */
    return(0);
}

/* vi: set ts=4 sw=4 expandtab: */
