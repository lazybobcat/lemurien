#ifndef FACTORY_H
#define FACTORY_H

/**
 * @file
 * @brief This file implements a class for a generic Factory
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include <string>

/**
 * @class Factory
 * @brief A generic (reusable) purly abstract class implementing the design pattern Factory.
 * @details You need to inherits from Factory and redefine the create() method.
 *
 *  @code
 *  // A abstract class to define the kind of objects the Factory will produce
 *  class Car
 *  {
 *      Color   mColor;
 *  };
 *
 *  // Declare a concrete Car, here a Renaud Megane
 *  class Megane : public Car
 *  {
 *      public:
 *          Megane(const Color& c) : mColor(c) { }
 *  };
 *
 *  // Our Car Factory, inheriting from Fatory
 *  class CarFactory : public Factory<Car>
 *  {
 *      public:
 *          // Overload of the Factory::create() method
 *          Car* create(const std::string& type)
 *          {
 *              if(type == "megane") // The type wanted is Megane. Let's build a grey Megane !
 *                  return new Megane(Color::MetalGrey);
 *              return NULL;
 *          }
 *  };
 *  @endcode
 * @tparam This class is templated with the abstract class of Product you want to create.
 */
template<class Product>
class Factory
{
public:
    /**
     * @brief create method has to be redifined by children classes
     * @param type This argument determine what kind of Product the class will create
     * @return Return a pointer on the Product
     */
    virtual Product*  create(const std::string& type) = 0;
};

#endif // FACTORY_H
