#pragma once
#include <SFML/System/Vector2.hpp>

#include <math.h>

namespace sfh
{

    ////////
    ///  Magnitude
    ////////
    template <typename T>
    T GetLength(const sf::Vector2<T>& vector);

    ////////
    /// Right Handed scalar multiplication
    ////////
    template <typename T>
    sf::Vector2<T> ScalarProduct(const sf::Vector2<T>& vector, T scalar);

    ////////
    /// Left Handed scalar multiplication
    ////////
    template <typename T>
    sf::Vector2<T> ScalarProduct(T scalar, const sf::Vector2<T>& vector);

    ////////
    ///  scalar multiplication by componet
    ////////
    template <typename T>
    sf::Vector2<T> ScalarProduct(const sf::Vector2<T>& left, const sf::Vector2<T>& right);
}

#include "MathHelper.inl"

