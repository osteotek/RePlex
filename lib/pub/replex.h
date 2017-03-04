#pragma once

#include <array>
#include <unordered_map>
#include <string>
#include <dlfcn.h>
#include <cassert>

template <typename E, size_t numsymbols>
class Replexmodule {
public:
    using SymbolArray = std::array<std::pair<const char*, void*>, numsymbols>;

    static void loadlibrary() { getinstance().load(); }
    static void reloadlibrary() { getinstance().reload(); }
    static void unloadlibrary() { getinstance().unload(); }

protected:
    static E& getinstance() {
        static E instance;
        return instance;
    }

    virtual const char* getpath() const = 0;

    Replexmodule(SymbolArray& symbols) : m_symbols(symbols) {}
    
    template <size_t index, typename Ret, typename... Args>
    Ret execute(Args... args) {
        static_assert(index >= 0 && index < numsymbols, "Out of bounds symbol index");
        auto symbol = m_symbols[index];
        return reinterpret_cast<Ret(*)(Args...)>(symbol.second)(args...);
    }

    template <size_t index, typename T>
    T* getvar() {
        static_assert(index >= 0 && index < numsymbols, "Out of bounds symbol index");
        auto symbol = m_symbols[index];
        return static_cast<T*>(symbol.second);
    }

private:
    void load() {
        m_libhandle = dlopen(getpath(), RTLD_NOW);
        assert(m_libhandle);
        loadsymbols();
    }

    void reload() {
        unload();
        load();
    }

    void unload() {
        dlclose(m_libhandle);
        m_libhandle = nullptr;
        unloadsymbols();
    }

    void loadsymbols() {
        for (auto&& symbol : m_symbols) {
            symbol.second = dlsym(m_libhandle, symbol.first);
        }
    }

    void unloadsymbols() {
        for (auto&& symbol : m_symbols) {
            symbol.second = nullptr;
        }
    }

    void* m_libhandle;
    SymbolArray& m_symbols;
};