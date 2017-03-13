#ifndef ZEUS_IMAGE_H
#define ZEUS_IMAGE_H

#include <string>
#include <memory>

struct Pixel
{
    unsigned char r, b, g, a;
};

/// 处理图片相关的功能，不包括绘图
class Image
{
public:
    enum ColorType{RGB, RGBA};
    typedef std::shared_ptr<Image> ptr;

    static Image::ptr imread(const std::string& filename);
    static void imwrite(const std::string& filename, Image::ptr image);
    static Image::ptr create(int height, int width);

    Image(const std::string& filename);
    Image(int height, int width);
    ~Image();

    void load(const std::string& filename);
    void save(const std::string& filename);
    
    int height();
    int width();
    ColorType type();
    /// x: [0:h-1]; y: [0:w-1]
    Pixel* getPixel(int x, int y);
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void fill(unsigned char r, unsigned char g, unsigned char b);
    void fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    int h, w;
    ColorType t;
    Pixel *pixels;

    void mallocMemory();

    void loadPNG(const std::string& filename);
    void savePNG(const std::string& filename);
};

#endif // ZEUS_IMAGE_H