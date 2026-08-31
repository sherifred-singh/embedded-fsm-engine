/**
 * @file ats_fsm.h
 * @brief Automatic Transfer Switch (ATS) Controller FSM Implementation.
 * @author Sherifred Singh
 */

#ifndef ATS_FSM_H
#define ATS_FSM_H

#include "fsm.h"

typedef enum {
    ATS_STATE_ON_MAINS = 0,
    ATS_STATE_MAINS_FAIL_DELAY,
    ATS_STATE_GEN_CRANKING,
    ATS_STATE_GEN_WARMUP,
    ATS_STATE_TRANSFER_TO_GEN,
    ATS_STATE_ON_GENERATOR,
    ATS_STATE_MAINS_RESTORE_DELAY,
    ATS_STATE_TRANSFER_TO_MAINS,
    ATS_STATE_GEN_COOLDOWN,
    ATS_STATE_FAULT_LOCKOUT,
    ATS_STATE_MAX
} ats_state_e;

typedef enum {
    ATS_EVENT_TICK_1S = 0,
    ATS_EVENT_MAINS_LOST,
    ATS_EVENT_MAINS_RESTORED,
    ATS_EVENT_GEN_STARTED,
    ATS_EVENT_GEN_CRANK_FAIL,
    ATS_EVENT_TRANSFER_DONE,
    ATS_EVENT_EMERGENCY_STOP,
    ATS_EVENT_RESET_FAULT,
    ATS_EVENT_MAX
} ats_event_e;

typedef struct {
    uint32_t timer_seconds;
    uint32_t mains_fail_delay_limit;
    uint32_t gen_warmup_delay_limit;
    uint32_t mains_restore_delay_limit;
    uint32_t gen_cooldown_delay_limit;
    uint8_t crank_attempts;
    uint8_t max_crank_attempts;
    bool mains_relay_closed;
    bool gen_relay_closed;
    bool gen_run_signal;
    bool break_before_make_active;
} ats_controller_t;

void ats_controller_init(fsm_t *fsm, ats_controller_t *ats);
const char* ats_state_to_string(ats_state_e state);

#endif /* ATS_FSM_H */
