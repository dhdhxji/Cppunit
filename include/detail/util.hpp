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
                using type = typename TUnit::BaseType;
            };

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
                                          std::is_same<
                                              typename GetRootBaseUnit<TUnit1>::type,
                                              typename GetRootBaseUnit<TUnit2>::type>::value>
            {
            };

            template <typename TFrom, typename TTo>
            TTo unit_cast(const TFrom &unit)
            {
                static_assert(IsUnitCompatible<TFrom, TTo>::value, "Units TFrom and TTo should have common base unit");
                const typename GetBaseUnit<TFrom>::type baseUnit = TFrom::ConvPolicy::toBase(unit);
                const TTo result = TTo::ConvPolicy::fromBase(baseUnit);

                return result;
            }
        }
    }
}

#endif // UTIL_HPP
