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
                      template <typename> typename TUnit,
                      template <typename> typename TBaseUnit,
                      typename TConvPolicy>
            struct Unit
            {
                // Small hack to make units with different data types (like a Meters<double> and Millimeters<int> compatible)
                using BaseUnit = TBaseUnit<int>;
                using UnitType = TUnit<int>;
                using OwnBaseUnit = TBaseUnit<T>;
                using OwnUnitTtype = TUnit<T>;
                
                using ConvPolicy = typename TConvPolicy::Policy<OwnUnitTtype>;

                T m_value;

                Unit() = default;
                Unit(const T &value) : m_value(value) {}
                Unit(const OwnUnitTtype &other) : m_value(other.m_value) {}
                template <typename TOtherUnit>
                Unit(const TOtherUnit &other) : m_value(Util::unit_cast<TOtherUnit, OwnUnitTtype>(other).m_value) {}

                template <typename TOtherUnit>
                OwnUnitTtype &operator=(const TOtherUnit &other)
                {
                    m_value = Util::unit_cast<TOtherUnit, OwnUnitTtype>(other).m_value;
                    return *this;
                }

                static OwnUnitTtype fromValue(const T &value)
                {
                    OwnUnitTtype unit;
                    unit.m_value = value;

                    return unit;
                }

                operator T() { return m_value; }
            };

            template <typename T, template <typename> typename TUnit>
            using BaseUnit = Unit<T, TUnit, TUnit, BaseConvPolicies::ItselfConvPolicy>;
        }
    }
};

#endif // UNIT_BASE_HPP
