#include <ratio>
#include <iostream>

#include "cppunit.hpp"

using namespace CppUnit;
template <typename T>
struct Grams : public BaseUnit<T, Grams>{};

template <typename T>
struct Meters : public BaseUnit<T, Meters> {};

template <typename T>
struct Millimeters : public Unit<T, Millimeters, Meters, RatioConvPolicy<std::ratio<1, 1000>>> {};

template <typename T>
struct Kilometers : public Unit<T, Kilometers, Meters, RatioConvPolicy<std::ratio<1000, 1>>> {};

template <typename T>
struct Megameters : public Unit<T, Megameters, Kilometers, RatioConvPolicy<std::ratio<1000, 1>>> {};

template <typename T>
struct Gigameters : public Unit<T, Gigameters, Megameters, RatioConvPolicy<std::ratio<1000, 1>>> {};



int main()
{
    Gigameters<int> Gm{int(1)};
    std::cout << unit_cast<Gigameters<int>, Millimeters<double>>(Gm) << std::endl;

    Millimeters<double> mm{double(10)};
    Kilometers<double> km1{mm};
    std::cout << "Init cast " << km1 << std::endl;

    Millimeters mm2 = mm;
    std::cout << "Init copy via assignment " << mm2 << std::endl; 

    Kilometers<double> km2{Millimeters<double>{}};

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
