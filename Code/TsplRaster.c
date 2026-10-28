/* File Name: TsplRaster.c; Author: Not recorded for this addition
 * Version: PSF20 R002 V4.1.0; Date: 2026-10-19
 * Description: Bounded coordinate composition using existing glyph readers. */
#include "TsplRaster.h"
#ifdef PSF20_R002_TSPL_PROTOCOL
#include <string.h>

/* Function name: TsplRaster_Band; Description: Compose a validated text band.
 * Parameters: immutable label, first row, row count, engine-owned destination,
 *             native glyph reader; Returns: status; Global vars: None.
 * Remarks: Destination capacity is rows * 24 words; no full-label bitmap.
 * Glyph data is column-major, with the top pixel in bit 31. */
TsplResult_t TsplRaster_Band(const TsplLabel_t *label, uint16_t startRow,
                           uint16_t rows, uint16_t *pixels, TsplGlyphReader_t readGlyph)
{
    uint32_t columns[32];
    uint16_t character, u, v, dx, dy;
    uint8_t index;
    const TsplText_t *operation;
    TsplPoint_t point;
    if (!rows || (rows > TSPL_BAND_ROWS) || (startRow >= label->height) ||
        ((uint32_t)startRow + rows > label->height)) return TSPL_RANGE;
    memset(pixels, 0, (size_t)rows * 24U * sizeof(*pixels));
    for (index = 0U; index < label->operationCount; ++index) {
        operation = &label->operations[index];
        for (character = 0U; character < operation->length; ++character) {
            readGlyph((uint8_t)label->text[operation->textOffset + character], columns);
            for (u = 0U; u < 12U; ++u) {
                for (v = 0U; v < 24U; ++v) {
                    if ((columns[u] & (0x80000000UL >> v)) == 0U) continue;
                    for (dx = 0U; dx < operation->scaleX; ++dx) {
                        for (dy = 0U; dy < operation->scaleY; ++dy) {
                            point.x = (int32_t)((character * 12U + u) * operation->scaleX + dx);
                            point.y = (int32_t)(v * operation->scaleY + dy);
                            TsplJob_Transform(label, operation, &point);
                            if ((point.x < 0) || (point.x >= (int32_t)TSPL_HEAD_DOTS) ||
                                (point.y < startRow) || (point.y >= (int32_t)startRow + rows)) continue;
                            pixels[(uint16_t)(point.y - startRow) * 24U + (uint16_t)point.x / 16U] |=
                                (uint16_t)(0x8000U >> ((uint16_t)point.x & 15U));
                        }
                    }
                }
            }
        }
    }
    return TSPL_OK;
}
#endif
