// Author: Tawezy7
// Date: 2024-09-06
#ifndef _INC_WORD_H_
#define _INC_WORD_H_

#ifdef __cplusplus
extern "C"
{
#endif
        extern unsigned char word[];
        extern unsigned char word1[];
        void lcdShowOneWord16x16(int x, int y, unsigned char *word);
        void lcdShowOneWord64x64(int x, int y, unsigned char *word);
        void lcdShowOneWordASCII8x16(int x, int y, unsigned char ASCII);
        void lcdShowString8x16(int x, int y, char *str);
        void bgColorSet(unsigned int color);
        void frontColorSet(unsigned int color);

    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_WORD_H_ */