#include "MathHelper.hpp"

namespace sfh
{
    template<typename T>
    T GetLength(const sf::Vector2<T>& vector)
    {
        return sqrt((vector.x * vector.x) + (vector.y * vector.y));
    }

    template<typename T>
    sf::Vector2<T> ScalarProduct(const sf::Vector2<T>& vector, T scalar)
    {

        return sf::Vector2<T>(vector.x * scalar, vector.y * scalar);
    }

    template<typename T>
    sf::Vector2<T> ScalarProduct(T scalar, const sf::Vector2<T>& vector)
    {
        return sf::Vector2<T>(vector.x * scalar, vector.y * scalar);
    }

    template<typename T>
    sf::Vector2<T> ScalarProduct(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
    {
        return sf::Vector2<T>(left.x * right.x, left.y * right.y);
    }



}