#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>

#pragma pack(push,1)
struct TGA_Header
{
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

struct TGAColor
{
	union
    {
		struct
        {
			unsigned char b, g, r, a;
		};
		unsigned char raw[4];
		unsigned int val;
	};
	int bytespp;

	TGAColor() : val(0), bytespp(1) {}

	TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A)
        : b(B), g(G), r(R), a(A), bytespp(4) {}

	TGAColor(int v, int bpp) : val(v), bytespp(bpp) {}

	TGAColor(const TGAColor &c) : val(c.val), bytespp(c.bytespp) {}

	TGAColor(const unsigned char *p, int bpp) : val(0), bytespp(bpp)
    {
		for (int i=0; i<bpp; i++)
        {
			raw[i] = p[i];
		}
	}

	TGAColor & operator =(const TGAColor &c)
    {
		if (this != &c)
        {
			bytespp = c.bytespp;
			val = c.val;
		}
		return *this;
	}
    
    void scale(float s)
    {
        r = std::min<int>((int)r * s, 255);
        g = std::min<int>((int)g * s, 255);
        b = std::min<int>((int)b * s, 255);
    }

    TGAColor operator +(const TGAColor& c) const
    {
        TGAColor res(0, 3);
        if ((bytespp == 3 || bytespp == 4) && (c.bytespp == 3 || c.bytespp == 4))
        {
            res.r = std::min<int>((int)r + c.r, 255);
            res.g = std::min<int>((int)g + c.g, 255);
            res.b = std::min<int>((int)b + c.b, 255);
        }
        else if(bytespp == 1 && c.bytespp == 1)
        {
            char val = std::min<int>((int)raw[0] + c.raw[0], 255);
            res.r = val;
            res.g = val;
            res.b = val;
        }
        else if(bytespp == 1)
        {
            res.r = std::min<int>((int)raw[0] + c.r, 255);
            res.g = std::min<int>((int)raw[0] + c.g, 255);
            res.b = std::min<int>((int)raw[0] + c.b, 255);
        }
        else
        {
            res.r = std::min<int>((int)r + c.raw[0], 255);
            res.g = std::min<int>((int)g + c.raw[0], 255);
            res.b = std::min<int>((int)b + c.raw[0], 255);
        }
        return res;
    }
};


class TGAImage
{
protected:
	unsigned char* data;
	int width;
	int height;
	int bytespp;

	bool   load_rle_data(std::ifstream &in);
	bool unload_rle_data(std::ofstream &out);
public:
	enum Format
    {
		GRAYSCALE = 1,
        RGB = 3,
        RGBA = 4
	};

	TGAImage();
	TGAImage(int w, int h, int bpp);
	TGAImage(const TGAImage &img);
	bool read_tga_file(const char *filename);
	bool write_tga_file(const char *filename, bool rle=true);
	bool flip_horizontally();
	bool flip_vertically();
	bool scale(int w, int h);
	TGAColor get(int x, int y);
	TGAColor get_from_ratio(float x, float y) const;
	bool set(int x, int y, TGAColor c);
	~TGAImage();
	TGAImage & operator =(const TGAImage &img);
	int get_width();
	int get_height();
	int get_bytespp();
	unsigned char *buffer();
	void clear();
};

#endif //__IMAGE_H__
