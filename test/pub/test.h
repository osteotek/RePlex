#pragma once

#include <replex.h>

extern "C" {
    int foo(int x);
    extern int bar;
}

std::array<std::pair<const char*, void*>, 2> g_exports = {
    std::make_pair("foo", nullptr),
    std::make_pair("bar", nullptr)
};

class TestModule : public Replexmodule<TestModule, g_exports.size()> {
public:
    static int Foo(int x) {
        return getinstance().execute<0, int>(x);
    }

    static int getbar() {
        return *getinstance().getvar<1, decltype(bar)>();
    }

    TestModule() : Replexmodule(g_exports) {}

protected:
    virtual const char* getpath() const override {
#ifdef DEBUG
        return "bin/Debug/libreplextest.dylib";
#else
        return "bin/Debug/libreplextest.dylib";
#endif
    }
};