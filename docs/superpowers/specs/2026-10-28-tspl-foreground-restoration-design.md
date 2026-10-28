# TSPL Foreground Restoration Design

## Intent

Keep TSPL as the only enabled print-command parser while restoring the proven legacy foreground behavior for the SR20 user interface, Bluetooth status handling, media monitoring, safety, and hardware services.

## Goals

- Keep `TSPL_PROTOCOL=1` and `ESC_POS_PROTOCOL=0`.
- Preserve the normal foreground loop and its home-screen/menu lifecycle.
- Let TSPL consume print data incrementally at the legacy parser boundary.
- Keep Bluetooth transport and status servicing independent of the selected print language.
- Do not feed a second blank label after a completed TSPL print.

## Non-goals

- Re-enable ESC/POS print parsing.
- Change TSPL command coverage or raster composition.
- Rewrite the native motor, sensor, OLED, or Bluetooth drivers.

## Approaches

1. Continue extending the TSPL replacement loop. Rejected because it already bypassed normal UI and transport behavior.
2. Restore the legacy foreground loop and replace only its incoming print-data parser. Selected because it retains known-good platform behavior while keeping TSPL exclusive.
3. Run both protocol parsers and auto-detect. Rejected because the requested build is TSPL-only.

## Design

The selected TSPL branch will retain the established foreground lifecycle. Its receive-data work unit will call `TsplFirmware_Service()` rather than `PrinterCore()` / ESC parsing. The existing menu timeout will trigger `OLED_RestoreHomeScreen()` on the transition from menu to home. Bluetooth display/status service will run even when media is unavailable. TSPL completion will not call the legacy `RunFeedLabPaper()` alignment routine; it will report completion after the final native raster band, leaving any explicit feed action to the user or a future TSPL feed command.

## Constraints

- Retain compile-time exclusive protocol macros.
- Preserve existing code under the ESC/POS branch.
- Add a regression test before each changed TSPL behavior.
- Build TSPL and ESC/POS configurations.
