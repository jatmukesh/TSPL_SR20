#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplRaster.h"
static unsigned checks;
#define CHECK(x) do { ++checks; if (!(x)) { fprintf(stderr,"raster:%d: %s\n",__LINE__,#x); exit(1); } } while(0)
static void Glyph(uint8_t character, uint32_t columns[32])
{
    CHECK(character=='A'); memset(columns,0,32U*sizeof columns[0]);
    columns[0]=0x80000000U; columns[11]=0x100U; /* (0,0), (11,23) */
}
int main(void)
{
    TsplLabel_t label;
    struct { uint16_t before, pixels[576], after; } buffer;
    TsplText_t *op;
    memset(&label,0,sizeof label); label.width=384; label.height=240;
    label.direction=1; label.operationCount=1; label.textUsed=2; strcpy(label.text,"A");
    op=&label.operations[0]; op->length=1; op->scaleX=op->scaleY=1;
    buffer.before=0x1234; buffer.after=0xabcd;
    CHECK(TsplRaster_Band(&label,0,24,buffer.pixels,Glyph)==TSPL_OK);
    CHECK(buffer.pixels[0]==0x8000 && buffer.pixels[23*24]==0x10);
    CHECK(buffer.before==0x1234 && buffer.after==0xabcd);
    op->x=30; op->y=10; op->rotation=90;
    CHECK(TsplRaster_Band(&label,0,24,buffer.pixels,Glyph)==TSPL_OK);
    CHECK((buffer.pixels[10*24+1] & 2U)!=0); /* x=30 */
    CHECK((buffer.pixels[21*24] & 0x100U)!=0); /* x=7 */
    label.direction=0; op->rotation=0; op->x=op->y=0;
    CHECK(TsplRaster_Band(&label,216,24,buffer.pixels,Glyph)==TSPL_OK);
    CHECK((buffer.pixels[23*24+23] & 1U)!=0); /* (383,239) */
    CHECK((buffer.pixels[23] & 0x800U)!=0); /* (372,216) */
    CHECK(TsplRaster_Band(&label,240,24,buffer.pixels,Glyph)==TSPL_RANGE);
    printf("Raster: %u checks passed\n",checks);
    return 0;
}
