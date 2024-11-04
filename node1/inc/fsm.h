/**
 * @file fsm.h
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef FSM_H
#define FSM_H

#include <stddef.h> // Required for size_t
#include <stdbool.h>

#include "defines.h"
#include "drivers/joystick.h"
#include "drivers/oled.h"
#include "drivers/button.h"
#include "drivers/menu.h"
#include "drivers/can.h"
#include "logger.h"

/*****************************************************************************/
/* Typedefs                                                                  */
/*****************************************************************************/

/**
 * @brief Enum for FSM states
 *
 * @param FSM_MENU The menu state
 * @param FSM_GAME The game state
 * @param FSM_GAME_OVER The game over state
 * @param FSM_CALIBRATION The calibration state
 */
typedef enum FSM_STATE {
    FSM_MENU,
    FSM_GAME,
    FSM_GAME_OVER,
    FSM_CALIBRATION,
} e_FSM_STATE;

/**
 * @brief Struct for the different states and their operations
 *
 * @param state Unique identifier for the state
 * @param name The name of the state
 * @param operation The operation to be performed when in the state
 */
typedef struct state_config_s {
    e_FSM_STATE state;
    char       *name;
    void (*operation)(void);
} state_config_t;

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/

void        fsm(void);
e_FSM_STATE fsm_get_state(void);
void        fsm_set_state(e_FSM_STATE state);

void        fsm_goto_game(void);
void        fsm_goto_menu(void);
void        fsm_goto_calibration(void);

#endif // FSM_H
