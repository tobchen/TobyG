/*
 * gamemgr.c
 *
 *  Created on: Dec 11, 2016
 *      Author: tobchen
 */

#include "gamemgr.h"

typedef struct tobyg_gamemgr_state {
	Uint8 id;
	int (*start)(void);
	int (*stop)(void);
	int (*loop)(void);
	struct tobyg_gamemgr_state* next;
} TobyG_GameMgrState;

static TobyG_GameMgrState* firstState;
static TobyG_GameMgrState* currentState;
static TobyG_GameMgrState* nextState;

static Uint8 isRunning;

static TobyG_GameMgrState* getState(Uint8 id);

static int stateError;

void TobyG_StartGameMgr(void) {
	firstState = NULL;
	currentState = NULL;
	nextState = NULL;
}

void TobyG_EndGameMgr(void) {
	while (firstState != NULL) {
		TobyG_GameMgrState* tmp = firstState;
		firstState = tmp->next;
		free(tmp);
	}
}

int TobyG_AddGameMgrState(Uint8 id, int (*start)(void), int (*stop)(void),
		int (*loop)(void)) {
	TobyG_GameMgrState* tmp;

	if (NULL == start || NULL == stop || NULL == loop) {
		return -1;
	}
	for (tmp = firstState; tmp != NULL; tmp = tmp->next) {
		if (tmp->id == id) {
			return -1;
		}
	}

	tmp = malloc(sizeof(TobyG_GameMgrState));
	if (NULL == tmp) {
		return -1;
	}
	tmp->id = id;
	tmp->start = start;
	tmp->stop = stop;
	tmp->loop = loop;
	tmp->next = firstState;
	firstState = tmp;

	return 0;
}

int TobyG_ChangeGameMgrState(Uint8 id) {
	TobyG_GameMgrState* tmp = getState(id);
	if (NULL == tmp) {
		return -1;
	}
	nextState = tmp;
	return 0;
}

TobyG_GameMgrRuntimeError TobyG_RunGameMgr(Uint8 id) {
	currentState = getState(id);
	if (NULL == currentState) {
		return TGAMEMGR_START_STATE_NOT_FOUND;
	}
	stateError = currentState->start();
	if (stateError) {
		return TGAMEMGR_ERROR_ON_STATE_START;
	}

	isRunning = 1;
	while (isRunning) {
		if (nextState != NULL) {
			stateError = currentState->stop();
			if (stateError) {
				return TGAMEMGR_ERROR_ON_STATE_STOP;
			}

			currentState = nextState;
			nextState = NULL;

			stateError = currentState->start();
			if (stateError) {
				return TGAMEMGR_ERROR_ON_STATE_START;
			}
		}

		currentState->loop();
		if (stateError) {
			currentState->stop();
			return TGAMEMGR_ERROR_ON_STATE_LOOP;
		}
	}

	stateError = currentState->stop();
	if (stateError) {
		return TGAMEMGR_ERROR_ON_STATE_STOP;
	}

	return TGAMEMGR_NO_ERROR;
}

void TobyG_QuitGameMgr(void) {
	isRunning = 0;
}

int TobyG_GetGameMgrError(void) {
	return stateError;
}

static TobyG_GameMgrState* getState(Uint8 id) {
	TobyG_GameMgrState* tmp;
	for (tmp = firstState; tmp != NULL; tmp = tmp->next) {
		if (tmp->id == id) {
			break;
		}
	}
	return tmp;
}
