# Embedded Event-Driven Finite State Machine (FSM) Engine

A robust, table-driven **Finite State Machine (FSM) Framework** written in pure C99 for industrial embedded systems.

Eliminates nested `if-else` and sprawling `switch-case` antipatterns by modeling system behavior using clean transition matrices, guard conditions, and entry/exit hooks.

---

## Included Reference Model: Industrial ATS Controller

This repository demonstrates the engine running an **Automatic Transfer Switch (ATS)** controller with:
- **Break-Before-Make Interlocking:** Enforces a safe dead-time during power transfer to prevent short-circuiting mains and generator.
- **Fail-Safe Generator Sequencing:** Automatic warm-up, cool-down, and crank retry limits.
- **Noise-Immune Delay Timers:** Filtering power fluctuations before executing power transfers.

```text
[ MAINS HEALTHY ]
        |
  (Mains Lost)
        v
[ MAINS FAIL DELAY ] ---> (Crank) ---> [ GEN CRANKING ] ---> [ GEN WARMUP ]
                                                                   |
                                                         (Break-Before-Make)
                                                                   v
[ ON GENERATOR ] <---------------------------------------- [ TRANSFER TO GEN ]
        |
  (Mains Restored)
        v
[ MAINS RESTORE DELAY ] ---> (Break-Before-Make) ---> [ GEN COOLDOWN ] ---> [ MAINS HEALTHY ]
```

---

## Running the Simulation & Test Harness

```bash
make
```

---

## License
Released under the [MIT License](LICENSE). Designed by **Sherifred Singh**.
