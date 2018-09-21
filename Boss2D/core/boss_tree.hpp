#pragma once
#include <boss_map.hpp>

namespace BOSS
{
    //! \brief 트리지원
    template<typename TYPE>
    class Tree
    {
    public:
	    Context& operator()(chars key)
	    {
	    }

	    const Context& operator()(chars key) const
	    {
	    }

	    Context& operator[](sint32 index)
	    {
	    }

	    const Context& operator[](sint32 index) const
	    {
	    }

	    Context& AtAdding()
	    {
	    }

	    sint32 Count() const
	    {
	    }

    public:
	    TYPE* operator->()
	    {
	    }
	
	    TYPE const* operator->() const
	    {
	    }
	
	    TYPE& Value()
	    {
	    }
	
	    TYPE const& ConstValue() const
	    {
	    }
	
	    TYPE* Ptr()
	    {
	    }

	    TYPE const* ConstPtr() const
	    {
	    }
    };
}
