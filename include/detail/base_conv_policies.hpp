#ifndef BASE_CONV_POLICIES_HPP
#define BASE_CONV_POLICIES_HPP

#include "util.hpp"

namespace CppUnit
{
    namespace Detail
    {
        namespace BaseConvPolicies
        {
            struct ItselfConvPolicy
            {
                template <typename T>
                struct Policy
                {
                    static T toBase(const T &v) { return v; }
                    static T fromBase(const T &v) { return v; }
                };
            };

            template <typename TRatio>
            struct RatioConvPolicy
            {
                // Unit = Base * numerator / denominator
                template <typename TUnit>
                struct Policy
                {
                    using BaseUnit_t = typename Util::GetBaseUnit<TUnit>::type;

                    static BaseUnit_t toBase(const TUnit &unit) { return BaseUnit_t::fromValue(unit.m_value / TRatio::den * TRatio::num); }
                    static TUnit fromBase(const BaseUnit_t &base) { return TUnit::fromValue(base.m_value / TRatio::num * TRatio::den); }
                };
            };
        }
    }
}

#endif // BASE_CONV_POLICIES_HPP
