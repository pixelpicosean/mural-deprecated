#pragma once

#include "../../common/StringUtil.h"
#include <map>

namespace mural
{

class Texture;
class SharedTextureCache
{
public:
    std::map<String, Texture*> textures;

public:
    static SharedTextureCache& getInstance()
    {
        static SharedTextureCache instance;
        return instance;
    }

    // void releaseStoragesOlderThan(/*seconds*/);
private:
    SharedTextureCache() {}
    SharedTextureCache(SharedTextureCache&) {}
    void operator=(SharedTextureCache&) {}
    ~SharedTextureCache();
};

#define theTextureCache SharedTextureCache::getInstance()

}
