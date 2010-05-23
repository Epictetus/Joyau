#include <oslib/oslib.h>

#define MAX(a, b) \
   (((a) > (b)) ? (a) : (b))

void oslDrawGradientLine(int x0, int y0, int x1, int y1,
			 OSL_COLOR color, OSL_COLOR color2);

#define oslScriptText oslDrawString

void oslPrintStirringString(int x, int y, const char *txt);

void oslDrawFillCircle(int centerX, int centerY,
                              int radius, OSL_COLOR color);

void oslDrawCircle(int centerX, int centerY, int radius,
                   OSL_COLOR color);

void oslDrawGradientTriangle(int x0, int y0, 
			     int x1, int y1 , 
			     int x2, int y2, 
			     OSL_COLOR c1, OSL_COLOR c2, OSL_COLOR c3);

void oslFadeInEffect();
