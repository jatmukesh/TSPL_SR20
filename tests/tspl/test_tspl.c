#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TsplParser.h"

static unsigned checks;
#define CHECK(condition) do { ++checks; if (!(condition)) { \
    fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #condition); exit(1); } } while (0)

static TsplResult_t Feed(TsplParser_t *parser, const char *text)
{
    TsplResult_t result = TSPL_OK;
    while (*text != '\0') result = TsplParser_PushByte(parser, (uint8_t)*text++);
    return result;
}

static void ParserTests(void)
{
    TsplParser_t parser;
    const TsplLabel_t *label;
    char oversized[600];
    unsigned i;
    TsplParser_Init(&parser);
    CHECK(Feed(&parser, "SIZE 48 mm,30 mm\r\nGAP 3 mm,0 mm\nDIRECTION 1\nREFERENCE 0,0\nCLS\nTEXT 8,8,\"PSF12X24.FNT\",0,1,1,\"Hello, SR20\"\nPRINT 1,2\n") == TSPL_PRINT);
    label = TsplParser_GetLabel(&parser);
    CHECK(label->width == 384 && label->height == 240);
    CHECK(label->gap == 24 && label->gapOffset == 0);
    CHECK(label->direction == 1 && label->copies == 2 && label->operationCount == 1);
    CHECK(strcmp(label->text + label->operations[0].textOffset, "Hello, SR20") == 0);
    CHECK(Feed(&parser, "TEXT 0,0,\"1\",0,1,1,\"x\"\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "PRINT 1\n") == TSPL_INVALID_JOB);
    CHECK(Feed(&parser, "CLS\nPRINT 1\n") == TSPL_PRINT);
    CHECK(label->operationCount == 0 && label->width == 384);
    CHECK(Feed(&parser, "SIZE 385 dot,96 dot\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "CLS\nPRINT 1\n") == TSPL_INVALID_JOB);
    CHECK(Feed(&parser, "SIZE 384 dot,96 dot\nCLS\nPRINT 1\n") == TSPL_PRINT);
    CHECK(Feed(&parser, "SIZE 1,0.5\n") == TSPL_LINE);
    CHECK(label->width == 203 && label->height == 102);
    CHECK(Feed(&parser, "SIZE 48 mm,11 mm\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "SIZE 48 mm,76 mm\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "SIZE 48 xx,30 mm\n") == TSPL_SYNTAX);
    CHECK(Feed(&parser, "SIZE -1 mm,30 mm\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "GAP 0,0\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "DIRECTION 1,1\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "REFERENCE 1\n") == TSPL_SYNTAX);
    CHECK(Feed(&parser, "BAR 1,1,1,1\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "PRINT 2,1\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "PRINT 1,0\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "PRINT 1,256\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "TEXT 1,2,\"PSF12X24.FNT\",45,1,1,\"x\"\n") == TSPL_UNSUPPORTED);
    CHECK(Feed(&parser, "TEXT 1,2,\"PSF12X24.FNT\",0,0,1,\"x\"\n") == TSPL_RANGE);
    CHECK(Feed(&parser, "TEXT 1,2,\"PSF12X24.FNT\",0,1,1,\"unterminated\n") == TSPL_SYNTAX);
    memset(oversized, 'A', sizeof oversized - 2);
    oversized[sizeof oversized - 2] = '\n'; oversized[sizeof oversized - 1] = '\0';
    CHECK(Feed(&parser, oversized) == TSPL_CAPACITY);
    CHECK(Feed(&parser, "SIZE 48 mm,30 mm\nGAP 3 mm,0 mm\nCLS\nPRINT 1\n") == TSPL_INVALID_JOB);
    CHECK(Feed(&parser, "DIRECTION 1\nREFERENCE 0,0\nCLS\nPRINT 1\n") == TSPL_PRINT);
    for (i = 0; i < 24; ++i)
        CHECK(Feed(&parser, "TEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"x\"\n") == TSPL_LINE);
    CHECK(Feed(&parser, "TEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"x\"\n") == TSPL_CAPACITY);
    TsplParser_Cancel(&parser);
    CHECK(Feed(&parser, "PRINT 1\n") == TSPL_INVALID_JOB);
    TsplParser_Init(&parser);
    CHECK(Feed(&parser, "SIZE 48 mm,30 mm\nGAP 3 mm,0 mm\nCLS\nPRINT 1") == TSPL_OK);
    CHECK(TsplParser_PushByte(&parser, '\n') == TSPL_PRINT);
    CHECK(TsplParser_PushByte(&parser, 0) == TSPL_SYNTAX);
    CHECK(TsplParser_PushByte(&parser, '\n') == TSPL_SYNTAX);
    TsplParser_Init(&parser);
    CHECK(Feed(&parser,"SIZE 48mm,30 mm\n")==TSPL_SYNTAX);
    CHECK(Feed(&parser,"SIZE 1 inch,0.5 inch\n")==TSPL_SYNTAX);
    CHECK(Feed(&parser,"TEXT 0,0,\"PSF12X24.FNT\",0,1,1,\"A\\[\"]B\"\n")==TSPL_LINE);
    CHECK(strcmp(parser.label.text,"A\"B")==0);
}

int main(void)
{
    ParserTests();
    printf("TSPL: %u checks passed\n", checks);
    return 0;
}
