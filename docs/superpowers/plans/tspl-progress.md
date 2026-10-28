# SDD ledger — plan: docs/superpowers/plans/2026-10-19-tspl-implementation.md

Execution: native, approved by the user's final "okay" after the protocol-switch addition.
Pre-flight: Task 2 label API feeds Task 3 controller and Task 4 compositor; compatible.
Pre-flight: Task 3 input/ports API feeds Task 5 firmware service; compatible.
Pre-flight: Task 4 native print records feed Task 5 existing buffer publication; compatible.
Pre-flight: Task 6 excludes input parsers but must preserve Task 5 local print facilities.
Ruling: Work in the approved SVN checkout and keep evidence/ledger files instead of Git-only skill scripts and commits. No Git repository is available; no SVN commits are authorized. Cost if wrong: recovery depends on saved patches rather than task commits.
Tasks: 1 baseline/harness; 2 parser; 3 controller; 4 bands; 5 firmware integration; 6 builds; 7 review/documentation.
Task 1: baseline build passed (0 errors, 0 warnings; Code=125156 RO=46632 RW=2788 ZI=59644). Parser first run failed for absent sources. Task 2: parser tests exposed a test setup error: DIRECTION and REFERENCE remained invalid, correctly surviving CLS. Recovery test now explicitly repairs both settings.
Task 2: complete, parser and capacity tests pass under GCC strict warnings. Task 3: complete, actual job controller tested through mocked time/media/engine ports. Task 4: complete, native glyph band layout and transformed pixels tested. Task 5: first integration complete; real native adapter and shared ring tested with actual firmware header layouts and mocked PDL operations. Task 6: TSPL and ESC/POS Keil builds both pass, zero warnings/errors. ESC/POS size exactly matches baseline. Unselected parser symbols absent in linked symbol tables.
Ruling: Introduced TsplRaster and TsplReceive as focused modules beyond the two initially proposed modules; native composition and ring safety need direct host tests without MCU services. Cost if wrong: extra module boundaries to maintain.
Ruling: GAP offset values other than zero and GAP distances different from calibrated m_BarPaGap are rejected explicitly. The existing native feed controller has no per-job offset port; inventing one would change hardware timing without evidence. Cost if wrong: mobile jobs using other GAP values are rejected rather than printed.
Ruling: TSPL self-test uses a native three-line capability label rather than advertising ESC/POS barcode support. Legacy ESC/POS self-test remains byte-for-byte in its guarded branch. Cost if wrong: TSPL diagnostic content differs from the legacy receipt.
Verification: GCC UndefinedBehaviorSanitizer could not link because libubsan is absent; no sanitizer success is claimed. Canary-framed 100000-byte parser fuzz and strict host suites pass.
