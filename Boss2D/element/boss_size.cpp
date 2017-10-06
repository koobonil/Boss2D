#include <boss.hpp>
#include "boss_size.hpp"

namespace BOSS
{
    Size::Size()
    {
        w = 0;
        h = 0;
    }

    Size::Size(const Size& rhs)
    {
        operator=(rhs);
    }

    Size::Size(float w, float h)
    {
        this->w = w;
        this->h = h;
    }

    Size::~Size()
    {
    }

    Size& Size::operator=(const Size& rhs)
    {
        w = rhs.w;
        h = rhs.h;
        return *this;
    }

    bool Size::operator==(const Size& rhs) const
    {
        return (w == rhs.w && h == rhs.h);
    }

    bool Size::operator!=(const Size& rhs) const
    {
        return !operator==(rhs);
    }
}
