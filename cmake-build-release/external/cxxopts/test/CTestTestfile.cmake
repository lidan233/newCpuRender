# CMake generated Testfile for 
# Source directory: /Users/lidan/CLionProjects/cpurender/external/cxxopts/test
# Build directory: /Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(options "options_test")
set_tests_properties(options PROPERTIES  _BACKTRACE_TRIPLES "/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;4;add_test;/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;0;")
add_test(find-package-test "/Applications/CLion.app/Contents/bin/cmake/mac/bin/ctest" "-C" "Release" "--build-and-test" "/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/find-package-test" "/Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test/find-package-test" "--build-generator" "Unix Makefiles" "--build-makeprogram" "/usr/bin/make" "--build-options" "-DCMAKE_CXX_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++" "-DCMAKE_BUILD_TYPE=Release" "-Dcxxopts_DIR=/Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts")
set_tests_properties(find-package-test PROPERTIES  _BACKTRACE_TRIPLES "/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;7;add_test;/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;0;")
add_test(add-subdirectory-test "/Applications/CLion.app/Contents/bin/cmake/mac/bin/ctest" "-C" "Release" "--build-and-test" "/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/add-subdirectory-test" "/Users/lidan/CLionProjects/cpurender/cmake-build-release/external/cxxopts/test/add-subdirectory-test" "--build-generator" "Unix Makefiles" "--build-makeprogram" "/usr/bin/make" "--build-options" "-DCMAKE_CXX_COMPILER=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++" "-DCMAKE_BUILD_TYPE=Release")
set_tests_properties(add-subdirectory-test PROPERTIES  _BACKTRACE_TRIPLES "/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;21;add_test;/Users/lidan/CLionProjects/cpurender/external/cxxopts/test/CMakeLists.txt;0;")
