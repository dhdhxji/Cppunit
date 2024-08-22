#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <utility>
#include "core.hpp"
#include "util.hpp"

namespace CppUnit
{
    namespace Detail
    {
        namespace Composite
        {
            template <
                typename TUnit1,
                typename TUnit2
            >
            struct MultipliedUnit
            {
                using BaseUnit = MultipliedUnit<
                    typename Util::GetBaseUnit<TUnit1, false>::type, 
                    typename Util::GetBaseUnit<TUnit2, false>::type>;
                using UnitType = MultipliedUnit<
                    typename TUnit1::UnitType, 
                    typename TUnit2::UnitType>;
                using OwnBaseUnit = MultipliedUnit<
                    typename Util::GetBaseUnit<TUnit1, true>::type, 
                    typename Util::GetBaseUnit<TUnit2, true>::type>;
                using OwnUnitType = MultipliedUnit<
                    typename TUnit1::OwnUnitType,
                    typename TUnit2::OwnUnitType>;

                TUnit1 m_value1;
                TUnit2 m_value2;

                static OwnUnitType from_value(const TUnit1 &unit1, const TUnit2 &unit2)
                {
                    return OwnUnitType {
                        unit1,
                        unit2
                    };
                }

                decltype(m_value1.value()) value()
                {
                    return m_value1.value() * m_value2.value();
                }
            };
        }

        namespace Util
        {
            template <typename Unit1, typename Unit2, typename Unit3, typename Unit4>
            struct UnitCaster<Composite::MultipliedUnit<Unit1, Unit2>, Composite::MultipliedUnit<Unit3, Unit4>>
            {
                static inline Composite::MultipliedUnit<Unit3, Unit4> cast(const Composite::MultipliedUnit<Unit1, Unit2> &unit)
                {
                    return Composite::MultipliedUnit<Unit3, Unit4>::from_value(
                        unit_cast<Unit1, Unit3>(unit.m_value1),
                        unit_cast<Unit2, Unit4>(unit.m_value2)
                    );
                }
            };
        }

    }
}

#endif // COMPOSITE_HPP
