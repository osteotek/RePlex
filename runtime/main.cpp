#include <replex.h>
#include <test.h>
#include <cstdlib>
#include <fstream>
#include <gtest/gtest.h>

const char* g_test_v1 = 
    "#include \"pub/test.h\"\n"
    "int bar = 3;\n"
    "int foo(int x)\n"
    "{\n"
    "   return x + 5;\n"
    "}";

const char* g_test_v2 = 
    "#include \"pub/test.h\"\n"
    "int bar = -2;\n"
    "int foo(int x)\n"
    "{\n"
    "   return x - 5;\n"
    "}";
    
class replextest : public ::testing::Test {
public:
    virtual void SetUp() {
        writefile("test/test.cpp", g_test_v1);
        compile(1);
        TestModule::loadlibrary();
    }

    void changeandreload() {
        writefile("test/test.cpp", g_test_v2);
        compile(2);
        TestModule::reloadlibrary();
    }

    virtual void TearDown() {
        TestModule::unloadlibrary();
        writefile("test/test.cpp", g_test_v1);
        compile(1);
    }

private:
    void writefile(const char* path, const char* text) {
        std::ofstream out(path, std::ios_base::trunc | std::ios_base::out);
        out << text;
    }

    void compile(int version) {
        if (version == m_version) {
            return;
        }

        m_version = version;
        EXPECT_EQ(std::system("make"), 0);

        sleep(1);
    }

    int m_version = 1;
};

TEST_F(replextest, VariableReload) {
    EXPECT_EQ(TestModule::getbar(), 3);
    changeandreload();
    EXPECT_EQ(TestModule::getbar(), -2);
}

TEST_F(replextest, FunctionReload) {
    EXPECT_EQ(TestModule::Foo(4), 9);
    changeandreload();
    EXPECT_EQ(TestModule::Foo(4), -1);
}
    
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}