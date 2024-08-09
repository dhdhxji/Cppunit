#include <type_traits>
#include <functional>
#include <ratio>
#include <iostream>

// TODO: typename vs class
template<typename T,
         typename TBaseUnit,
         typename TUnit,
         typename TConvPolicy>
struct Unit
{
    using BaseType = TBaseUnit;
    using UnitType = TUnit;
    using ConvPolicy = typename TConvPolicy::Policy<TUnit>;

    T m_value;
    static TUnit fromValue(const T& value) 
    {
        TUnit unit;
        unit.m_value = value;

        return unit;
    }
};

struct ItselfConvPolicy
{
    template <typename T>
    struct Policy {
        static T toBase(const T &v) {return v;}
        static T fromBase(const T &v) {return v;}
    };
};

template <typename TUnit>
struct GetBaseUnit {
    using type = typename TUnit::BaseType;
};

// Check for c++17 here
// template <typename TUnit>
// using BaseUnit_t = typename BaseUnit<TUnit>::type;


// Remove Policy from name
template <typename TRatio>
struct RatioConvPolicy
{
    // Unit = Base * numerator / denominator
    template <typename TUnit>
    struct Policy {
        using BaseUnit_t = typename GetBaseUnit<TUnit>::type;

        static BaseUnit_t toBase(const TUnit &unit) { return BaseUnit_t::fromValue(unit.m_value / TRatio::den * TRatio::num); }
        static TUnit fromBase(const BaseUnit_t &base) { return TUnit::fromValue(base.m_value / TRatio::num * TRatio::den); }
    };
};

template <typename T, typename TUnit>
using BaseUnit = Unit<T, TUnit, TUnit, ItselfConvPolicy>;


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
        typename GetRootBaseUnit<TUnit2>::type
    >::value
> {};


template <typename TFrom, typename TTo>
TTo unit_cast(const TFrom &unit)
{
    static_assert(IsUnitCompatible<TFrom, TTo>::value, "Units TFrom and TTo should have common base unit");
    const typename GetBaseUnit<TFrom>::type baseUnit = TFrom::ConvPolicy::toBase(unit);
    const TTo result = TTo::ConvPolicy::fromBase(baseUnit);

    return result;
}

template <typename T>
T unit_cast(const T &unit) {return unit;}



struct Meters : public BaseUnit<double, Meters> {};

struct Kilometers : public Unit<double, Meters, Kilometers, RatioConvPolicy<std::ratio<1000, 1>>> {};
struct Millimeters : public Unit<double, Meters, Millimeters, RatioConvPolicy<std::ratio<1, 1000>>> {};


// TODO: Automatic SI unit conversion (struct Megameters : public SIMegaUnit<double, Meters, Megameters>)

struct MegametersConv {
    template <typename TUnit>
    struct Policy {
        using BaseType_t = typename TUnit::BaseType;

        static BaseType_t toBase(const TUnit &Mm) {return BaseType_t::fromValue(Mm.m_value * 1000);}
        static TUnit fromBase(const BaseType_t &km) {return TUnit::fromValue(km.m_value / 1000);}
    };
};
struct Megameters : public Unit<double, Kilometers, Megameters, MegametersConv> {};



struct Grams : public BaseUnit<double, Grams>{};



int main()
{
    // Millimeters mm{10};
    // Kilometers km1{mm}; // Should be OK also
    // Kilometers km2 = mm; // Should be OK
    // Kilometers km3{/*DOUBLE_MAX*/};
    // Millimeters mm2 = km3; // Should be overflow error 
    // // (I dont think it is possible in compile time, since values may not be known at compile time, so we should use exceptions here or overflow flag)

    // Millimeters mm3{Grams{}}; // Should be error
    // Millimeters::ConvPolicy::fromBase<Millimeters>(Meters::fromValue(123));

    auto mm = Millimeters::fromValue(10);
    
    std::cout << unit_cast<Millimeters, Kilometers>(mm).m_value << std::endl;
}
