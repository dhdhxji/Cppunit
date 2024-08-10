#include <ratio>
#include <iostream>

#include "cppunit.hpp"

using namespace CppUnit;
struct Grams : public BaseUnit<double, Grams>{};

struct Meters : public BaseUnit<double, Meters> {};

struct Millimeters : public Unit<double, Millimeters, Meters, RatioConvPolicy<std::ratio<1, 1000>>> {};
struct Kilometers : public Unit<double, Kilometers, Meters, RatioConvPolicy<std::ratio<1000, 1>>> {};
struct Megameters : public Unit<double, Megameters, Kilometers, RatioConvPolicy<std::ratio<1000, 1>>> {};
struct Gigameters : public Unit<double, Gigameters, Megameters, RatioConvPolicy<std::ratio<1000, 1>>> {};



int main()
{
    Gigameters Gm{double(1)};
    std::cout << unit_cast<Gigameters, Millimeters>(Gm) << std::endl;

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
