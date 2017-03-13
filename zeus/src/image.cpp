#include <image.h>
#include <common.h>
#include "scripts/pnglibconf.h.prebuilt"
#include "png.h"

using namespace std;

Pixel::Pixel()
{
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;
}

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Image::ptr Image::imread(const std::string& filename)
{
    return make_shared<Image>(filename);
}

void Image::imwrite(const std::string& filename, Image::ptr image)
{
    image->save(filename);
}

Image::ptr Image::create(int height, int width)
{
    return make_shared<Image>(height, width);
}

Image::Image(const std::string& filename): pixels(NULL)
{
    this->load(filename);
}

Image::Image(int height, int width): pixels(NULL)
{
    this->w = width;
    this->h = height;
    this->t = RGBA;
    this->mallocMemory();
    this->fill(255, 255, 255, 255);
}

Image::~Image()
{
    if (pixels) delete[] pixels;
    pixels = NULL;
}

void Image::load(const std::string& filename)
{
    loadPNG(filename); // FIXME
}

void Image::save(const std::string& filename)
{
    savePNG(filename); // FIXME
}

int Image::height()
{
    return this->h;
}

int Image::width()
{
    return this->w;
}

Image::ColorType Image::type()
{
    return this->t;
}

Pixel* Image::getPixel(int x, int y)
{
    if(0 <= x && x < h);else LOG_DEBUG << "x " << x;
    if(0 <= y && y < w);else LOG_DEBUG << "y " << y;
    ASSERT(0 <= x && x < h);
    ASSERT(0 <= y && y < w);
    return pixels+x*w+y;
}

void Image::setPixel(int x, int y, Pixel pixel)
{
    *getPixel(x, y) = pixel;
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    getPixel(x,y)->r = r;
    getPixel(x,y)->g = g;
    getPixel(x,y)->b = b;
}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    getPixel(x,y)->r = r;
    getPixel(x,y)->g = g;
    getPixel(x,y)->b = b;
    getPixel(x,y)->a = a;
}

void Image::fill(unsigned char r, unsigned char g, unsigned char b)
{
    for(int i=0;i<this->h;i++)
        for(int j=0;j<this->w;j++)
        {
            getPixel(i,j)->r = r;
            getPixel(i,j)->g = g;
            getPixel(i,j)->b = b;
        }
}

void Image::fill(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    for(int i=0;i<this->h;i++)
        for(int j=0;j<this->w;j++)
        {
            getPixel(i,j)->r = r;
            getPixel(i,j)->g = g;
            getPixel(i,j)->b = b;
            getPixel(i,j)->a = a;
        }
}

void Image::mallocMemory()
{
    if(pixels) delete[] pixels;
    pixels = new Pixel[w*h];
}

#define PNG_BYTES_TO_CHECK 4
void Image::loadPNG(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "rb");
    ASSERT(fp);

    png_structp png_ptr;
    png_infop info_ptr;
    char buf[PNG_BYTES_TO_CHECK]; 
    
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);
    
    if(setjmp(png_jmpbuf(png_ptr)))
    {
        FATAL;
    }
    /* 读取PNG_BYTES_TO_CHECK个字节的数据 */  
    int temp = fread(buf, 1, PNG_BYTES_TO_CHECK, fp);
    ASSERT(temp == PNG_BYTES_TO_CHECK)
    /* 检测数据是否为PNG的签名 */
    temp = png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK );
    ASSERT(temp == 0);
    
    /* 复位文件指针 */
    rewind(fp);
    /* 开始读文件 */
    png_init_io(png_ptr, fp);
    /* 读取PNG图片信息 */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
    /* 获取图像的色彩类型 */
    int color_type = png_get_color_type(png_ptr, info_ptr);
    /* 获取图像的宽高 */
    this->w = png_get_image_width(png_ptr, info_ptr);
    this->h = png_get_image_height(png_ptr, info_ptr);
    this->mallocMemory();
    /* 获取图像的所有行像素数据，row_pointers里边就是rgba数据 */
    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);  
    /* 根据不同的色彩类型进行相应处理 */
    switch(color_type) {
        case PNG_COLOR_TYPE_RGB_ALPHA:
            this->t = RGBA;
            for(int x=0; x<h; x++) {
                for(int y=0; y<w; y++) {
                    getPixel(x,y)->r = row_pointers[x][y*4+0];
                    getPixel(x,y)->g = row_pointers[x][y*4+1];
                    getPixel(x,y)->b = row_pointers[x][y*4+2];
                    getPixel(x,y)->a = row_pointers[x][y*4+3];
                }
            }
            break;
        case PNG_COLOR_TYPE_RGB:
            this->t = RGB;
            for(int x=0; x<h; x++) {
                for(int y=0; y<w; y++) {
                    getPixel(x,y)->r = row_pointers[x][y*3+0];
                    getPixel(x,y)->g = row_pointers[x][y*3+1];
                    getPixel(x,y)->b = row_pointers[x][y*3+2];
                    getPixel(x,y)->a = 255;
                }
            }
            break;
        /* 其它色彩类型的图像就不读了 */  
        default:break;
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);
}

void Image::savePNG(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "wb");
    ASSERT(fp);

    png_structp png_ptr;
    png_infop info_ptr;
  
    /* 创建并初始化 png_struct 及其所需的错误处理函数，如果你想使用默 
    * 认的 stderr 和 longjump() 方法，你可以将最后三个参数设为 NULL， 
    * 在使用动态链接库的情况下，我们也会检测函数库版本是否与在编译时
    * 使用的版本是否兼容。（必要
    */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    ASSERT(png_ptr);
    
    /* 分配内存并初始化图像信息数据。（必要）*/
    info_ptr = png_create_info_struct(png_ptr);
    ASSERT(info_ptr);
    
    /* 设置错误处理。如果你在调用 png_create_write_struct() 时没 
    * 有设置错误处理函数，那么这段代码是必须写的。*/
    if(setjmp(png_jmpbuf(png_ptr)))
    {
        FATAL;
    }
    
    /* 设置输出控制，如果你使用的是 C 的标准 I/O 流 */
    png_init_io(png_ptr, fp);
  
    /* （必需）在这里设置图像的信息，宽度、高度的上限是 2^31。
    * bit_depth 取值必需是 1、2、4、8 或者 16, 但是可用的值也依赖于 color_type。
    * color_type 可选值有： PNG_COLOR_TYPE_GRAY、PNG_COLOR_TYPE_GRAY_ALPHA、
    * PNG_COLOR_TYPE_PALETTE、PNG_COLOR_TYPE_RGB、PNG_COLOR_TYPE_RGB_ALPHA。
    * interlace 可以是 PNG_INTERLACE_NONE 或 PNG_INTERLACE_ADAM7,
    * 而 compression_type 和 filter_type 目前必需是 PNG_COMPRESSION_TYPE_BASE
    * 和 and PNG_FILTER_TYPE_BASE。
    */
    switch(this->t) {
        case RGB:
            png_set_IHDR(png_ptr, info_ptr, this->w, this->h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        case RGBA:
            png_set_IHDR(png_ptr, info_ptr, this->w, this->h, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        break;
    }
    
    /* 写入文件头部信息（必需） */
    png_write_info(png_ptr, info_ptr);
    
    /* 在这个示例代码中，"image" 是一个一维的字节数组（每个元素占一个字节空间） */
    const int bytes_per_pixel = this->t == RGB ? 3 : 4;
    png_byte *image = new png_byte[this->h*this->w*bytes_per_pixel];
    for(int x = 0; x < this->h; x ++)
        for(int y = 0; y < this->w; y ++)
        {
            image[(x*this->w+y)*bytes_per_pixel+0] = getPixel(x, y)->r;
            image[(x*this->w+y)*bytes_per_pixel+1] = getPixel(x, y)->g;
            image[(x*this->w+y)*bytes_per_pixel+2] = getPixel(x, y)->b;
            if(this->t == RGBA)image[(x*this->w+y)*bytes_per_pixel+3] = getPixel(x, y)->a;
        }

    png_bytep row_pointers[this->h];
    
    /* 将这些像素行指针指向你的 "image" 字节数组中对应的位置，即：指向每行像素的起始处 */
    for (int k = 0; k < this->h; k++)
        row_pointers[k] = image + k*this->w*bytes_per_pixel;
  
    /* 一次调用就将整个图像写进文件 */
    png_write_image(png_ptr, row_pointers);
    /* 必需调用这个函数完成写入文件其余部分 */
    png_write_end(png_ptr, info_ptr);
    /* 写完后清理并释放已分配的内存 */
    png_destroy_write_struct(&png_ptr, &info_ptr);
    /* 关闭文件 */
    fclose(fp);
    delete[] image;
}