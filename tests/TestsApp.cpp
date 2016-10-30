
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

class TestsApp {
public:
    TestsApp() {
        
    }

    ~TestsApp() {
        
    }

    int Run(int argc, char **argv) {
        return Catch::Session().run(argc, argv);
    }
};

#undef main
int main(int argc, char **argv) {
    return TestsApp().Run(argc, argv);
}
