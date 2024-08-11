#ifndef UTIL_HPP
#define UTIL_HPP

#include <type_traits>

namespace CppUnit
{
    namespace Detail
    {
        namespace Util
        {
            template <typename TUnit>
            struct GetBaseUnit
            {
                using type = typename TUnit::BaseUnit;
            };

            template <typename TUnit1, typename TUnit2>
            struct IsUnitsSame : std::bool_constant<std::is_same<typename TUnit1::UnitType, typename TUnit2::UnitType>::value>
            {};

            template <typename TUnit>
            struct GetRootBaseUnit
            {
            private:
                template <typename TUnitInternal, typename TBaseUnitInternal>
                struct GetRootBaseUnitImpl
                {
                    using type = typename GetRootBaseUnitImpl<TBaseUnitInternal, typename GetBaseUnit<TBaseUnitInternal>::type>::type;
                };

                template <typename TUnitInternal>
                struct GetRootBaseUnitImpl<TUnitInternal, TUnitInternal>
                {
                    using type = TUnitInternal;
                };

            public:
                using type = typename GetRootBaseUnitImpl<TUnit, typename GetBaseUnit<TUnit>::type>::type;
            };

            template <typename TUnit1, typename TUnit2>
            struct IsUnitCompatible : std::bool_constant<
                                          IsUnitsSame<
                                              typename GetRootBaseUnit<TUnit1>::type,
                                              typename GetRootBaseUnit<TUnit2>::type>::value>
            {
            };

            template <typename TUnit1, typename TUnit2>
            struct GetCommonBaseUnit
            {
                static_assert(IsUnitCompatible<TUnit1, TUnit2>::value);
                using type = typename GetRootBaseUnit<TUnit1>::type;
            };

            template <typename TUnit, typename TTarget>
            struct ToBaseCastRecursive
            {
            private:
                using BaseUnit = typename GetBaseUnit<TUnit>::type;

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
                using TargetBaseUnit = typename GetBaseUnit<TTarget>::type;

                struct WrapperPolicy
                {
                    TTarget fromBase(const TUnit &base)
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
                using CommonBase = typename GetCommonBaseUnit<TFrom, TTo>::type;

                const CommonBase base = ToBaseCastRecursive<TFrom, CommonBase>::type::toBase(unit);
                return FromBaseCastRecursive<CommonBase, TTo>::type::fromBase(base);
            }
        }
    }
}

#endif // UTIL_HPP
