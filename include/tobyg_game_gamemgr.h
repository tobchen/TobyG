/*
 * tobyg_game_gamemgr.h
 *
 *  Created on: Dec 11, 2016
 *      Author: tobchen
 */

#ifndef TOBYG_INCLUDE_TOBYG_GAME_GAMEMGR_H
#define TOBYG_INCLUDE_TOBYG_GAME_GAMEMGR_H

#include <SDL2/SDL.h>

typedef enum tobyg_gamemgr_runtime_error {
	TGAMEMGR_NO_ERROR,
	TGAMEMGR_START_STATE_NOT_FOUND,
	TGAMEMGR_ERROR_ON_STATE_START,
	TGAMEMGR_ERROR_ON_STATE_STOP,
	TGAMEMGR_ERROR_ON_STATE_LOOP
} TobyG_GameMgrRuntimeError;

/*!
 * \brief Start the game management system.
 */
void TobyG_StartGameMgr(void);

/*!
 * \brief End the game management system.
 */
void TobyG_EndGameMgr(void);

/*!
 * \brief Add a game state.
 * @param id game state's id
 * @param start start function
 * @param stop end function
 * @param loop loop function
 * @return -1 on error, 0 on success
 */
int TobyG_AddGameMgrState(Uint8 id, int (*start)(void), int (*stop)(void),
		int (*loop)(void));

/*!
 * \brief Change to new game state.
 * @param id new game state's id
 * @return -1 on error, 0 on success
 */
int TobyG_ChangeGameMgrState(Uint8 id);

/*!
 * \brief Run the game manager (starting the main loop).
 * @param id starting state's id
 * @return error type (or no error)
 */
TobyG_GameMgrRuntimeError TobyG_RunGameMgr(Uint8 id);

/*!
 * \brief Quit the game manager (quitting the main loop).
 */
void TobyG_QuitGameMgr(void);

/*!
 * \brief Get a state's error id.
 * @return state's error id
 */
int TobyG_GetGameMgrError(void);

#endif /* TOBYG_INCLUDE_TOBYG_GAME_GAMEMGR_H */
