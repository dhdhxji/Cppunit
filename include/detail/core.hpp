#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP

#include "util.hpp"
#include "composite.hpp"
#include "base_conv_policies.hpp"

namespace CppUnit
{
    namespace Detail
    {
        namespace Core
        {
            struct DummyUnitContainer {};

            template <typename T,
                      template <typename> typename TUnit,
                      template <typename> typename TBaseUnit,
                      typename TConvPolicy>
            struct Unit
            {
                // Small hack to make units with different data types (like a Meters<double> and Millimeters<int> compatible)
                using BaseUnit = TBaseUnit<DummyUnitContainer>;
                using UnitType = TUnit<DummyUnitContainer>;
                using OwnBaseUnit = TBaseUnit<T>;
                using OwnUnitType = TUnit<T>;
                
                using ConvPolicy = typename TConvPolicy::Policy<OwnUnitType>;

                T m_value;

                Unit() = default;
                Unit(const T &value) : m_value(value) {}
                Unit(const OwnUnitType &other) : m_value(other.m_value) {}
                template <typename TOtherUnit>
                Unit(const TOtherUnit &other) : m_value(Util::unit_cast<TOtherUnit, OwnUnitType>(other).m_value) {}

                template <typename TOtherUnit>
                OwnUnitType &operator=(const TOtherUnit &other)
                {
                    m_value = Util::unit_cast<TOtherUnit, OwnUnitType>(other).m_value;
                    return *this;
                }

                static OwnUnitType fromValue(const T &value)
                {
                    OwnUnitType unit;
                    unit.m_value = value;

                    return unit;
                }

                operator T() { return m_value; }

                T value()
                {
                    return m_value;
                }

                template <typename TOtherUnit>
                Composite::MultipliedUnit<OwnUnitType, TOtherUnit> operator*(const TOtherUnit &other)
                {
                    return Composite::MultipliedUnit<OwnUnitType, TOtherUnit>::fromValue(*this, other);
                }

                template <typename TOtherUnit>
                Composite::DividedUnit<OwnUnitType, TOtherUnit> operator/(const TOtherUnit &other)
                {
                    return Composite::DividedUnit<OwnUnitType, TOtherUnit>::fromValue(*this, other);
                }
            };

            template <typename T, template <typename> typename TUnit>
            using BaseUnit = Unit<T, TUnit, TUnit, BaseConvPolicies::ItselfConvPolicy>;
        }
    }
};

#endif // UNIT_BASE_HPP
