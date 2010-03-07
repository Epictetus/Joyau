#include <oslib/oslib.h>

#define MAX(a, b) \
   (((a) > (b)) ? (a) : (b))

inline void oslDrawGradientLine(int x0, int y0, int x1, int y1,
			 OSL_COLOR color, OSL_COLOR color2) {
   OSL_LINE_VERTEX* vertices;

   vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(2 * sizeof(OSL_LINE_VERTEX));
   color = oslBlendColor(color);
   color2 = oslBlendColor(color2);
   
   vertices[0].color = color;
   vertices[0].x = x0;
   vertices[0].y = y0;
   vertices[0].z = 0;
   
   vertices[1].color = color2;
   vertices[1].x = x1;
   vertices[1].y = y1;
   vertices[1].z = 0;
   
   // oslDisableTexturing();
   sceGuDrawArray(GU_LINES,
		  GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 
		  2, 0, vertices);
}

#define oslScriptText oslDrawString

inline void oslPrintStirringString(int x, int y, const char *txt) {
   oslScriptText(x, y, txt);
}

inline void oslDrawFillCircle(int centerX, int centerY,
                              int radius, OSL_COLOR color) {
   int x;
   int y;

   for(x = -1 * radius; x <= radius; x ++ ) {
      y = sqrt((radius * radius) - (x * x));
      oslDrawLine(x + centerX, y + centerY, centerX , y + centerY, color);
      y = sqrt( ( radius * radius ) - (x * x) ) * -1 ;
      oslDrawLine ( x + centerX , y + centerY , centerX , y + centerY, color) ;
   }
   for(y = -1 * radius ; y <= radius ; y ++) {
      x = sqrt( ( radius * radius ) - ( y * y ) ) ;
      oslDrawLine ( x + centerX , y + centerY , centerX , y + centerY, color) ;
      x = sqrt( ( radius * radius ) - ( y * y ) ) * -1 ;
      oslDrawLine ( x + centerX , y + centerY , centerX , y + centerY, color);
   }
}

inline void oslDrawCircle(int centerX, int centerY, int radius,
                   OSL_COLOR color) {
   int x;
   int y;
   radius ++;
   for( x = -1 * radius ; x <= radius ; x ++ ) {
      y = sqrt ( ( radius * radius ) - ( x * x ) ) ;
      oslDrawLine( x + centerX , y + centerY , 
		   x + centerX +1, y + centerY +1, color) ;
      y = sqrt ( ( radius * radius ) - (x * x) ) * -1 ;
      oslDrawLine ( x + centerX , y + centerY , 
		    x + centerX +1, y + centerY +1, color) ;
   }
   
   for(y = -1 * radius ; y <= radius ; y ++) {
      x = sqrt ( ( radius * radius ) - ( y * y ) ) ;
      oslDrawLine ( x + centerX , y + centerY ,
		    x + centerX +1, y + centerY +1, color) ;
      x = sqrt ( ( radius * radius ) - ( y * y ) ) * -1 ;
      oslDrawLine ( x + centerX , y + centerY , 
		    x + centerX +1, y + centerY +1, color) ;

   }
}

inline void oslDrawGradientTriangle(int x0, int y0, 
			     int x1, int y1 , 
			     int x2, int y2, 
			     OSL_COLOR c1, OSL_COLOR c2, OSL_COLOR c3) {
   OSL_LINE_VERTEX* vertices;
   
   vertices = (OSL_LINE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_LINE_VERTEX));
   
   c1 = oslBlendColor(c1);
   c2 = oslBlendColor(c2);
   c3 = oslBlendColor(c3);

   vertices[0].color = c1;
   vertices[0].x = x0;
   vertices[0].y = y0;
   vertices[0].z = 0;
   
   vertices[1].color = c2;
   vertices[1].x = x1;
   vertices[1].y = y0;
   vertices[1].z = 0;

   vertices[2].color = c3;
   vertices[2].x = x0;
   vertices[2].y = y1;
   vertices[2].z = 0;
   
   vertices[3].color = c1;
   vertices[3].x = x0;
   vertices[3].y = y0;
   vertices[3].z = 0;
   
   // oslDisableTexturing();
   sceGuDrawArray(GU_TRIANGLE_STRIP, 
		  GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 
		  4, 0, vertices);
}

inline void oslFadeInEffect() {}
