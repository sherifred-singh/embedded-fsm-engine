/**
 * @file test_fsm.c
 * @brief Unit tests and simulation for the FSM engine and ATS controller.
 * @author Sherifred Singh
 */

#include <stdio.h>
#include <assert.h>
#include "ats_fsm.h"

int main(void) {
    printf("\n=======================================================\n");
    printf(" 🚀 C Embedded FSM Engine -- ATS Controller Simulation\n");
    printf("=======================================================\n\n");

    fsm_t fsm;
    ats_controller_t ats;

    ats_controller_init(&fsm, &ats);

    printf("[1] Initial State: %s\n", ats_state_to_string(fsm_get_state(&fsm)));
    assert(fsm_get_state(&fsm) == ATS_STATE_ON_MAINS);
    assert(ats.mains_relay_closed == true);
    assert(ats.gen_relay_closed == false);

    printf("[2] Triggering Event: MAINS_LOST\n");
    fsm_dispatch(&fsm, ATS_EVENT_MAINS_LOST);
    assert(fsm_get_state(&fsm) == ATS_STATE_MAINS_FAIL_DELAY);

    printf("[3] Simulating Timer Tick -> Crank Generator\n");
    fsm_dispatch(&fsm, ATS_EVENT_TICK_1S);
    assert(fsm_get_state(&fsm) == ATS_STATE_GEN_CRANKING);
    assert(ats.gen_run_signal == true);

    printf("[4] Generator Started -> Warmup\n");
    fsm_dispatch(&fsm, ATS_EVENT_GEN_STARTED);
    assert(fsm_get_state(&fsm) == ATS_STATE_GEN_WARMUP);

    printf("[5] Warmup Done -> Transfer with Break-Before-Make\n");
    fsm_dispatch(&fsm, ATS_EVENT_TICK_1S);
    assert(fsm_get_state(&fsm) == ATS_STATE_TRANSFER_TO_GEN);
    assert(ats.break_before_make_active == true);
    assert(ats.mains_relay_closed == false);
    assert(ats.gen_relay_closed == false);

    printf("[6] Transfer Complete -> ON_GENERATOR\n");
    fsm_dispatch(&fsm, ATS_EVENT_TRANSFER_DONE);
    assert(fsm_get_state(&fsm) == ATS_STATE_ON_GENERATOR);
    assert(ats.gen_relay_closed == true);

    printf("[7] Mains Restored -> Restore Delay\n");
    fsm_dispatch(&fsm, ATS_EVENT_MAINS_RESTORED);
    assert(fsm_get_state(&fsm) == ATS_STATE_MAINS_RESTORE_DELAY);

    printf("[8] Transfer to Mains\n");
    fsm_dispatch(&fsm, ATS_EVENT_TICK_1S);
    assert(fsm_get_state(&fsm) == ATS_STATE_TRANSFER_TO_MAINS);
    assert(ats.break_before_make_active == true);

    fsm_dispatch(&fsm, ATS_EVENT_TRANSFER_DONE);
    assert(fsm_get_state(&fsm) == ATS_STATE_GEN_COOLDOWN);

    fsm_dispatch(&fsm, ATS_EVENT_TICK_1S);
    assert(fsm_get_state(&fsm) == ATS_STATE_ON_MAINS);
    assert(ats.mains_relay_closed == true);
    assert(ats.gen_relay_closed == false);

    printf("\n=======================================================\n");
    printf(" 🎉 ALL ATS FSM TESTS PASSED! FULL CYCLE VERIFIED (100%%)\n");
    printf("=======================================================\n\n");

    return 0;
}
