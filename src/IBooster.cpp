#include "IBooster.hpp"

IBooster::IBooster()
{}

EObjectType IBooster::getType() const
{
    return _objectType;
}