/**
 * @file fsm.c
 * @author Theodor Johansson, Paulius Juzenas & William Stensrud
 * @brief
 * @version 0.1
 * @date 2024-10-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "fsm.h"

/*****************************************************************************/
/* Local function declarations                                               */
/*****************************************************************************/

void            fsm_menu(void);
void            fsm_game(void);
void            fsm_game_over(void);
void            fsm_calibration(void);

state_config_t *fsm_find_state(e_FSM_STATE state);

/*****************************************************************************/
/* Variable declarations                                                     */
/*****************************************************************************/

// Array of states
static state_config_t states[] = {
  {FSM_MENU,        "Menu",        fsm_menu       },
  {FSM_GAME,        "Game",        fsm_game       },
  {FSM_GAME_OVER,   "Game Over",   fsm_game_over  },
  {FSM_CALIBRATION, "Calibration", fsm_calibration},
};
size_t states_size = sizeof(states) / sizeof(states[0]);

// Current state
static e_FSM_STATE current_state = FSM_MENU;

// Previously read input direction
static e_JOYSTICK_DIR joystick_previous_dir = JOYSTICK_CENTER;

/*****************************************************************************/
/* Function definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 *
 */
state_config_t *fsm_find_state(e_FSM_STATE state) {
    for (int i = 0; i < states_size; i++) {
        if (states[i].state == state) {
            return &states[i];
        }
    }

    return NULL;
}

/**
 * @brief Main function for the elevator state machine
 *
 */
void fsm(void) {
    // Get current state
    state_config_t *current_state_config = fsm_find_state(current_state);
    if (current_state_config == NULL) {
        log_warning("Could not find state: %d", current_state);
        return;
    }

    // Run operation for current state
    current_state_config->operation();
}

/**
 * @brief
 *
 */
void fsm_menu(void) {
    // Read joystick
    joystick_t joystick    = joystick_read();
    bool       dir_changed = joystick.current_dir != joystick_previous_dir;
    if (dir_changed) {
        joystick_previous_dir = joystick.current_dir;
    }
    bool joy_up   = (joystick.current_dir == JOYSTICK_UP && dir_changed);
    bool joy_down = (joystick.current_dir == JOYSTICK_DOWN && dir_changed);
    int  dir      = joy_down - joy_up;
    menu_move_arrow(dir);

    // Move between pages
    if (button_right_pressed()) {
        menu_activate_selection();
    }

    // Draw menu
    menu_draw_list();
}

/**
 * @brief
 *
 */
void fsm_calibration(void) {
    const char *axis_names[5];
    axis_names[JOYSTICK_CENTER] = "center";
    axis_names[JOYSTICK_UP]     = "top";
    axis_names[JOYSTICK_DOWN]   = "bottom";
    axis_names[JOYSTICK_RIGHT]  = "right";
    axis_names[JOYSTICK_LEFT]   = "left";

    uint8_t         calibration_samples = 100;
    static uint8_t  progress            = 0;
    static int      current_axis        = 0;
    static uint16_t calx                = 0;
    static uint16_t caly                = 0;
    static uint8_t  current_delay       = 0;
    const uint8_t   calibration_delay   = 75; // 75 samples * 50Hz = 1.5s

    // Non-blocking delay
    if (oled_should_flush() && current_delay < calibration_delay) {
        current_delay++;
    }
    if (current_delay < calibration_delay) {
        menu_draw_calibration(axis_names[current_axis], 0.0f);
        return;
    }

    joystick_t joystick = joystick_read_raw();
    calx += joystick.raw_x;
    caly += joystick.raw_y;
    progress++;

    if (progress == calibration_samples) {
        joystick_config_t *config = joystick_get_config();
        if (current_axis == JOYSTICK_CENTER) {
            config->x_config.center = calx / calibration_samples;
            config->y_config.center = caly / calibration_samples;
        } else if (current_axis == JOYSTICK_UP) {
            config->y_config.max = caly / calibration_samples;
        } else if (current_axis == JOYSTICK_DOWN) {
            config->y_config.min = caly / calibration_samples;
        } else if (current_axis == JOYSTICK_RIGHT) {
            config->x_config.max = calx / calibration_samples;
        } else if (current_axis == JOYSTICK_LEFT) {
            config->x_config.min = calx / calibration_samples;
        }

        // Reset variables
        calx          = 0;
        caly          = 0;
        progress      = 0;
        current_delay = 0;
        current_axis++;
        if (current_axis == JOYSTICK_ENUM_LENGTH) {
            config->calibrated = true;
            current_axis       = 0;

            log_info("Joystick calibrated");
            log_info("Joystick X: %u/%u/%u", config->x_config.min, config->x_config.center, config->x_config.max);
            log_info("Joystick Y: %u/%u/%u", config->y_config.min, config->y_config.center, config->y_config.max);

            fsm_goto_menu();
        }
    }

    // Draw menu
    menu_draw_calibration(axis_names[current_axis], (float)(progress + 1) / calibration_samples);
}

/**
 * @brief
 *
 */
void fsm_game(void) {
    // Read joystick
    joystick_t joystick = joystick_read();

    // Transmit virtual button
    if (button_right_pressed()) {
        can_message_s t_message = {
          .data   = {1},
          .length = 1,
          .id     = CAN_ID_BUTTON,
        };
        can_transmit(&t_message);
    }

    // Transmit CAN message
    can_message_s t_message = {
      .data   = {joystick.x, joystick.y},
      .length = 2,
      .id     = CAN_ID_JOYSTICK,
    };
    can_transmit(&t_message);

    // Receive CAN message
    can_message_s r_message;
    bool          can_receive_status = can_receive(&r_message); // Should this use mailboxes or interrupts?
    if (can_receive_status) {
        log_debug("Received CAN message: %u: %s", r_message.id, r_message.data);
    }
}

/**
 * @brief
 *
 */
void fsm_game_over(void) {
    // Pass
}

/**
 * @brief
 *
 * @return e_FSM_STATE
 */
e_FSM_STATE fsm_get_state(void) {
    // Return
    return current_state;
}

/**
 * @brief
 *
 * @param state
 */
void fsm_set_state(e_FSM_STATE state) {
    // Set current state
    current_state = state;
}

void fsm_goto_game(void) { fsm_set_state(FSM_GAME); }
void fsm_goto_menu(void) { fsm_set_state(FSM_MENU); }
void fsm_goto_calibration(void) { fsm_set_state(FSM_CALIBRATION); }
