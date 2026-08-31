/**
 * @file fsm.c
 * @brief Generic FSM Engine Implementation.
 * @author Sherifred Singh
 */

#include "fsm.h"

static const fsm_state_desc_t* find_state_desc(const fsm_t *fsm, fsm_state_t state) {
    if (fsm->state_descriptors == NULL) return NULL;
    for (size_t i = 0; i < fsm->state_desc_count; i++) {
        if (fsm->state_descriptors[i].state == state) {
            return &fsm->state_descriptors[i];
        }
    }
    return NULL;
}

void fsm_init(fsm_t *fsm,
              fsm_state_t initial_state,
              const fsm_transition_t *transitions,
              size_t transition_count,
              const fsm_state_desc_t *state_descriptors,
              size_t state_desc_count,
              void *user_data) {
    if (fsm == NULL) return;

    fsm->current_state = initial_state;
    fsm->previous_state = FSM_STATE_NONE;
    fsm->transitions = transitions;
    fsm->transition_count = transition_count;
    fsm->state_descriptors = state_descriptors;
    fsm->state_desc_count = state_desc_count;
    fsm->user_data = user_data;

    const fsm_state_desc_t *desc = find_state_desc(fsm, initial_state);
    if (desc && desc->on_entry) {
        desc->on_entry(fsm, FSM_EVENT_NONE, fsm->user_data);
    }
}

bool fsm_dispatch(fsm_t *fsm, fsm_event_t event) {
    if (fsm == NULL || fsm->transitions == NULL) return false;

    for (size_t i = 0; i < fsm->transition_count; i++) {
        const fsm_transition_t *t = &fsm->transitions[i];

        if (t->current_state == fsm->current_state && t->event == event) {
            if (t->guard != NULL) {
                if (!t->guard(fsm, event, fsm->user_data)) {
                    continue;
                }
            }

            fsm_state_t target_state = t->next_state;

            if (target_state != fsm->current_state) {
                const fsm_state_desc_t *exit_desc = find_state_desc(fsm, fsm->current_state);
                if (exit_desc && exit_desc->on_exit) {
                    exit_desc->on_exit(fsm, event, fsm->user_data);
                }
            }

            if (t->action) {
                t->action(fsm, event, fsm->user_data);
            }

            if (target_state != fsm->current_state) {
                fsm->previous_state = fsm->current_state;
                fsm->current_state = target_state;

                const fsm_state_desc_t *entry_desc = find_state_desc(fsm, target_state);
                if (entry_desc && entry_desc->on_entry) {
                    entry_desc->on_entry(fsm, event, fsm->user_data);
                }
            }

            return true;
        }
    }
    return false;
}

fsm_state_t fsm_get_state(const fsm_t *fsm) {
    return (fsm != NULL) ? fsm->current_state : FSM_STATE_NONE;
}

fsm_state_t fsm_get_previous_state(const fsm_t *fsm) {
    return (fsm != NULL) ? fsm->previous_state : FSM_STATE_NONE;
}
