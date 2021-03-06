// Author: Artiom Tretjakovas
// Description: This file contains implementations of test suite manipulation functions. Function prototypes are
//              described in "testSuite.h" file.

#include "testSuite.h"

#include <stdio.h>

#include "colors.h"
#include "ioutils.h"
#include "panic.h"

GcladosTestSuiteAccumulatedResult gcladosInitializeTestSuiteResult(GcladosTestSuite suite) {
    GcladosTestSuiteAccumulatedResult result = {
            .completedTestCount = 0,
            .status = GCLADOS_WAITING,
            .testResultCount = suite.testCount,
            .testResults = calloc(suite.testCount, sizeof(GcladosTestResult)),
    };

    return result;
}

void gcladosCompleteTestSuiteResult(GcladosTestSuiteAccumulatedResult *result) {
    bool pass = true;
    for(size_t i = 0; i < result->testResultCount; ++i) {
        if(result->testResults[i].pass == false) {
            pass = false;
            break;
        }
    }

    result->status = pass ? GCLADOS_PASS : GCLADOS_FAILED;
}

GcladosTestSuite gcladosCreateTestSuite(const char *testSuiteName, GcladosTest *tests, size_t testCount) {
    GcladosTestSuite suite = {
            .testSuiteName = testSuiteName,
            .tests = tests,
            .testCount = testCount,
    };

    return suite;
}

bool gcladosRunNextTest(GcladosTestSuite suite, GcladosTestSuiteAccumulatedResult *suiteAccumulatedResult) {
    if(suiteAccumulatedResult->completedTestCount >= suite.testCount) {
        gcladosPanic("Trying to run more tests than actually exist.", EXIT_FAILURE);
    }

    size_t currentTestIndex = suiteAccumulatedResult->completedTestCount;

    suiteAccumulatedResult->testResults[currentTestIndex] = gcladosRunTest(suite.tests[currentTestIndex]);

    suiteAccumulatedResult->completedTestCount += 1;

    return suiteAccumulatedResult->completedTestCount == suite.testCount;
}

void gcladosPrintSuite(GcladosTestSuite suite, GcladosTestSuiteAccumulatedResult state) {
    GcladosAnsiFlags flags;
    char *text;

    switch(state.status) {
        case GCLADOS_WAITING:
            text = " WAIT ";
            flags = gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_YELLOW), gcladosColors.framed());
            break;
        case GCLADOS_RUNNING:
            text = " RUNS ";
            flags = gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_CYAN), gcladosColors.framed());
            break;
        case GCLADOS_FAILED:
            text = " FAIL ";
            flags = gcladosColors.createFlags(2,
                                              gcladosColors.foregroundColor(GCLADOS_BLACK),
                                              gcladosColors.backgroundColor(GCLADOS_RED));
            break;
        case GCLADOS_PASS:
            text = " PASS ";
            flags = gcladosColors.createFlags(1, gcladosColors.backgroundColor(GCLADOS_GREEN));
            break;
        case GCLADOS_SKIP:
            text = " SKIP ";
            flags = gcladosColors.createFlags(1, gcladosColors.backgroundColor(GCLADOS_YELLOW));
            break;
    }

    char *status = gcladosColors.applyFlags(text, flags);

    printf("%s %s ", status, suite.testSuiteName);

    free(status);
    gcladosColors.freeFlags(flags);

    if(state.status == GCLADOS_RUNNING) {
        gcladosPrintProgress(stdout, (double) (state.completedTestCount + 1) / (double) suite.testCount, 10);
    }

    printf("\n");
}

void gcladosFreeTestSuiteResult(GcladosTestSuiteAccumulatedResult testSuiteResult) {
    free(testSuiteResult.testResults);
}
