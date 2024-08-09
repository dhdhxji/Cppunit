#include <type_traits>
#include <functional>
#include <ratio>
#include <iostream>

template<typename T,
         typename TBaseUnit,
         typename TUnit,
         typename TConvPolicy>
struct Unit
{
    using BaseType = TBaseUnit;
    using UnitType = TUnit;
    using ConvPolicy = TConvPolicy;

    T m_value;
    static TUnit fromValue(const T& value) 
    {
        TUnit unit;
        unit.m_value = value;

        return unit;
    }
};

template <typename T>
struct ItselfConvPolicy
{
    static T toBase(const T &v) {return v;}
    static T fromBase(const T &v) {return v;}
};

// Remove Policy from name
// std::ratio??
template <typename TUnit, typename TRatio>
struct RatioConvPolicy
{
    // Unit = Base * numerator / denominator

    using BaseType_t = typename TUnit::BaseType;

    // 1km = 1000m, 1m
    //
    static BaseType_t toBase(const TUnit &unit) { return BaseType_t::fromValue(unit.m_value / TRatio::den * TRatio::num); }
    static TUnit fromBase(const BaseType_t &base) { return TUnit::fromValue(base.m_value / TRatio::num * TRatio::den); }
};

template <typename T, typename TUnit>
using BaseUnit = Unit<T, TUnit, TUnit, ItselfConvPolicy<TUnit>>;

// TODO is_compatible<Uni1, Unit2>::value

template <typename TFrom, typename TTo>
TTo unit_cast(const TFrom &unit)
{
    using BaseType_t = typename TFrom::BaseType;

    const BaseType_t baseUnit = TFrom::ConvPolicy::toBase(unit);
    const TTo result = TTo::ConvPolicy::fromBase(baseUnit);

    return result;
}

template <typename T>
T unit_cast(const T &unit) {return unit;}



struct Meters : public BaseUnit<double, Meters> {};

// TODO Get rid of redundand template parameters
struct Kilometers : public Unit<double, Meters, Kilometers, RatioConvPolicy<Kilometers, std::ratio<1000, 1>>> {};
struct Millimeters : public Unit<double, Meters, Millimeters, RatioConvPolicy<Millimeters, std::ratio<1, 1000>>> {};


// TODO: Automatic SI unit conversion (struct Megameters : public SIMegaUnit<double, Meters, Megameters>)

template <typename TUnit>
struct MegametersConv {
    using BaseType_t = typename TUnit::BaseType;

    static BaseType_t toBase(const TUnit &Mm) {return BaseType_t::fromValue(Mm.m_value * 1000);}
    static TUnit fromBase(const BaseType_t &km) {return TUnit::fromValue(km.m_value / 1000);}
};
struct Megameters : public Unit<double, Kilometers, Megameters, MegametersConv<Megameters>> {};



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


    auto mm = Millimeters::fromValue(10);
    
    std::cout << unit_cast<Millimeters, Kilometers>(mm).m_value << std::endl;
}
