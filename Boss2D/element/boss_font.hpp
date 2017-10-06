#pragma once
#include <boss.hpp>

namespace BOSS
{
    //! \brief 재질-폰트
    class Font
    {
    public:
        Font();
        Font(const Font& rhs);
        Font(chars name, float size);
        ~Font();
        Font& operator=(const Font& rhs);

    public:
        String name;
        float size;
    };
    typedef Array<Font> Fonts;
}
