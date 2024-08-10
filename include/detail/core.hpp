#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP

#include "util.hpp"
#include "base_conv_policies.hpp"

namespace CppUnit
{
    namespace Detail
    {
        namespace Core
        {
            template <typename T,
                      typename TUnit,
                      typename TBaseUnit,
                      typename TConvPolicy>
            struct Unit
            {
                using BaseUnit = TBaseUnit;
                using UnitType = TUnit;
                using ConvPolicy = typename TConvPolicy::Policy<TUnit>;

                T m_value;

                Unit() = default;
                Unit(const T &value) : m_value(value) {}
                Unit(const TUnit &other) : m_value(other.m_value) {}
                template <typename TOtherUnit>
                Unit(const TOtherUnit &other) : m_value(Util::unit_cast<TOtherUnit, TUnit>(other).m_value) {}

                template <typename TOtherUnit>
                TUnit &operator=(const TOtherUnit &other)
                {
                    m_value = Util::unit_cast<TOtherUnit, TUnit>(other).m_value;
                    return *this;
                }

                static TUnit fromValue(const T &value)
                {
                    TUnit unit;
                    unit.m_value = value;

                    return unit;
                }

                operator T() { return m_value; }
            };

            template <typename T, typename TUnit>
            using BaseUnit = Unit<T, TUnit, TUnit, BaseConvPolicies::ItselfConvPolicy>;
        }
    }
};

#endif // UNIT_BASE_HPP
