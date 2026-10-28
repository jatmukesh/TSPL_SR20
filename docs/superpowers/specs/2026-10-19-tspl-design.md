# SR20 TSPL integration design

Date: 2026-10-19  
Status: Approved by the user on 2026-10-19; firmware implementation has not started.

## Intent and confirmed requirements

Add an incremental TSPL parser to PSF20 R002 V4.1.0. Select it exclusively at compile time while preserving the ESC/POS build. The initial commands are SIZE, GAP, DIRECTION, REFERENCE, CLS, TEXT, and PRINT.

The user corrected the original request on 2026-10-19: the head has 384 dots, the resolution is 8 dots/mm, and effective print width is 48 mm. The original 72 mm requirement does not apply. Supported nominal label heights remain 12–75 mm.

The user also authorized a separate asynchronous Size Overflow path for TSPL. The existing blocking ESC/POS prompt remains available in the legacy build. Both paths reuse existing display, input, communication, media, and print hardware facilities.

## Existing architecture and integration points

- `Source/main.c` installs PDL callbacks and starts peripherals before entering `Handler_main()` in `Code/PrintCore.C`.
- `Code/ReceiveBufferPro.c` receives USB, Bluetooth, and serial data into the existing receive storage. `PrinterCore_RAM()` consumes bytes and services foreground work while waiting for input. It blocks internally when storage is empty, so a service call only beside the parser is insufficient for timeout handling.
- `Handler_main()` selects the A/B `PrintListPar` buffer, parses commands, and submits completed lists using the existing print-status and producer/consumer handshake. `ProcessMotoLineData()` and related interrupt handlers own physical printing.
- `Code/ParseESC.c` currently handles commands and constructs print records. It is closely coupled to shared firmware state; it is not a reusable incremental line tokenizer.
- `Code/ParseMatrix.c` provides dot transformations. `Code/flash.c` supplies glyph data and glyph readers. `PRINT_MAP`, `PRINT_DATA`, `PRINT_SPACE`, and `LIST_END` are existing engine records.
- `Label_Size_Calculation()` in `PrintCore.C` derives `label_length_mm` from measured boundaries and maps tolerances to nominal `g_label_size_final` values. TSPL must not overwrite these values with SIZE or GAP arguments.
- `ShowSizeMismatchPrompt()` in `PrintCore.C` currently polls Feed for 5000 ms and waits for release. TSPL will not call this blocking function.
- `Get_Oversize_Detection_BT_Response(requestedMm, detectedMm)` already reports both heights. Its wire format places detected height before requested height: `SRS20_EVENT:OVERSIZE,<detected>,<requested>\r\n`.
- `MDK/SR20.uvprojx` is the Keil ARMCC 5 project and selects `PRINTER_58MM`. There is no active TSPL selector. GCC and ARMCC executables are present; successful compilation and licensing have not been established.
- The checkout is SVN revision 175. Existing local changes in `PrintCore.C` are comments on battery state and must be preserved. No repository commit is part of this task.

## Approach and alternatives

Use a bounded parser and pending job, followed by a small adapter to the existing print-list pipeline. This keeps commands independent of motor and thermal-head timing and permits host tests without MCU headers.

Routing TSPL through generated ESC/POS bytes is rejected: it would retain another command parser in the selected build and inherit its blocking reads and command side effects. A separate full-label bitmap and independent engine are also rejected: they duplicate existing facilities and add unnecessary RAM.

## Compile-time configuration

Add two explicit numeric switches in `Code/macros.h`: `TSPL_PROTOCOL`, defaulting to 1, and `ESC_POS_PROTOCOL`, defaulting to 0. The user explicitly requested an ESC/POS enable/disable macro as well as TSPL selection. Under their respective `#if` blocks, derive `PSF20_R002_TSPL_PROTOCOL` and `PSF20_R002_ESC_POS_PROTOCOL`. To select the legacy build, set `TSPL_PROTOCOL=0` and `ESC_POS_PROTOCOL=1`.

Reject values other than 0 or 1 and reject both-enabled or both-disabled configurations with `#error`. Exactly one incoming protocol must be selected. Guard ESC/POS implementation with its own derived symbol; do not leave its inclusion implicit merely because TSPL is disabled.

Guard all TSPL declarations, state, implementation, and integration calls. Keep replacement legacy branches under `#else` with a `// legacy flow` comment. No runtime protocol selection or detection is introduced.

List new source files in the Keil project, but use translation-unit guards so the unselected implementation generates no protocol code or state. Guard ESC/POS-only source bodies and dependencies out of TSPL builds. Shared glyph readers, list records, engine routines, and local UI printing remain available independently of the incoming command parser. Verify absence of unselected parser symbols in the linked image.

## Parser and pending-job ownership

Proposed modules are `TsplParser.c/.h` for framing, arguments, and pending operations, and `TsplFirmware.c/.h` for firmware submission, status, and overflow integration. These names describe new modules, not existing hardware APIs.

The parser uses fixed storage: a 512-byte command buffer including its terminator, at most 24 TEXT operations, and a 2048-byte aggregate text pool including terminators. A single content string is limited to 255 bytes. Capacity errors are explicit and invalidate the pending drawing set. No heap allocation occurs while receiving, parsing, or submitting.

Input can end at any byte. CRLF and LF terminate commands; CRLF split across receive events is one terminator. Quoted fields retain commas. Unfinished input remains buffered and cannot print. Unterminated quotes at a command boundary, embedded NUL, invalid control bytes, malformed numeric fields, and oversized commands produce errors. Oversized input is discarded through the next command boundary before framing resumes.

Each command is parsed and validated before changing settings or appending an operation. Syntax, range, capacity, and unsupported-command failures latch the drawing set invalid. PRINT cannot execute an earlier partial design after an error. A valid CLS clears operations and the drawing error latch; invalid settings remain invalid until corrected. Empty lines have no effect.

At PRINT the complete validated job becomes immutable until submission, confirmation, completion, or cancellation resolves it. There is one label slot and no new queue. During an overflow wait or active submission, pause command consumption and use existing receive backpressure. Receive overflow cancels the retained job and requires framing recovery; it must never leave an executable truncated stream.

## Initial command contract

Use the TSC programming manual as the syntax reference: <https://fs.tscprinters.com/en/dl/1/2541>. The implementation and tests must agree on accepted argument forms; unsupported forms return an explicit unsupported-argument result.

| Command | Initial behavior |
| --- | --- |
| SIZE | Parse width and height with supported inch, mm, and dot forms. Retain requested dimensions separately from detected media. Require positive width no greater than 384 dots and nominal design height within 12–75 mm. |
| GAP | Parse gap and offset into job settings. Validate against existing feed/sensor limits. Do not alter measured dimensions or persist calibration from a host command. Reject continuous-media requests in this initial label-only scope. |
| DIRECTION | Support directions 0 and 1 using the documented coordinate transformation. Support the optional non-mirrored form; reject mirroring rather than ignore it. |
| REFERENCE | Parse origin offsets in dots. Validate transformed content bounds at PRINT. |
| CLS | Clear drawing operations while preserving label settings. |
| TEXT | Parse coordinates, quoted font, rotation, horizontal/vertical integer scaling, and quoted content. Support rotations 0/90/180/270 and scales 1–10, subject to bounds. Reject additional alignment arguments in this initial subset. |
| PRINT | Support one label set with an optional positive copy count, capped at 255. Reject multiple sets. Validate before each copy; never mutate the drawing set to fit media. |

Use integer conversion with checked arithmetic and deterministic rounding to the nearest device dot. Reject negative dimensions, invalid suffixes, exponents, trailing garbage, and arithmetic overflow. Coordinates and transformation intermediates use sufficiently wide signed arithmetic before bounds checks.

The initial reusable font is the existing 12×24 bitmap exposed by the resident name `PSF12X24.FNT`, for printable ASCII content. Do not advertise the existing 9×17 or 12×24 glyphs as TSC numbered fonts with different metrics. Unavailable numbered fonts, downloaded fonts, Unicode, and font-loading commands are explicitly unsupported in this first implementation. This is a compatibility limitation, not silent font substitution.

## Rendering and print-engine integration

Render only a validated immutable label. Reuse the existing glyph source and transformations. Add only the coordinate placement/composition needed to express TSPL TEXT operations as existing print-list raster records. There is no new hardware raster engine, label queue, sensor, or full-label canvas.

Compose bounded bands no taller than the existing 24-row raster path. A full-width band needs 1152 bytes (384 / 8 × 24); prefer writing directly into an available list buffer where ownership permits. Overlapping text combines black pixels. Blank areas and the requested label height are retained; content is not cropped or scaled to match measured media.

The adapter participates in the existing A/B buffer handshake, existing IRQ start/completion flow, label feed/stop flags, thermal boundaries, and low-battery label accounting. Shared submission helpers may be extracted only where required to expose these existing operations without invoking ESC/POS handlers. Preserve the legacy statements in the disabled branch.

Preflight the whole label before handing the first band to the engine. Submission failure before handoff produces no print. After a hardware failure during printing, cancel remaining bands and copies and clear executable retained state. Already printed physical dots cannot be rolled back. Buffer ownership remains with the engine until its existing completion signal releases it.

## Media and readiness validation

Use fresh existing paper/lid state and calibration validity, raw `label_length_mm`, and nominal `g_label_size_final`. Treat missing media, invalid calibration, unknown nominal height, open lid, thermal stop, and shutdown as distinct readiness/media outcomes. A stale height from before a lid/media change is not sufficient to print.

The sensor path inspected measures height and media presence; it does not establish a measured media width. Use the 384-dot head and existing configured media width/margins (`g_PaperWide`, `g_HorShift`, and print offsets), and document that width is configured rather than independently sensed. Do not describe SIZE as a measurement.

Check every transformed TEXT rectangle against the design and effective physical horizontal bounds. Enforce calibrated nominal media support and existing tolerance mapping. Compare requested design height with detected physical height for overflow. A user override waives only that height mismatch, never hardware readiness, design bounds, or the head width.

## TSPL asynchronous Size Overflow path

Implement foreground states for editing, waiting for confirmation, submitting, and waiting for completion. Overflow state and deadline belong to the retained job, not the transport callback.

On height overflow, retain the exact job and report both heights using the existing mobile message. Display these two OLED lines exactly:

```text
Size Overflow
Print? Feed = Y
```

Use an elapsed 5000 ms deadline, matching the existing workflow. Capture a fresh Feed press after entering the prompt; a button already held must not approve the job. Claim the prompt's Feed event so it cannot also trigger ordinary feed or self-test. Track release without a busy loop. Prompt ownership suppresses competing home/menu refreshes while required status work continues.

Service the state during idle receive waits and active foreground passes. Do not call Delay1ms or wait for new bytes/key release to advance the prompt. Continue watchdog, transport/backpressure, lid/media updates, battery, and temperature services. Use a monotonic tick that is not reset by unrelated command handlers.

On Feed, revalidate current media, calibration, all bounds, and readiness before submitting. If validation fails, report the specific outcome and discard executable retained state. At the timeout boundary, timeout wins and discards the job. Restore the normal display when prompt ownership ends. Later PRINT cannot revive the discarded job; a fresh drawing sequence is required.

On media/lid changes or unrecoverable receive errors during confirmation, cancel the retained job. Never silently truncate or resize a confirmed oversized label. Remaining copies revalidate readiness and media before starting.

## Errors and reporting

Provide separate parser result categories for malformed input, unsupported command/argument, capacity, invalid job/bounds, media/calibration failure, hardware readiness/failure, and submission failure. Expose the most recent status for tests and firmware diagnostics without adding unconditional debug logging.

Reuse existing NACK categories where they faithfully apply. For distinctions absent from the old protocol, add a bounded TSPL-specific status message through the existing transport response facility, document its wire format, and test it. Do not rename existing mobile height messages or reverse their field order.

## Validation and completion criteria

Add lightweight host C tests with mocked media, time, input, and submission dependencies. Cover every command, supported unit conversions, quotes and embedded commas, all split positions of representative streams, multiple commands, empty input, malformed fields, capacities, unsupported forms, invalidation/CLS recovery, copies, and submission failure.

Exercise the actual overflow state implementation with mocked firmware facilities: exact prompt text, retained dimensions, fresh Feed event, no duplicate feed action, no-input timeout, tick wraparound, held button, boundary timing, media change, stale calibration, readiness failure, telemetry order, discard, and stale-job prevention. Compare rendered bands and bounds for rotation, direction, scaling, reference, and overlapping text.

Build TSPL enabled and disabled with Keil. Inspect linked symbols and RAM/flash use. Compile and run host tests with strict warnings and available runtime diagnostics. Test-only mocks do not count as hardware validation. Build failures, unavailable licenses, unsupported compiler diagnostics, and board-only verification remain explicitly reported.

The firmware handoff includes command examples using the supported font, supported/deferred forms, memory limits, modified-file summary, executed build/test results, and hardware checks still required. Physical print quality, Feed behavior, sensor transitions, motor alignment, and mobile application interoperability require the real board/application.

## Deferred scope

CPCL, runtime protocol switching, command auto-detection, barcode/QR/bitmap/graphics input commands, downloadable fonts, numbered TSC font emulation, continuous media, mirroring, TEXT alignment extensions, multiple PRINT sets, and unrelated configuration commands are excluded. Internal raster records used for TEXT do not enable a TSPL BITMAP command.
