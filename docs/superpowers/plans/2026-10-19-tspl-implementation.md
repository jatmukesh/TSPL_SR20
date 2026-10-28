# SR20 TSPL Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans for native execution, or superpowers:subagent-driven-development if the user selects delegation. Steps use checkbox syntax for tracking.

**Goal:** Implement the seven approved TSPL commands with exclusive compile-time selection and a separate asynchronous overflow path on the 384-dot SR20.

**Architecture:** A bounded parser owns one pending label. A portable job controller owns validation, confirmation, and copy lifecycle. A firmware adapter composes text into existing print-list records and uses the existing engine, media checks, and foreground services.

**Tech Stack:** C99, Keil ARMCC 5, Windows PowerShell, GCC host tests, SVN working copy.

**Spec:** `docs/superpowers/specs/2026-10-19-tspl-design.md`, approved 2026-10-19.

## Global Constraints

- Effective width: 48 mm, 384 dots, 8 dots/mm. Nominal label height: 12–75 mm.
- Default to `TSPL_PROTOCOL=1` and `ESC_POS_PROTOCOL=0` in `Code/macros.h`. Derive `PSF20_R002_TSPL_PROTOCOL` and `PSF20_R002_ESC_POS_PROTOCOL` under their respective numeric switches. Select the legacy build with `TSPL_PROTOCOL=0` and `ESC_POS_PROTOCOL=1`.
- Require each switch to be 0 or 1 and exactly one to be enabled. Use `#error` for invalid values, both enabled, or both disabled. Guard the ESC/POS implementation explicitly with `PSF20_R002_ESC_POS_PROTOCOL`.
- New implementation and integration are guarded. Changed legacy behavior remains under `#else // legacy flow`.
- No CPCL, runtime selection, protocol detection, dynamic allocation, duplicate queue, motor driver, thermal driver, sensor, or full-label bitmap.
- Fixed limits: command buffer 512 bytes, 24 TEXT operations, aggregate text pool 2048 bytes, individual text 255 bytes, copies 1–255.
- TEXT initially supports printable ASCII through `PSF12X24.FNT`, rotations 0/90/180/270, and scales 1–10. Other fonts and extensions fail explicitly.
- Prompt text is exactly `Size Overflow` and `Print? Feed = Y`; timeout is 5000 ms. Confirmation requires a fresh Feed press.
- Preserve raw detected height and calibrated nominal height separately from requested SIZE and GAP.
- Preserve the pre-existing edits in `Code/PrintCore.C`; preserve file encoding and line endings when patching legacy files.
- Follow PSF coding-style file/function headers and fixed-width types. Do not invent author or release metadata; identify unavailable author metadata explicitly in the delivery notes.
- Work in this SVN checkout; do not create a Git repository or make an SVN commit. Keep logs and a patch as local evidence.

## Review Focus

1. Empty receive storage while confirmation is pending must still expire without new input.
2. Feed held before the prompt, or pressed at the deadline, must not cause an unintended print or ordinary feed.
3. Transport overflow or media replacement must invalidate a retained job before any later PRINT can revive it.
4. Band/copy transitions must not reuse an engine-owned buffer or lose thermal/battery label boundaries.
5. Local startup/menu/self-test printing must work without retaining the ESC/POS input parser in a TSPL image.

## File responsibilities

- `Code/TsplParser.c/.h`: portable framing, token parsing, dimensions, settings, and fixed pending TEXT storage.
- `Code/TsplJob.c/.h`: portable geometry validation, immutable job lifecycle, confirmation deadline, copies, and cancellation. This small split keeps the real controller directly host-testable without MCU mocks throughout the firmware adapter.
- `Code/TsplFirmware.c/.h`: existing-media snapshots, OLED/mobile reporting, native glyph access, and composition of existing raster records.
- `Code/PrintCore.C`: guarded parser dispatch, existing list-buffer handoff, completion/error events, fresh monotonic tick, calibration invalidation, local-print entry points.
- `Code/ReceiveBufferPro.c/.h`: guarded nonblocking byte access or cooperative idle-service seam, overflow cancellation, prompt input/display ownership, and existing transport backpressure.
- `Code/macros.h`, `Code/ParseESC.c/.h`, `MDK/SR20.uvprojx`: selector and source/dependency guards. Touch other protocol-only translation units only when actual link dependencies require it.
- `tests/tspl/`: C tests, test utilities, PowerShell runner, isolated build outputs and evidence.
- `docs/tspl.md`: supported command forms, font example, limits, errors, mobile telemetry, build/test commands, board checks.

## Task 1: Establish baseline and test harness

**Interfaces:** `tests/tspl/run.ps1` runs all host suites, fails on any compiler/test error, and keeps build outputs under `tests/tspl/build/`.

- [ ] Save the initial `svn diff` and tracked-file status under the local evidence directory. Capture hashes of files that may be edited and exact tool versions.
- [ ] Run the current Keil project build before changing firmware. Use a dedicated log, read the error/warning totals, and record any existing build failure separately. Avoid changing release-checksum constants or running firmware upload tools.
- [ ] Create a minimal host assertion runner and fail it deliberately with `CHECK(0)`. Run `powershell -NoProfile -ExecutionPolicy Bypass -File tests/tspl/run.ps1` and confirm nonzero exit. Replace the deliberate failure with a passing runner check.
- [ ] Compile host suites with `gcc -std=c99 -Wall -Wextra -Werror -pedantic`. Add supported runtime checking in an additional run; do not claim unavailable sanitizers ran.

## Task 2: Implement incremental parsing and pending operations

**Interfaces produced in `TsplParser.h`:**

```c
void TsplParser_Init(TsplParser_t *parser);
TsplResult_t TsplParser_PushByte(TsplParser_t *parser, uint8_t value);
void TsplParser_Cancel(TsplParser_t *parser);
const TsplLabel_t *TsplParser_GetLabel(const TsplParser_t *parser);
```

`TsplParser_t` contains the framing state, bounded pending `TsplLabel_t`, and last result. `TsplLabel_t` contains requested settings, operation offsets into the text pool, copy count, validity, and print-request state. It contains no firmware pointers. A PRINT result transfers control to the controller before another byte is consumed.

- [ ] Write failing `test_tspl_parser.c` cases for a valid seven-command stream, text containing commas, all split positions, several commands together, empty input, and incomplete final PRINT. Assert exact pending settings/operations and that incomplete PRINT never requests submission.
- [ ] Add failing unit cases for `SIZE 48 mm,12 mm`, equivalent inch/dot dimensions, 385-dot width, heights below 96 or above 600 dots, invalid units/numbers, missing/extra arguments, unmatched quotes, NUL, oversized input, 25 operations, and text/pool limits.
- [ ] Add failing cases for unknown commands, unavailable fonts, mirror/alignment/multiple-set arguments, invalid rotation/scale/copies, error latching, atomic command updates, CLS retaining settings, and framing recovery after overflow. Assert a rejected command never leaves the old drawing set executable.
- [ ] Run tests and record RED before implementing `TsplParser.c/.h` and the selector in `macros.h`.
- [ ] Use integer arithmetic with checked conversion and nearest-dot rounding. Verify supported quote/unit rules against the TSC reference; reject grammar extensions explicitly. Retain requested unit precision needed for truthful height reporting.
- [ ] Run the complete host suite and record GREEN. Check parser/job static sizes against the declared limits.

## Task 3: Implement job validation and asynchronous confirmation

**Interfaces produced in `TsplJob.h`:**

```c
void TsplJob_Init(TsplJob_t *job, const TsplJobPorts_t *ports);
TsplResult_t TsplJob_Request(TsplJob_t *job, const TsplLabel_t *label);
void TsplJob_Service(TsplJob_t *job, const TsplInputs_t *inputs);
void TsplJob_Cancel(TsplJob_t *job, TsplResult_t reason);
uint8_t TsplJob_OwnsFeed(const TsplJob_t *job);
```

`TsplInputs_t` contains monotonic milliseconds, current Feed level/event, media generation, detected/nominal height, effective horizontal bounds, calibration validity, readiness, and existing-engine completion/error state. `TsplJobPorts_t` supplies prompt/report/restore/submit callbacks with explicit result handling. The job borrows the parser's immutable label until resolved; no duplicate queue or label copy is required.

- [ ] Write failing `test_tspl_job.c` cases for valid submission, no media, stale/invalid calibration, open lid, unsupported nominal height, horizontal/design bounds, readiness errors, and submission rejection. Assert zero handoffs for all preflight failures.
- [ ] Add geometry cases covering all four rotations, both directions, nonzero reference/margins, signed arithmetic boundaries, overlap, and scaled extents.
- [ ] Add overflow cases asserting exact OLED strings, requested/detected reporting, 4999/5000 ms behavior, a fresh press, held-before-entry rejection, timeout winning at the boundary, release tracking, and uint32 tick wraparound. Service with no incoming bytes.
- [ ] Add cases for media-generation change, readiness failure after Feed, receive cancellation, no stale reprint after timeout, copy revalidation, and hardware error cancelling remaining copies.
- [ ] Record RED, implement `TsplJob.c/.h`, then run the full host suite to GREEN. Keep each service call bounded; no delay or polling loop belongs in the controller.

## Task 4: Compose text using native glyphs and existing raster records

**Interfaces produced in `TsplFirmware.h`:**

```c
void TsplFirmware_Init(void);
void TsplFirmware_Service(void);
TsplResult_t TsplFirmware_Consume(uint8_t value);
void TsplFirmware_ReceiveError(void);
uint8_t TsplFirmware_OwnsFeed(void);
```

Private adapter helpers read `PSF12X24.FNT` glyphs through `ReadInFlashAndDeg()`, transform coordinates, and fill an available native print list. Firmware-specific list publication remains in `PrintCore.C`, where the buffer ownership state already lives.

- [ ] Write failing tests comparing known glyph pixels with composed bands, including text overlap, empty rows, nonzero origins, rotation/direction, and the final short band. Surround output storage with canaries and assert no writes outside it.
- [ ] Write failing record tests asserting `PRINT_MAP`, width 384 at scale 1, band height at scale 1, zero trailing alignment, correct byte/word order, index termination, and exact row count. Verify a maximum 24-row band fits the selected `PrintListPar` storage.
- [ ] Record RED; implement only glyph placement/composition and native record construction. Reuse native glyph data rather than adding a font or a full-label canvas.
- [ ] Run all host tests to GREEN. Document any compositor helper split needed to keep MCU-independent geometry directly testable.

## Task 5: Integrate receive, print lifecycle, media, and prompt ownership

**Consumes:** parser and controller APIs above; native print-list ownership and completion state; existing PDL callbacks; `Get_Oversize_Detection_BT_Response()`.

- [ ] Add failing integration tests for no-input expiry, active-receive service, immutable pending ownership, transport overflow, prompt-only Feed consumption, and prompt display ownership. Exercise the actual controller/adapter code with test ports, not a second test-only state machine.
- [ ] Add failing handoff tests proving producer-owned-only writes, final-band completion before slot release, final-copy completion, cancellation of remaining work on hardware error, and correct label-boundary callbacks.
- [ ] Record RED. Add a TSPL monotonic tick incremented by `SysTickProFun()` because existing `g_sysTime` is reset by unrelated foreground code. Keep tick storage/calls guarded.
- [ ] Add guarded TSPL dispatch at `Handler_main()` and service hooks within the existing idle receive foreground path. Never call a blocking byte fetch while waiting for confirmation. Preserve battery, thermal, watchdog, USB/BT flow-control, and media service calls.
- [ ] Use existing media checks and invalidate measurement validity after lid/media change until calibration completes. Read raw and nominal height separately. Cap configured effective width at the 384-dot head and include existing offsets.
- [ ] Apply job GAP/offset only through an existing supported feed-position seam and keep calibration values intact. Where the hardware seam cannot represent an argument, reject it explicitly; do not store-and-ignore it. Report any required restriction as a delivery limitation.
- [ ] Add prompt ownership around ordinary Feed/self-test handling and competing OLED refreshes. Preserve a fresh event, consume it once, and release ownership only after safe release/cancellation handling.
- [ ] Report oversize through the existing message function with requested and detected values in its existing wire order. Add bounded `TSPL_ERROR:<category>\r\n` reporting for other TSPL failures through the existing response facilities; test categories and lengths.
- [ ] Use native A/B list publication, IRQ activation, completion, feed/stop flags, thermal checks, and low-battery label accounting. Do not route generated commands through ESC/POS.
- [ ] Run all host/integration tests to GREEN.

## Task 6: Make protocol builds exclusive and preserve local printing

**Files:** `Code/macros.h`, `Code/ParseESC.c/.h`, protocol-dependent sources actually referenced by `MDK/SR20.uvprojx`, and guarded call sites in `PrintCore.C`/`ReceiveBufferPro.c`.

- [ ] Add failing configuration checks for TSPL/ESC_POS combinations 1/0 and 0/1, each derived symbol's presence/absence, no opposite-parser entry symbols, and no duplicate definitions. Assert compile failure for 0/0, 1/1, and either switch outside 0/1. Define both switches in a way that permits isolated test builds to override them without editing the user's defaults.
- [ ] Audit startup self-test, menu printing, calibration feedback, and internally injected text. Add tests for their selected-protocol entry path so ASCII cannot accidentally become TSPL commands and ESC handlers are not retained solely for local printing.
- [ ] Record RED, add source-body/dependency guards and project entries, and implement the minimal local-print seam using shared engine facilities.
- [ ] Build both configurations from current sources with Keil, keeping separate logs/artifacts. Use the project/build command's actual exit code and inspect compile/link summaries. Restore the requested default after checks.
- [ ] Inspect both linked symbol lists and map files, record RAM/flash totals, and run the full host suite. Distinguish successful compilation from linking and successful linking from hardware execution.

## Task 7: Review and deliver evidence

- [ ] Run a separate review of all changed code for buffer bounds, signed/unsigned conversions, ISR sharing, job invalidation, and preserved legacy branches. Use the approved execution method's review workflow; do not delegate implementation unless the user selects it.
- [ ] Reproduce important findings with failing tests, apply focused fixes, then rerun the whole host suite and any affected protocol builds.
- [ ] Write `docs/tspl.md` with supported command forms and this example:

```text
SIZE 48 mm,30 mm
GAP 3 mm,0 mm
DIRECTION 1
REFERENCE 0,0
CLS
TEXT 8,8,"PSF12X24.FNT",0,1,1,"Hello, SR20"
PRINT 1,1
```

- [ ] Document that the example requires matching calibrated media/feed capabilities; list font/argument limits and deliberately deferred commands. Include exact host/build commands and board checks not performed.
- [ ] Save a final local patch and concise execution ledger recording test counts, build outcomes, memory totals, restrictions, and review findings. Compare the final patch against the initial patch to confirm the user's original edits remain intact.
- [ ] Deliver changed-file links, supported/deferred scope, actual checks run, and unresolved board/application verification. Do not claim the implementation complete if a required integration remains unimplemented.

## Plan review and execution

Recommended method: native execution in this chat, followed by a fresh final review. Parser, controller, and adapter depend on the same ownership decisions, so a single implementer avoids integration drift. The alternative is subagent-driven task execution with independent review at each task boundary.

The written plan requires user review before firmware edits under the requested Superpowers workflow. The approved design remains the source of requirements.
