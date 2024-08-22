#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <utility>
#include <functional>
#include "core.hpp"
#include "util.hpp"

namespace CppUnit
{
    namespace Detail
    {
        namespace Composite
        {
            template<typename, typename, template <typename> typename>
            struct CompositeUnit;

            template <typename TUnit1, typename TUnit2>
            using MultipliedUnit = CompositeUnit<TUnit1, TUnit2, std::multiplies>;

            template <typename TUnit1, typename TUnit2>
            using DividedUnit = CompositeUnit<TUnit1, TUnit2, std::divides>;


            template <
                typename TUnit1,
                typename TUnit2,
                template <typename> typename TOpFunctor
            >
            struct CompositeUnit
            {
                using BaseUnit = CompositeUnit<
                    typename Util::GetBaseUnit<TUnit1, false>::type, 
                    typename Util::GetBaseUnit<TUnit2, false>::type,
                    TOpFunctor>;
                using UnitType = CompositeUnit<
                    typename TUnit1::UnitType, 
                    typename TUnit2::UnitType,
                    TOpFunctor>;
                using OwnBaseUnit = CompositeUnit<
                    typename Util::GetBaseUnit<TUnit1, true>::type, 
                    typename Util::GetBaseUnit<TUnit2, true>::type,
                    TOpFunctor>;
                using OwnUnitType = CompositeUnit<
                    typename TUnit1::OwnUnitType,
                    typename TUnit2::OwnUnitType,
                    TOpFunctor>;

                TUnit1 m_value1;
                TUnit2 m_value2;

                static OwnUnitType fromValue(const TUnit1 &unit1, const TUnit2 &unit2)
                {
                    OwnUnitType result;
                    result.m_value1 = unit1;
                    result.m_value2 = unit2;
                    return result;
                }

                decltype(m_value1.value()) value()
                {
                    return TOpFunctor()(m_value1.value(), m_value2.value());
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
        }

        namespace Util
        {
            template <
                typename Unit1, 
                typename Unit2, 
                typename Unit3, 
                typename Unit4, 
                template <typename> typename TOpFunctor
            >
            struct UnitCaster<Composite::CompositeUnit<Unit1, Unit2, TOpFunctor>, Composite::CompositeUnit<Unit3, Unit4, TOpFunctor>>
            {
                static inline Composite::CompositeUnit<Unit3, Unit4, TOpFunctor> cast(const Composite::CompositeUnit<Unit1, Unit2, TOpFunctor> &unit)
                {
                    return Composite::CompositeUnit<Unit3, Unit4, TOpFunctor>::fromValue(
                        unit_cast<Unit1, Unit3>(unit.m_value1),
                        unit_cast<Unit2, Unit4>(unit.m_value2)
                    );
                }
            };

            template <
                typename Unit1, 
                typename Unit2, 
                typename Unit3, 
                typename Unit4, 
                template <typename> typename TOpFunctor1,
                template <typename> typename TOpFunctor2
            >
            struct UnitCaster<Composite::CompositeUnit<Unit1, Unit2, TOpFunctor1>, Composite::CompositeUnit<Unit3, Unit4, TOpFunctor2>>
            {
                static_assert("Error casting composite units with different Op");
            };
        }
    }
}

#endif // COMPOSITE_HPP
