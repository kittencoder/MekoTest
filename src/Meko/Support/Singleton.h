#ifndef SUPPORT_SINGLETON_H
#define SUPPORT_SINGLETON_H

#include "../Meko.h"

namespace Meko::Support {

/**
 * @see https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 */
template <class ClassToSingleton>
class Singleton {
   public:
    static ClassToSingleton& getInstance();

    Singleton();  // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    // Singleton(Singleton const&);       // Don't Implement
    // void operator=(Singleton const&);  // Don't implement

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&) = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
};

// shorthand definer for template-class
#define CLSDEF(type) template <class ClassToSingleton> \
type Singleton<ClassToSingleton>

CLSDEF(ClassToSingleton&)::getInstance() {
    static ClassToSingleton instance;  // Guaranteed to be destroyed.
                                       // Instantiated on first use.
    return instance;
}

CLSDEF()::Singleton(){};
#undef CLSDEF

}  // namespace Meko::Support

#endif