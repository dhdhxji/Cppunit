#include <ratio>
#include <iostream>

#include "cppunit.hpp"

using namespace CppUnit;

// TODO: typename vs class


// Check for c++ standard here
// template <typename TUnit>
// using BaseUnit_t = typename BaseUnit<TUnit>::type;



// --------- Units Here -----------------
struct Meters : public BaseUnit<double, Meters> {};

struct Kilometers : public Unit<double, Kilometers, Meters, RatioConvPolicy<std::ratio<1000, 1>>> {};
struct Millimeters : public Unit<double, Millimeters, Meters, RatioConvPolicy<std::ratio<1, 1000>>> {};


// TODO: Automatic SI unit conversion (struct Megameters : public SIMegaUnit<double, Meters, Megameters>)

struct MegametersConv {
    template <typename TUnit>
    struct Policy {
        using BaseType_t = typename TUnit::BaseType;

        static BaseType_t toBase(const TUnit &Mm) {return BaseType_t::fromValue(Mm.m_value * 1000);}
        static TUnit fromBase(const BaseType_t &km) {return TUnit::fromValue(km.m_value / 1000);}
    };
};
struct Megameters : public Unit<double, Megameters, Kilometers, MegametersConv> {};



struct Grams : public BaseUnit<double, Grams>{};



int main()
{
    Millimeters mm{double(10)};
    Kilometers km1{mm};
    std::cout << "Init cast " << km1 << std::endl;

    Millimeters mm2 = mm;
    std::cout << "Init copy via assignment " << mm2 << std::endl; 

    Kilometers km2{Millimeters{}};

    // Kilometers km2 = mm;
    // std::cout << "Assignment init " << km2 << std::endl;

    // Kilometers km3{/*DOUBLE_MAX*/};
    // Millimeters mm2 = km3; // Should be overflow error 
    // // (I dont think it is possible in compile time, since values may not be known at compile time, so we should use exceptions here or overflow flag)

    // Millimeters mm3{Grams{}}; // Should be error
    // auto value = 11_km //(or value = 11km)
    // composite units (velocity = km / h)

    // auto mm = Millimeters{10};

    // std::cout << unit_cast<Millimeters, Kilometers>(mm) << std::endl;
}
