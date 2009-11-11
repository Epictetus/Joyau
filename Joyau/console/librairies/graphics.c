#include <stdlib.h>
#include <malloc.h>
#include <pspdisplay.h>
#include <psputils.h>
#include <png.h>
#include <pspgu.h>

#include "graphics.h"
#include "framebuffer.h"

#define IS_ALPHA(color) (((color)&0xff000000)==0xff000000?0:1)
#define FRAMEBUFFER_SIZE (PSP_LINE_SIZE*SCREEN_HEIGHT*4)
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

extern u8 msx[];

unsigned int __attribute__((aligned(16))) list[262144];
static int dispBufferNumber;
static int initialized = 0;

int getNextPower2(int width)
{
	int b = width;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * width) b >>= 1;
	return b;
}

Color* getVramDrawBuffer()
{
	Color* vram = (Color*) g_vram_base;
	if (dispBufferNumber == 0) vram += FRAMEBUFFER_SIZE / sizeof(Color);
	return vram;
}

Color* getVramDisplayBuffer()
{
	Color* vram = (Color*) g_vram_base;
	if (dispBufferNumber == 1) vram += FRAMEBUFFER_SIZE / sizeof(Color);
	return vram;
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
}

Image* loadImage(const char* filename)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type, x, y;
	u32* line;
	FILE *fp;
	Image* image = (Image*) malloc(sizeof(Image));
	if (!image) return NULL;

	if ((fp = fopen(filename, "rb")) == NULL) return NULL;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		free(image);
		fclose(fp);
		return NULL;;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		free(image);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	if (width > 512 || height > 512) {
		free(image);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}
	image->imageWidth = width;
	image->imageHeight = height;
	image->textureWidth = getNextPower2(width);
	image->textureHeight = getNextPower2(height);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	image->data = (Color*) memalign(16, image->textureWidth * image->textureHeight * sizeof(Color));
	if (!image->data) {
		free(image);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}
	line = (u32*) malloc(width * 4);
	if (!line) {
		free(image->data);
		free(image);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return NULL;
	}
	for (y = 0; y < height; y++) {
		png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
		for (x = 0; x < width; x++) {
			u32 color = line[x];
			image->data[x + y * image->textureWidth] =  color;
		}
	}
	free(line);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);
	image->swizzled=0;
	Swizzle(image);
	return image;
}

extern void blitScreenToImage(Image* dest)
{
	if (!initialized) return;
	Color* vram = getVramDisplayBuffer();
	sceGuCopyImage(GU_PSM_8888,0,0,dest->imageWidth,dest->imageHeight,getNextPower2(dest->imageWidth),vram,0,0,PSP_LINE_SIZE,dest->data);
}

extern void blitImagePartToScreen(int sx, int sy, int width, int height, Image* source, int dx, int dy, int colormode)
{
	if (!initialized) return;
	if ((dx+width<0 || dy+height<0) || (dx>480 || dy>272)) return;

	sceGuTexMode(GU_PSM_8888,0,0,source->swizzled);
	sceGuTexImage(0, source->textureWidth, source->textureWidth, source->textureWidth, (void*) source->data);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
	if (colormode & GU_PSM_8888) sceGuEnable(GU_BLEND);
	int j = 0;
	while (j < width) {
		tVertex* vertices = sceGuGetMemory(2 * sizeof(tVertex));
		int sliceWidth = 64;
		if (j + sliceWidth > width) sliceWidth = width - j;
		vertices[0].u = sx + j;
		vertices[0].v = sy;
		vertices[0].x = dx + j;
		vertices[0].y = dy;
		vertices[0].z = 0;
		vertices[1].u = sx + j + sliceWidth;
		vertices[1].v = sy + height;
		vertices[1].x = dx + j + sliceWidth;
		vertices[1].y = dy + height;
		vertices[1].z = 0;
		sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
		j += sliceWidth;
	}
}

void blitImageToScreen(int dw, int dh, Image* source, int dx, int dy, int colormode)
{
	if (!initialized || source == NULL) return;
	if ((dx+dw<0 || dy+dh<0) || (dx>480 || dy>272)) return;

	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuTexMode(GU_PSM_8888,0,0,source->swizzled);
	sceGuTexImage(0, getNextPower2(source->imageWidth), getNextPower2(source->imageHeight), source->textureWidth, (void*) source->data);

	tVertex* vertices = sceGuGetMemory(2 * sizeof(tVertex));
	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].x = dx;
	vertices[0].y = dy;
	vertices[0].z = 0;
	vertices[1].u = source->imageWidth;
	vertices[1].v = source->imageHeight;
	vertices[1].x = dx+dw;
	vertices[1].y = dy+dh;
	vertices[1].z = 0;
	sceGuEnable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
	if (colormode & GU_PSM_8888) sceGuEnable(GU_BLEND);
	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
}

Image* createImage(int width, int height)
{
	Image* image = (Image*) malloc(sizeof(Image));
	if (!image) return NULL;
	image->imageWidth = width;
	image->imageHeight = height;
	image->textureWidth = getNextPower2(width);
	image->textureHeight = getNextPower2(height);
	image->data = (Color*) memalign(16, image->textureWidth * image->textureHeight * sizeof(Color));
	if (!image->data) return NULL;
	memset(image->data, 0, image->textureWidth * image->textureHeight * sizeof(Color));
	return image;
}

void freeImage(Image* image)
{
	free(image->data);
	free(image);
}

void clearImage(Color color, Image* image)
{
	int i;
	int size = image->textureWidth * image->textureHeight;
	Color* data = image->data;
	for (i = 0; i < size; i++, data++) *data = color;
}

void clearScreen(Color color)
{
	if (!initialized) return;
	sceGuClearDepth(0);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
}

Color getPixelScreen(int x, int y)
{
	Color* vram = getVramDrawBuffer();
	return vram[PSP_LINE_SIZE * y + x];
}

Color getPixelImage(int x, int y, Image* image)
{
	return image->data[x + y * image->textureWidth];
}

void saveImage(const char* filename, Color* data, int width, int height, int lineSize, int saveAlpha)
{
	png_structp png_ptr;
	png_infop info_ptr;
	FILE* fp;
	int i, x, y;
	u8* line;
	
	if ((fp = fopen(filename, "wb")) == NULL) return;
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) return;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return;
	}
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8,
		saveAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	line = (u8*) malloc(width * (saveAlpha ? 4 : 3));
	for (y = 0; y < height; y++) {
		for (i = 0, x = 0; x < width; x++) {
			Color color = data[x + y * lineSize];
			u8 r = color & 0xff; 
			u8 g = (color >> 8) & 0xff;
			u8 b = (color >> 16) & 0xff;
			u8 a = saveAlpha ? (color >> 24) & 0xff : 0xff;
			line[i++] = r;
			line[i++] = g;
			line[i++] = b;
			if (saveAlpha) line[i++] = a;
		}
		png_write_row(png_ptr, line);
	}
	free(line);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	fclose(fp);
}

void flipScreen()
{
	if (!initialized) return;
	sceGuSwapBuffers();
	dispBufferNumber ^= 1;
}

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) /* change this if you change to another screenmode */
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2) /* zbuffer seems to be 16-bit? */

void initGraphics()
{
	dispBufferNumber = 0;

	sceGuInit();

	guStart();
	sceGuDrawBuffer(GU_PSM_8888, (void*)FRAMEBUFFER_SIZE, PSP_LINE_SIZE);
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)0, PSP_LINE_SIZE);
	sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
	sceGuDepthBuffer((void*) (FRAMEBUFFER_SIZE*2), PSP_LINE_SIZE);
	sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuDepthRange(0xc350, 0x2710);
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuAlphaFunc(GU_GREATER, 0, 0xff);
	sceGuEnable(GU_ALPHA_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuTexMode(GU_PSM_8888, 0, 0, 0);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	sceGuAmbientColor(0xffffffff);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	initialized = 1;
}

void disableGraphics()
{
	sceGuTerm();
	initialized = 0;
}

void guStart()
{
	sceGuStart(GU_DIRECT, list);
}

void fillScreenRect(Color color, int x, int y, int width, int height)
	{
  Vertex *vert = sceGuGetMemory(2 * sizeof(Vertex));
  vert[0].x = x;
  vert[0].y = y;
  vert[0].z = 0;
  vert[1].x = x+width;
  vert[1].y = y+height;
  vert[1].z = 0;
  sceGuDisable(GU_TEXTURE_2D);
  sceGuEnable(GU_BLEND);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuColor(color);
  sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vert);
	}

void drawLineScreen(Color color, int sx, int sy, int dx, int dy)
	{
  Vertex *vert = sceGuGetMemory(2 * sizeof(Vertex));
  vert[0].x = sx;
  vert[0].y = sy;
  vert[0].z = 0;
  vert[1].x = dx;
  vert[1].y = dy;
  vert[1].z = 0;
  sceGuEnable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuColor(color);
  sceGuDrawArray(GU_LINES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vert);
	}

void putPixelScreen(Color color, int x, int y)
	{
	cVertex *vert = sceGuGetMemory(sizeof(cVertex));
  vert[0].x = x;
  vert[0].y = y;
  vert[0].z = 0;
  vert[0].color = color;
  sceGuEnable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
  sceGuDrawArray(GU_POINTS,GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D,1,0,vert);
	}

void drawShade(Color color1, Color color2, int x, int y, int width, int height)
	{
	cVertex *vert = sceGuGetMemory(2 * sizeof(cVertex));
  vert[0].x = x;
  vert[0].y = y+height;
  vert[0].z = 0;
  vert[0].color = color1;
  vert[1].x = x+width;
  vert[1].y = y;
  vert[1].z = 0;
  vert[1].color = color2;
  sceGuEnable(GU_BLEND);
	sceGuDisable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
  sceGuDrawArray(GU_SPRITES, GU_COLOR_8888 | GU_VERTEX_16BIT | GU_TRANSFORM_2D,2,0,vert);
	}

#define PART_WIDTH 192
#define PART_HEIGHT 108
void blurScreen()
	{
	if (!initialized) return;

	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	tVertex* vertices = sceGuGetMemory(3 * 2 * sizeof(tVertex));
	sceGuTexMode(GU_PSM_8888,0,0,0);
	sceGuTexImage(0,getNextPower2(SCREEN_WIDTH),getNextPower2(SCREEN_WIDTH),getNextPower2(SCREEN_WIDTH),(void*)getVramDrawBuffer());
	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].x = 0;
	vertices[0].y = 0;
	vertices[0].z = 0;
	vertices[1].u = SCREEN_WIDTH;
	vertices[1].v = SCREEN_HEIGHT;
	vertices[1].x = PART_WIDTH;
	vertices[1].y = PART_HEIGHT;
	vertices[1].z = 0;
	vertices[2].u = 0;
	vertices[2].v = 0;
	vertices[2].x = SCREEN_WIDTH-PART_WIDTH;
	vertices[2].y = SCREEN_HEIGHT-PART_HEIGHT;
	vertices[2].z = 0;
	vertices[3].u = PART_WIDTH;
	vertices[3].v = PART_HEIGHT;
	vertices[3].x = SCREEN_WIDTH;
	vertices[3].y = SCREEN_HEIGHT;
	vertices[3].z = 0;
	vertices[4].u = SCREEN_WIDTH-PART_WIDTH;
	vertices[4].v = SCREEN_HEIGHT-PART_HEIGHT;
	vertices[4].x = 0;
	vertices[4].y = 0;
	vertices[4].z = 0;
	vertices[5].u = SCREEN_WIDTH;
	vertices[5].v = SCREEN_HEIGHT;
	vertices[5].x = SCREEN_WIDTH;
	vertices[5].y = SCREEN_HEIGHT;
	vertices[5].z = 0;
	sceGuEnable(GU_BLEND);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 3*2, 0, vertices);
	sceGuEnable(GU_BLEND);
	}

extern void swizzle_fast(unsigned char *dest, unsigned char *source, int width, int height)
	{
	int i,j;
	int rowblocks = (width / 16);
	int rowblocks_add = (rowblocks-1)*128;
	unsigned int block_address = 0;
	unsigned int *src = (unsigned int*)source;
	for (j = 0; j < height; j++)
		{
		unsigned int *block = (unsigned int*)((unsigned int)&dest[block_address] | 0x40000000);
		for (i = 0; i < rowblocks; i++)
			{
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			block += 28;
			}
		if ((j&0x7)==0x7)
			{
			block_address += rowblocks_add;
			}
		block_address+=16;
		}
	}
 
extern void Swizzle(Image* img)
	{
	sceKernelDcacheWritebackAll();
	if(img->swizzled || img->imageHeight+img->imageWidth<32) return;
	else
		{
		int type = 4; // GU_PSM_8888
		long size = img->textureWidth * img->textureHeight * type;
		u8* temp = (u8*)malloc(size);
		swizzle_fast( temp, (u8*)img->data, (img->textureWidth * type), img->textureHeight );
		free(img->data);
		img->data = (u32*)temp;
		img->swizzled = GU_TRUE;
		}
	sceKernelDcacheWritebackAll();
	}
