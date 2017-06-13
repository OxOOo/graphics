#ifndef LIGHT_H
#define LIGHT_H

// 光照
class Light
{
public:
    Light();
    virtual ~Light();
    typedef shared_ptr<Light> ptr;
};

#endif // LIGHT_H