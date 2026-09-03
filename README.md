# ⚙️ Embedded Event-Driven Finite State Machine (FSM) Engine

[![C/C++ FSM CI](https://github.com/Sherifred123/embedded-fsm-engine/actions/workflows/ci.yml/badge.svg)](https://github.com/Sherifred123/embedded-fsm-engine/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

A robust, table-driven **Finite State Machine (FSM) Framework** written in pure C99 for industrial embedded systems (**PIC18**, **STM32**, **ESP32**).

Eliminates nested `if-else` and sprawling `switch-case` antipatterns by modeling system behavior using clean transition matrices, guard conditions, and entry/exit hooks.

---

## 🏭 Included Reference Model: Industrial ATS Controller

This repository demonstrates the engine running an **Automatic Transfer Switch (ATS)** controller with:
- **Break-Before-Make Interlocking:** Enforces a safe dead-time during power transfer to prevent short-circuiting mains and generator.
- **Fail-Safe Generator Sequencing:** Automatic warm-up, cool-down, and crank retry limits.
- **Noise-Immune Delay Timers:** Filtering power fluctuations before executing power transfers.
- **Emergency Stop & Fault Lockout:** Instant relay shedding with manual reset safety interlock.

```text
┌─────────────────┐
│  MAINS HEALTHY  │
└────────┬────────┘
         │ (Mains Lost)
         ▼
┌──────────────────────┐
│  MAINS FAIL DELAY    │ ───(Crank)───► ┌────────────────┐ ───(Gen Started)───► ┌──────────────┐
└──────────────────────┘                │  GEN CRANKING  │                      │  GEN WARMUP  │
                                        └────────────────┘                      └──────┬───────┘
                                                                                       │ (Break-Before-Make)
                                                                                       ▼
┌─────────────────┐                                                         ┌──────────────────┐
│  ON GENERATOR   │ ◄───────────────────────────────────────────────────────│ TRANSFER TO GEN  │
└────────┬────────┘                                                         └──────────────────┘
         │ (Mains Restored)
         ▼
┌──────────────────────┐
│ MAINS RESTORE DELAY  │ ───(Break-Before-Make)───► ┌──────────────┐ ───(Cooldown Done)───► [ MAINS HEALTHY ]
└──────────────────────┘                            │ GEN COOLDOWN │
                                                    └──────────────┘
```

---

## 🧪 Running the Simulation & Test Harness

```bash
make
```

### Expected Output:
```text
=======================================================
 🚀 C Embedded FSM Engine -- ATS Controller Simulation
=======================================================

[1] Initial State: ON_MAINS
[2] Triggering Event: MAINS_LOST
[3] Simulating Timer Tick -> Crank Generator
[4] Generator Started -> Warmup
[5] Warmup Done -> Transfer with Break-Before-Make
[6] Transfer Complete -> ON_GENERATOR
[7] Mains Restored -> Restore Delay
[8] Transfer to Mains
[9] Simulating Emergency Stop Interlock -> FAULT_LOCKOUT

=======================================================
 🎉 ALL ATS FSM TESTS PASSED! FULL CYCLE VERIFIED (100%)
=======================================================
```

---

## 📜 License
Released under the [MIT License](LICENSE). Designed and maintained by **Sherifred Singh**.
