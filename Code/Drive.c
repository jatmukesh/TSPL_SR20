#include "Drive.h"
#ifdef RD_MICROPRINTER
#include "RD_MP_Drive.c"
#endif

#ifdef RD_W2C1PRINTER
#include "RD_W2C1_Drive.c"
#endif


#ifdef RD_OEMPRINTER
#include "RD-OEM_Drive.c"
#endif

#ifdef RD_EXPRINTER
#include "RD_EX_Drive.c"
#endif

#ifdef RD_MP3PRINTER
#include "RD_MP3_Drive.c"
#endif
#ifdef RD_BMPRINTER  
#include "RD_BM_Drive.c"
#endif
#ifdef RD_TSPRINTER
#include "RD_TS_Drive.c"
#endif
#ifdef RD_A100PRINTER
#include "RD_A100_Drive.c"
#endif
#ifdef RD_A200PRINTER
#include "RD_A200_Drive.c"
#endif
