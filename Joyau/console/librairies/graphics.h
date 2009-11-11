#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <psptypes.h>

#define	PSP_LINE_SIZE 512
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

typedef u32 Color;

#define RGB(r,g,b) ((r)|((g)<<8)|((b)<<16))
#define RGBA(r,g,b,a) ((r)|((g)<<8)|((b)<<16)|((a)<<24))
#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

enum {
   LITERED  = 	0xFF5858FF,
   TRED     = 	0x8F0000FF,
   RED 	    = 	0xFF0000FF,
   GREEN    =	0xFF00FF00,
   BLUE     =	0xFFFF0000,
   LITEBLUE =  	0xB4FF9628,
   WHITE    =	0xFFFFFFFF,
   LITEGRAY = 	0xFFBFBFBF,
   GRAY     =  	0xFF7F7F7F,
   DARKGRAY = 	0xFF3F3F3F,	
   BLACK    = 	0xFF000000,
   TBLACK   =	0x8F000000
};

typedef struct
	{
	unsigned short u, v;
	short x, y, z;
	} tVertex;

typedef struct
	{
	unsigned int color;
	short x,y,z;
	} cVertex;

typedef struct
	{
	short x,y,z;
	} Vertex;

typedef struct
{
	int textureWidth;  // the real width of data, 2^n with n>=0
	int textureHeight;  // the real height of data, 2^n with n>=0
	int imageWidth;  // the image width
	int imageHeight;
	char swizzled; // Boolean
	Color* data;
} Image;

int getNextPower2(int width);

extern void initGraphics();
extern void disableGraphics();
extern void guStart();
extern void clearScreen(Color color);
extern void flipScreen();

extern void blitImagePartToScreen(int sx, int sy, int width, int height, Image* source, int dx, int dy, int colormode);
extern void blitImageToScreen(int dw, int dh, Image* source, int dx, int dy, int colormode);
extern Color getPixelScreen(int x, int y);
extern void fillScreenRect(Color color, int x, int y, int width, int height);
extern void drawLineScreen(Color color, int sx, int sy, int dx, int dy);
extern void putPixelScreen(Color color, int x, int y);
extern void drawShade(Color color1, Color color2, int x, int y, int width, int height);
extern void blurScreen();

extern Image* loadImage(const char* filename);
extern Image* createImage(int width, int height);
extern void freeImage(Image* image);
extern void clearImage(Color color, Image* image);
extern Color getPixelImage(int x, int y, Image* image);
extern void saveImage(const char* filename, Color* data, int width, int height, int lineSize, int saveAlpha);
extern void blitScreenToImage(Image* dest);

extern Color* getVramDrawBuffer();
extern Color* getVramDisplayBuffer();
extern void Swizzle(Image* img);

#endif
