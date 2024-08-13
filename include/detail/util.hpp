#ifndef UTIL_HPP
#define UTIL_HPP

#include <type_traits>

namespace CppUnit
{
    namespace Detail
    {
        namespace Util
        {
            template <typename TUnit, bool OwnUnit>
            struct GetBaseUnit
            {
                using type = typename std::conditional<
                    OwnUnit,
                    typename TUnit::OwnBaseUnit,
                    typename TUnit::BaseUnit>::type;
            };

            template <typename TUnit1, typename TUnit2>
            struct IsUnitsSame : std::bool_constant<std::is_same<typename TUnit1::UnitType, typename TUnit2::UnitType>::value>
            {};

            template <typename TUnit, bool OwnUnit>
            struct GetRootBaseUnit
            {
            private:
                template <typename TUnitInternal, typename TBaseUnitInternal>
                struct GetRootBaseUnitImpl
                {
                    using type = typename GetRootBaseUnitImpl<TBaseUnitInternal, typename GetBaseUnit<TBaseUnitInternal, OwnUnit>::type>::type;
                };

                template <typename TUnitInternal>
                struct GetRootBaseUnitImpl<TUnitInternal, TUnitInternal>
                {
                    using type = TUnitInternal;
                };

            public:
                using type = typename GetRootBaseUnitImpl<TUnit, typename GetBaseUnit<TUnit, OwnUnit>::type>::type;
            };

            template <typename TUnit1, typename TUnit2>
            struct IsUnitCompatible : std::bool_constant<
                                          IsUnitsSame<
                                              typename GetRootBaseUnit<TUnit1, false>::type,
                                              typename GetRootBaseUnit<TUnit2, false>::type>::value>
            {
            };

            template <typename TUnit1, typename TUnit2, bool OwnUnit>
            struct GetCommonBaseUnit
            {
                static_assert(IsUnitCompatible<TUnit1, TUnit2>::value);
                using type1 = typename GetRootBaseUnit<TUnit1, OwnUnit>::type;
                using type2 = typename GetRootBaseUnit<TUnit2, OwnUnit>::type;
            };

            template <typename TUnit, typename TTarget>
            struct ToBaseCastRecursive
            {
            private:
                using BaseUnit = typename GetBaseUnit<TUnit, true>::type;

                struct WrapperPolicy
                {
                    static TTarget toBase(const TUnit &unit)
                    {
                        return ToBaseCastRecursive<BaseUnit, TTarget>::type::toBase(
                            TUnit::ConvPolicy::toBase(unit));
                    }
                };

            public:
                using type = typename std::conditional<
                    IsUnitsSame<BaseUnit, TTarget>::value,
                    typename TUnit::ConvPolicy,
                    WrapperPolicy>::type;
            };

            template <typename TUnit, typename TTarget>
            struct FromBaseCastRecursive
            {
            private:
                using TargetBaseUnit = typename GetBaseUnit<TTarget, true>::type;

                struct WrapperPolicy
                {
                    static TTarget fromBase(const TUnit &base)
                    {
                        return TTarget::ConvPolicy::fromBase(
                            FromBaseCastRecursive<TUnit, TargetBaseUnit>::type::fromBase(base));
                    }
                };

            public:
                using type = typename std::conditional<
                    IsUnitsSame<TUnit, TargetBaseUnit>::value,
                    typename TTarget::ConvPolicy,
                    WrapperPolicy>::type;
            };

            template <typename TFrom, typename TTo>
            TTo unit_cast(const TFrom &unit)
            {
                static_assert(IsUnitCompatible<TFrom, TTo>::value, "Units TFrom and TTo must have common base unit");

                using CommonBase1 = typename GetCommonBaseUnit<TFrom, TTo, true>::type1;
                using CommonBase2 = typename GetCommonBaseUnit<TFrom, TTo, true>::type2;

                // Units may have different container type, still, Meters<int> and Meters<double> are the same units which should be converted
                const CommonBase1 base1 = ToBaseCastRecursive<TFrom, CommonBase1>::type::toBase(unit);
                const CommonBase2 base2 = CommonBase2::fromValue(base1.m_value);

                return FromBaseCastRecursive<CommonBase2, TTo>::type::fromBase(base2);
            }
        }
    }
}

#endif // UTIL_HPP
