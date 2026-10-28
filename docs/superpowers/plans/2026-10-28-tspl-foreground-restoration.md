# TSPL Foreground Restoration Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Restore normal printer foreground behavior while keeping TSPL as the selected print protocol.

**Architecture:** The legacy foreground lifecycle remains authoritative for UI, Bluetooth, safety, and media. The TSPL branch substitutes only bounded TSPL receive/service work for ESC/POS parsing and avoids legacy post-print label alignment.

**Tech Stack:** C99, ARMCC/Keil MDK, existing PDL/native raster engine, GCC host tests.

**Spec:** `docs/superpowers/specs/2026-10-28-tspl-foreground-restoration-design.md`

## Global Constraints

- TSPL enabled, ESC/POS disabled in the delivered source.
- Preserve the ESC/POS branch unchanged.
- Test before production changes; build both macro configurations.

## Review Focus

- Menu timeout must redraw the home screen exactly once after leaving the menu.
- Bluetooth status refresh must occur while media is absent.
- A completed TSPL `PRINT 1,1` must not call legacy label alignment/feed.
- TSPL parser consumption must not invoke the ESC/POS parser.
- A real media fault must still reject a TSPL job without blocking the foreground.

---

### Task 1: Preserve foreground services

**Files:**
- Modify: `Code/PrintCore.C`
- Test: `tests/tspl/test_firmware.c`

**Interfaces:**
- Produces: TSPL foreground behavior that services menu, Bluetooth display, and TSPL work independently of media readiness.

- [ ] Write a failing test for the desired media-independent service behavior.
- [ ] Run the focused test and observe failure.
- [ ] Modify the TSPL foreground branch to preserve platform service ordering while replacing only parser work.
- [ ] Run the focused test and observe success.

### Task 2: Correct home and post-print behavior

**Files:**
- Modify: `Code/PrintCore.C`, `Code/TsplNative.c`
- Test: `tests/tspl/test_native.c`

**Interfaces:**
- Produces: home-screen restoration on menu timeout and no automatic extra-label feed after TSPL completion.

- [ ] Write failing tests for the native completion contract.
- [ ] Run the focused test and observe failure.
- [ ] Remove TSPL post-print use of `RunFeedLabPaper()` and add the foreground home transition.
- [ ] Run the focused test and observe success.

### Task 3: Verify selected and disabled builds

**Files:**
- Modify: `Code/macros.h`, `tests/tspl/run.ps1`

- [ ] Run the full TSPL host suite.
- [ ] Build TSPL and inspect the produced firmware image.
- [ ] Build ESC/POS after a temporary macro switch, then restore TSPL selection.
