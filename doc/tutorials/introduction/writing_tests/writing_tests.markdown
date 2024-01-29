OpenCV configuration options reference {#tutorial_writing_tests}
======================================

@prev_tutorial{tutorial_documentation}
@next_tutorial{tutorial_transition_guide}

@tableofcontents

# Introduction {#tutorial_writing_tests_intro}

The OpenCV library

#

Short reminder: How to build and run the tests (BUILD_TESTS option enabled by default, build all or opencv_test_xxx target (❓ is there opencv_tests target?), set testdata location in the environment, run the test, filter test cases)

File structure (mostly covered)

Several words about ts module (internal, static, includes gtest, requires core, imgproc, imgcodecs, videoio, highgui)

General guidelines: kinds of tests, synthetic data as input, gold output, array comparison, numerical instability, image licensing (❓ too general themes? out of scope? separate article?)

Namespaces (cvtest, anonymous) and common include-files

Test body: simple tests, fixture tests, parametrized tests (code snippets, some information would intersect with gtest docs, but it's ok, link to gtest for more details)

Assert and Expect macros (only most common, would intersect with gtest docs, link for more details)

OpenCV-specific things: utility headers (ts.hpp, ts_ext.hpp, ocl_test.hpp, cuda_test.hpp), functions from cvtest namespace should be used for verification; random input generation; array comparison with cvtest::norm, cvtest::cmpEps*; additional macros from ocl_test.hpp and cuda_test.hpp; how and when to print to stdout; existing fixtures ArrayTest, BadArgTest; using testdata with findDataFile; runtime test skipping; test tagging (❓ too advanced? separate section?),

OpenCL test specifics(❓)

Testdata generation (❓ too advanced?)

Note: maybe some things (e.g. in cvtest namespace) can be documented in respective headers and enabled in the documentation. After that they can be referred from the tutorial. However this can require a separate activity...
