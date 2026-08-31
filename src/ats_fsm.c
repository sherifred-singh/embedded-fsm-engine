/**
 * @file ats_fsm.c
 * @brief Concrete ATS State Machine Implementation.
 * @author Sherifred Singh
 */

#include "ats_fsm.h"
#include <stdio.h>

static void on_entry_mains(fsm_t *f, fsm_event_t e, void *ud) {
    (void)f; (void)e;
    ats_controller_t *ats = (ats_controller_t *)ud;
    ats->mains_relay_closed = true;
    ats->gen_relay_closed = false;
    ats->gen_run_signal = false;
    ats->timer_seconds = 0;
}

static void on_entry_crank(fsm_t *f, fsm_event_t e, void *ud) {
    (void)f; (void)e;
    ats_controller_t *ats = (ats_controller_t *)ud;
    ats->crank_attempts++;
    ats->gen_run_signal = true;
    ats->timer_seconds = 0;
}

static void on_entry_gen_transfer(fsm_t *f, fsm_event_t e, void *ud) {
    (void)f; (void)e;
    ats_controller_t *ats = (ats_controller_t *)ud;
    /* Strict break-before-make */
    ats->mains_relay_closed = false;
    ats->gen_relay_closed = false;
    ats->break_before_make_active = true;
    ats->timer_seconds = 0;
}

static void on_entry_on_gen(fsm_t *f, fsm_event_t e, void *ud) {
    (void)f; (void)e;
    ats_controller_t *ats = (ats_controller_t *)ud;
    ats->mains_relay_closed = false;
    ats->gen_relay_closed = true;
    ats->break_before_make_active = false;
}

static void on_entry_mains_transfer(fsm_t *f, fsm_event_t e, void *ud) {
    (void)f; (void)e;
    ats_controller_t *ats = (ats_controller_t *)ud;
    ats->mains_relay_closed = false;
    ats->gen_relay_closed = false;
    ats->break_before_make_active = true;
    ats->timer_seconds = 0;
}

static const fsm_state_desc_t ats_descriptors[] = {
    { ATS_STATE_ON_MAINS,           on_entry_mains,        NULL },
    { ATS_STATE_GEN_CRANKING,       on_entry_crank,        NULL },
    { ATS_STATE_TRANSFER_TO_GEN,    on_entry_gen_transfer, NULL },
    { ATS_STATE_ON_GENERATOR,       on_entry_on_gen,       NULL },
    { ATS_STATE_TRANSFER_TO_MAINS,  on_entry_mains_transfer, NULL }
};

static const fsm_transition_t ats_transitions[] = {
    { ATS_STATE_ON_MAINS,           ATS_EVENT_MAINS_LOST,     ATS_STATE_MAINS_FAIL_DELAY,   NULL, NULL },
    { ATS_STATE_MAINS_FAIL_DELAY,   ATS_EVENT_MAINS_RESTORED, ATS_STATE_ON_MAINS,           NULL, NULL },
    { ATS_STATE_MAINS_FAIL_DELAY,   ATS_EVENT_TICK_1S,        ATS_STATE_GEN_CRANKING,       NULL, NULL },
    { ATS_STATE_GEN_CRANKING,       ATS_EVENT_GEN_STARTED,    ATS_STATE_GEN_WARMUP,         NULL, NULL },
    { ATS_STATE_GEN_WARMUP,         ATS_EVENT_TICK_1S,        ATS_STATE_TRANSFER_TO_GEN,    NULL, NULL },
    { ATS_STATE_TRANSFER_TO_GEN,    ATS_EVENT_TRANSFER_DONE,  ATS_STATE_ON_GENERATOR,       NULL, NULL },
    { ATS_STATE_ON_GENERATOR,       ATS_EVENT_MAINS_RESTORED, ATS_STATE_MAINS_RESTORE_DELAY,NULL, NULL },
    { ATS_STATE_MAINS_RESTORE_DELAY,ATS_EVENT_TICK_1S,        ATS_STATE_TRANSFER_TO_MAINS,  NULL, NULL },
    { ATS_STATE_TRANSFER_TO_MAINS,  ATS_EVENT_TRANSFER_DONE,  ATS_STATE_GEN_COOLDOWN,       NULL, NULL },
    { ATS_STATE_GEN_COOLDOWN,       ATS_EVENT_TICK_1S,        ATS_STATE_ON_MAINS,           NULL, NULL },
    { ATS_STATE_GEN_CRANKING,       ATS_EVENT_GEN_CRANK_FAIL, ATS_STATE_FAULT_LOCKOUT,      NULL, NULL },
    { ATS_STATE_FAULT_LOCKOUT,      ATS_EVENT_RESET_FAULT,    ATS_STATE_ON_MAINS,           NULL, NULL }
};

void ats_controller_init(fsm_t *fsm, ats_controller_t *ats) {
    ats->timer_seconds = 0;
    ats->mains_fail_delay_limit = 3;
    ats->gen_warmup_delay_limit = 5;
    ats->mains_restore_delay_limit = 5;
    ats->gen_cooldown_delay_limit = 5;
    ats->crank_attempts = 0;
    ats->max_crank_attempts = 3;
    ats->mains_relay_closed = true;
    ats->gen_relay_closed = false;
    ats->gen_run_signal = false;
    ats->break_before_make_active = false;

    fsm_init(fsm,
             ATS_STATE_ON_MAINS,
             ats_transitions,
             sizeof(ats_transitions) / sizeof(ats_transitions[0]),
             ats_descriptors,
             sizeof(ats_descriptors) / sizeof(ats_descriptors[0]),
             ats);
}

const char* ats_state_to_string(ats_state_e state) {
    switch(state) {
        case ATS_STATE_ON_MAINS:           return "ON_MAINS";
        case ATS_STATE_MAINS_FAIL_DELAY:   return "MAINS_FAIL_DELAY";
        case ATS_STATE_GEN_CRANKING:       return "GEN_CRANKING";
        case ATS_STATE_GEN_WARMUP:         return "GEN_WARMUP";
        case ATS_STATE_TRANSFER_TO_GEN:    return "TRANSFER_TO_GEN (BREAK-BEFORE-MAKE)";
        case ATS_STATE_ON_GENERATOR:       return "ON_GENERATOR";
        case ATS_STATE_MAINS_RESTORE_DELAY:return "MAINS_RESTORE_DELAY";
        case ATS_STATE_TRANSFER_TO_MAINS:  return "TRANSFER_TO_MAINS (BREAK-BEFORE-MAKE)";
        case ATS_STATE_GEN_COOLDOWN:       return "GEN_COOLDOWN";
        case ATS_STATE_FAULT_LOCKOUT:      return "FAULT_LOCKOUT";
        default:                           return "UNKNOWN";
    }
}
