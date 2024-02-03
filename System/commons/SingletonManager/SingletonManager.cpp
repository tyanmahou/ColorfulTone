#include <commons/SingletonManager/SingletonManager.hpp>
#include <utils/Singleton/DynamicSingleton.hpp>

namespace ct
{
    SingletonManager::SingletonManager()
    {}

    SingletonManager::~SingletonManager()
    {
        // シングルトンリリース
        DynamicSingletonFinalizer::Finalize();
    }
}
