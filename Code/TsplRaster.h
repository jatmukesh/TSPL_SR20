/* File Name: TsplRaster.h; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Text placement into the existing 24-row raster record. */
#ifndef TSPL_RASTER_H
#define TSPL_RASTER_H
#include "TsplJob.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
typedef void (*TsplGlyphReader_t)(uint8_t character, uint32_t columns[32]);
TsplResult_t TsplRaster_Band(const TsplLabel_t *label, uint16_t startRow,
                           uint16_t rows, uint16_t *pixels, TsplGlyphReader_t readGlyph);
#endif
#endif
