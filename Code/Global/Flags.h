#pragma once

#include <cstdint>
#include <initializer_list>

namespace ge2
{

    //Flags is a bit-array templated over an integer (usually enum) type
    //The highest flag value should be less than the number of bits in BIT_ARRAY_TYPE
    //FLAG_TYPE must be static castable to BIT_ARRAY_TYPE
    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE = std::uint16_t>
    class Flags
    {
    public:
        Flags() = default;
        Flags(BIT_ARRAY_TYPE);
        Flags(std::initializer_list<FLAG_TYPE> flags);

        bool IsFlagRaised(FLAG_TYPE flag) const;
        void RaiseFlag(FLAG_TYPE flag);
        void LowerFlag(FLAG_TYPE flag);

        bool AreFlagsRaised(std::initializer_list<FLAG_TYPE>) const;
        void RaiseFlags(std::initializer_list<FLAG_TYPE>);
        void LowerFlags(std::initializer_list<FLAG_TYPE>);

        BIT_ARRAY_TYPE RawFlags() const;

    private:
        BIT_ARRAY_TYPE ConvertFlag(FLAG_TYPE) const;
        BIT_ARRAY_TYPE m_flags = 0;
    };

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::Flags(BIT_ARRAY_TYPE flags)
    {
        m_flags = flags;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::Flags(std::initializer_list<FLAG_TYPE> flags)
    {
        RaiseFlags(flags);
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    bool Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::IsFlagRaised(FLAG_TYPE flag) const
    {
        BIT_ARRAY_TYPE numericFlag = ConvertFlag(flag);
        return numericFlag & m_flags;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline void Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::RaiseFlag(FLAG_TYPE flag)
    {
        BIT_ARRAY_TYPE numericFlag = ConvertFlag(flag);
        m_flags |= numericFlag;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline void Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::LowerFlag(FLAG_TYPE flag)
    {
        BIT_ARRAY_TYPE numericFlag = ConvertFlag(flag);
        m_flags &= ~m_flags;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline bool Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::AreFlagsRaised(std::initializer_list<FLAG_TYPE> flags) const
    {
        for (auto flag : flags)
        {
            if (!IsFlagRaised(flag))
            {
                return false;
            }
        }

        return true;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline void Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::RaiseFlags(std::initializer_list<FLAG_TYPE> flags)
    {
        for (auto flag : flags)
        {
            RaiseFlag(flag);
        }
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline void Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::LowerFlags(std::initializer_list<FLAG_TYPE> flags)
    {
        for (auto flag : flags)
        {
            LowerFlag(flag);
        }
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline BIT_ARRAY_TYPE Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::RawFlags() const
    {
        return m_flags;
    }

    template<typename FLAG_TYPE, typename BIT_ARRAY_TYPE>
    inline BIT_ARRAY_TYPE Flags<FLAG_TYPE, BIT_ARRAY_TYPE>::ConvertFlag(FLAG_TYPE flag) const
    {
        _ASSERT(BIT_ARRAY_TYPE(flag) < sizeof(BIT_ARRAY_TYPE) * 8);
        return 1 << static_cast<BIT_ARRAY_TYPE>(flag);
    }

}