#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <assert.h>
#include <pnm.h>
#include <a2methods.h>
#include <a2plain.h>

float getPixelDiff(A2Methods_UArray2 img1, A2Methods_UArray2 img2, 
                          int c, int r, int img1Denom, int img2Denom) 
{
        A2Methods_T methods = uarray2_methods_plain;


        Pnm_rgb img1Colors = methods -> at(img1, c, r);
        Pnm_rgb img2Colors = methods -> at(img2, c, r);
        
        float rDiff = 1.0 * img1Colors -> red   / img1Denom - 
                      1.0 * img2Colors -> red   / img2Denom;
        float gDiff = 1.0 * img1Colors -> green / img1Denom - 
                      1.0 * img2Colors -> green / img2Denom;
        float bDiff = 1.0 * img1Colors -> blue  / img1Denom - 
                      1.0 * img2Colors -> blue  / img2Denom;

        return rDiff * rDiff + gDiff * gDiff + bDiff * bDiff;
        
}

float Ppm_ppmdiff(FILE *image1File, FILE *image2File) 
{
        if (image1File == NULL || image2File == NULL) {
                fprintf(stderr, "An image was NULL\n");
                fprintf(stdout, "1.0");
                return 1.0;
        }

        A2Methods_T methods = uarray2_methods_plain;
        
        Pnm_ppm image1 = Pnm_ppmread(image1File, methods);
        Pnm_ppm image2 = Pnm_ppmread(image2File, methods);

        A2Methods_UArray2 img1Pixels = image1 -> pixels;
        A2Methods_UArray2 img2Pixels = image2 -> pixels;

        int widthDiff = methods -> width(img1Pixels) - 
                        methods -> width(img2Pixels);
        if (abs(widthDiff) > 1) {
                fprintf(stderr, "Widths were more than 1 apart\n");
                fprintf(stdout, "1.0\n");
                return 1.0;
        }

        int heightDiff = methods -> height(img1Pixels) - 
                         methods -> height(img2Pixels);
        if (abs(heightDiff) > 1) {
                fprintf(stderr, "Heights were more than 1 apart\n");
                fprintf(stdout, "1.0\n");
        }

        int minWidth  = fmin(methods -> width(img1Pixels), 
                             methods -> width(img2Pixels));
        int minHeight = fmin(methods -> height(img1Pixels),
                             methods -> height(img2Pixels));

        float img1Denominator = image1 -> denominator;
        float img2Denominator = image2 -> denominator;
        float diffSqr = 0;
        for (int c = 0; c < minWidth; c++) {
                for (int r = 0; r < minHeight; r++) {
                        diffSqr += 
                                getPixelDiff(img1Pixels, img2Pixels, c, r, 
                                        img1Denominator, img2Denominator);
                }
        }
        

        Pnm_ppmfree(&image1);
        Pnm_ppmfree(&image2);

        float ans = sqrt(diffSqr / (3.0 * minWidth * minHeight));
        fprintf(stdout, "%0.4f\n", ans);
        return ans;
}

int main(int argc, char *argv[]) 
{
        if (argc != 3) {
                fprintf(stderr, "Not correct file count\n");
                exit(EXIT_FAILURE);
        }

        FILE *image1 = stdin;
        if (strcmp(argv[1], "-") != 0) {
                image1 = fopen(argv[1], "r");
                assert(image1 != NULL);
        }
        
        FILE *image2 = stdin;
        if (strcmp(argv[2], "-") != 0) {
                image2 = fopen(argv[2], "r");
                assert(image2 != NULL);
        }

        float val = Ppm_ppmdiff(image1, image2);
        (void) val;
        if (image1 != stdin) {
                fclose(image1);
        }
        if (image2 != stdin) {
                fclose(image2);
        }
}