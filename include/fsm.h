/**
 * @file fsm.h
 * @brief Lightweight, table-driven Finite State Machine (FSM) Engine in C.
 * @author Sherifred Singh
 */

#ifndef FSM_H
#define FSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint32_t fsm_state_t;
typedef uint32_t fsm_event_t;

#define FSM_STATE_NONE  ((fsm_state_t)0xFFFFFFFF)
#define FSM_EVENT_NONE  ((fsm_event_t)0xFFFFFFFF)

typedef struct fsm_s fsm_t;

typedef void (*fsm_action_fn)(fsm_t *fsm, fsm_event_t event, void *user_data);
typedef bool (*fsm_guard_fn)(const fsm_t *fsm, fsm_event_t event, const void *user_data);

typedef struct {
    fsm_state_t current_state;
    fsm_event_t event;
    fsm_state_t next_state;
    fsm_guard_fn guard;
    fsm_action_fn action;
} fsm_transition_t;

typedef struct {
    fsm_state_t state;
    fsm_action_fn on_entry;
    fsm_action_fn on_exit;
} fsm_state_desc_t;

struct fsm_s {
    fsm_state_t current_state;
    fsm_state_t previous_state;
    const fsm_transition_t *transitions;
    size_t transition_count;
    const fsm_state_desc_t *state_descriptors;
    size_t state_desc_count;
    void *user_data;
};

void fsm_init(fsm_t *fsm,
              fsm_state_t initial_state,
              const fsm_transition_t *transitions,
              size_t transition_count,
              const fsm_state_desc_t *state_descriptors,
              size_t state_desc_count,
              void *user_data);

bool fsm_dispatch(fsm_t *fsm, fsm_event_t event);
fsm_state_t fsm_get_state(const fsm_t *fsm);
fsm_state_t fsm_get_previous_state(const fsm_t *fsm);

#ifdef __cplusplus
}
#endif

#endif /* FSM_H */
/* MISRA-C:2012 Rule 8.4 Compliant Table-Driven State Machine Engine */
