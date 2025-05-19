#pragma once

#include "PlayerStrategy.hpp"
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

namespace sevens {

class StrategyLoader {
public:
    StrategyLoader(const std::string& lib_path) {
#ifdef _WIN32
        handle = LoadLibraryA(lib_path.c_str());
        if (!handle) {
            throw std::runtime_error("Failed to load library: " + lib_path);
        }
#else
        handle = dlopen(lib_path.c_str(), RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error("Failed to load library: " + lib_path);
        }
#endif
    }

    ~StrategyLoader() {
#ifdef _WIN32
        if (handle) {
            FreeLibrary(static_cast<HMODULE>(handle));
        }
#else
        if (handle) {
            dlclose(handle);
        }
#endif
    }

    std::shared_ptr<PlayerStrategy> createStrategy() {
#ifdef _WIN32
        // Définition du type de fonction attendue
        using CreateStrategyFunc = PlayerStrategy* (*)();
        
        // On récupère l'adresse sous forme de pointeur générique
        FARPROC genericFunc = GetProcAddress(static_cast<HMODULE>(handle), "createStrategy");
        if (!genericFunc) {
            throw std::runtime_error("Failed to load createStrategy function");
        }

        // On cast vers le bon type de pointeur de fonction
        auto createFunc = reinterpret_cast<CreateStrategyFunc>(genericFunc);
#else
        auto createFunc = reinterpret_cast<PlayerStrategy*(*)()>(
            dlsym(handle, "createStrategy")
        );
#endif
        if (!createFunc) {
            throw std::runtime_error("Failed to load createStrategy function");
        }
        return std::shared_ptr<PlayerStrategy>(createFunc());
    }

private:
    void* handle;
};

} // namespace sevens
