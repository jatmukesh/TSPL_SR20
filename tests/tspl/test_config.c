#include "macros.h"
#if TSPL_PROTOCOL
#ifndef PSF20_R002_TSPL_PROTOCOL
#error Missing TSPL implementation guard
#endif
#ifdef PSF20_R002_ESC_POS_PROTOCOL
#error ESC/POS cannot be present in a TSPL build
#endif
#else
#ifndef PSF20_R002_ESC_POS_PROTOCOL
#error Missing ESC/POS implementation guard
#endif
#ifdef PSF20_R002_TSPL_PROTOCOL
#error TSPL cannot be present in an ESC/POS build
#endif
#endif
int main(void) { return 0; }
