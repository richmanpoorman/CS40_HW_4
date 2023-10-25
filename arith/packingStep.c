/* 
 *   Name       : packingStep.c
 *   Assignment : CS40 Homework 4 (arith)
 *   Purpose    : Module for packing and unpacking codewords
 *   Editors    : Matthew Wong (mwong14), Ivi Fung (sfung02)
 */

#include <pnm.h>
#include <a2plain.h>
#include "CompressionStep.h"
#include <a2methods.h>  
#include "PixelStructs.h"
#include <arith40.h>
#include "codewordInfo.h"
#include <assert.h>
#include "bitpack.h"

static void compress(Pnm_ppm image);
static void decompress(Pnm_ppm image);

static void pack(int col, int row, A2Methods_UArray2 uarray2, 
                 A2Methods_Object *ptr, void *cl);
static void unpack(int col, int row, A2Methods_UArray2 uarray2, 
                   A2Methods_Object *ptr, void *cl);


/*
 *  Name      : compress
 *  Purpose   : Pack the Dct_int values into Codeword pixels
 *  Parameters: (Pnm_ppm) image = Image with Dct_ints as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Dct_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              Codeword image (if necessary)
 *              Will CRE if given a null image, or the values are null
 */
static void compress(Pnm_ppm image)
{
        assert(image != NULL);

        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels   = image -> pixels;
        assert(pixels != NULL);
        int               size     = sizeof(struct Dct_int);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, pack, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : pack
 *  Purpose   : Packs the DCT int pixel into 32 bits
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The Codeword value in the 
 *                                             new array
 *              (void *)             cl      = The DCT int image
 *  Output    : (None)
 *  Notes     : Converts the DCT int to Codeword
 */
static void pack(int col, int row, A2Methods_UArray2 uarray2, 
                     A2Methods_Object *ptr, void *cl)
{
        
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;

        assert(uarray2 != NULL);
        assert(pixels != NULL);

        Dct_int  data       = methods -> at(pixels, col, row);
        Codeword inNewImage = ptr;

        assert(data != NULL);
        assert(inNewImage != NULL);

        int      a          = data -> a;
        int      b          = data -> b;
        int      c          = data -> c;
        int      d          = data -> d;
        int      pb         = data -> pb;
        int      pr         = data -> pr;

        int aLength  = getALength();
        int bLength  = getBLength();
        int cLength  = getCLength();
        int dLength  = getDLength();
        int pbLength = getPbLength();
        int prLength = getPrLength();

        int aLsb  = getALsb();
        int bLsb  = getBLsb();
        int cLsb  = getCLsb();
        int dLsb  = getDLsb();
        int pbLsb = getPbLsb();
        int prLsb = getPrLsb();

        uint64_t word = 0;
        word = Bitpack_newu(word, aLength, aLsb, a);
        
        word = Bitpack_news(word, bLength, bLsb, b);

        word = Bitpack_news(word, cLength, cLsb, c);

        word = Bitpack_news(word, dLength, dLsb, d);

        word = Bitpack_newu(word, pbLength, pbLsb, pb);

        word = Bitpack_newu(word, prLength, prLsb, pr);

        struct Codeword newPixel = {
                word
        };
        *inNewImage = newPixel;
}

/*
 *  Name      : decompress
 *  Purpose   : Unpack the Codeword value into Dct_int pixels
 *  Parameters: (Pnm_ppm) image = Image with Codewords as pixels
 *  Output    : (None)
 *  Notes     : Assumes that the Pnm_ppm is in proper format, with the
 *              values being Dct_float structs (no way to check :( );
 *              Will CRE if can not allocate new memory to create the new
 *              Dct_int image (if necessary)
 *              Will CRE if given a null image, or the values are null
 */
static void decompress(Pnm_ppm image)
{
        assert(image != NULL);
        int width  = image -> width;
        int height = image -> height;

        A2Methods_T methods = uarray2_methods_plain;

        A2Methods_UArray2 pixels   = image -> pixels;
        assert(pixels != NULL);
        int               size     = sizeof(struct Dct_float);
        A2Methods_UArray2 newImage = methods -> new(width, height, size);
        assert(newImage != NULL);

        methods -> map_row_major(newImage, unpack, pixels);
        methods -> free(&pixels);
        
        image -> pixels = newImage;
}

/*
 *  Name      : unpack
 *  Purpose   : Unpacks the Codeword pixel into DCT ints
 *  Parameters: (int)                col     = The current column to copy
 *              (int)                row     = The current row to copy
 *              (A2Methods_UArray2)  uarray2 = The new array to copy into
 *              (A2Methods_Object *) ptr     = The DCT int value in the 
 *                                             new array
 *              (void *)             cl      = The Codeword image
 *  Output    : (None)
 *  Notes     : Converts the Codeword int to DCT int
 */
static void unpack(int col, int row, A2Methods_UArray2 uarray2, 
                       A2Methods_Object *ptr, void *cl)
{
        A2Methods_T       methods = uarray2_methods_plain;
        A2Methods_UArray2 pixels  = cl;

        assert(uarray2 != NULL);
        assert(pixels != NULL);

        Codeword data       = methods -> at(pixels, col, row);
        Dct_int  inNewImage = ptr;
        
        assert(data != NULL);
        assert(inNewImage != NULL);

        uint64_t word = data -> codeword;

        int aLength  = getALength();
        int bLength  = getBLength();
        int cLength  = getCLength();
        int dLength  = getDLength();
        int pbLength = getPbLength();
        int prLength = getPrLength();

        int aLsb  = getALsb();
        int bLsb  = getBLsb();
        int cLsb  = getCLsb();
        int dLsb  = getDLsb();
        int pbLsb = getPbLsb();
        int prLsb = getPrLsb();

        int a  = Bitpack_getu(word, aLength , aLsb );
        int b  = Bitpack_gets(word, bLength , bLsb );
        int c  = Bitpack_gets(word, cLength , cLsb );
        int d  = Bitpack_gets(word, dLength , dLsb );
        int pb = Bitpack_getu(word, pbLength, pbLsb);
        int pr = Bitpack_getu(word, prLength, prLsb);

        struct Dct_int newPixel = {
                a, 
                b, 
                c, 
                d, 
                pb, 
                pr
        };
        *inNewImage = newPixel;

}


static struct CompressionStep packingStepStruct = {
        compress,
        decompress
};

CompressionStep packingStep = &packingStepStruct;
