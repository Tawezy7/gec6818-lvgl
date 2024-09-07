// Author: Tawezy7
// Date: 2024-09-06
#ifndef _INC_WORD_H_
#define _INC_WORD_H_

#ifdef __cplusplus

extern "C"
{
#endif
        extern unsigned char word[];
        void lcdShowOneWord16x16(int x, int y, unsigned char *word);
    /*...*/

#ifdef __cplusplus
}
#endif

#endif /* _INC_WORD_H_ */