#include "run.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>

#include "testParser.h"
#include "builder.h"
#include "filenameUtils.h"
#include "panic.h"

typedef struct {
    char** paths;
    size_t pathCount;
    size_t gccArgCount;
    char** gccArgs;
} RunCommandOptions;

int printError(const char* message, int a) {
    return printf("%s (%d)", message, a);
}

RunCommandOptions* parseRunArgs(int argc, char* argv[]) {
    RunCommandOptions* options = malloc(sizeof(RunCommandOptions));

    int doubleHyphenPosition = -1;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--") == 0) {
            doubleHyphenPosition = i;
            break;
        }
    }

    if(doubleHyphenPosition != -1) {
        options->gccArgs = argv + doubleHyphenPosition + 1;
        options->gccArgCount = argc - doubleHyphenPosition - 1;
    } else {
        options->gccArgs = NULL;
        options->gccArgCount = 0;
    }

    if(argc > 0 && (doubleHyphenPosition == -1 || argc - doubleHyphenPosition > 0)) {
        glob_t globBuffer;

        globBuffer.gl_pathc = 0;
        globBuffer.gl_pathv = NULL;
        globBuffer.gl_offs = 0;

        int queryCount = doubleHyphenPosition == -1 ? argc : doubleHyphenPosition;
        for(int i = 0; i < queryCount; i++) {
            if(glob(argv[i], GLOB_APPEND, printError, &globBuffer) != 0) {
                // TODO: add more detailed message here
                printf("Error!\n");
                return NULL;
            }
        }

        options->pathCount = globBuffer.gl_pathc;
        options->paths = globBuffer.gl_pathv;
    } else {
        options->pathCount = 0;
        options->paths = NULL;
    }

    return options;
}

char* compileTestEntry(char* entryFilePath, RunCommandOptions options) {
    char commandBuffer[1024];

    size_t offset = sprintf(commandBuffer, "gcc %s", entryFilePath);

    for(int i = 0; i < options.pathCount; i++) {
        offset += sprintf(commandBuffer + offset, " %s", options.paths[i]);
    }

    for(int i = 0; i < options.gccArgCount; ++i) {
        offset += sprintf(commandBuffer + offset, " %s", options.gccArgs[i]);
    }

    char* outputFile = tmpNameExtended("");

    if(outputFile == NULL) {
        gcladosPanic("Could not create temporary file for test executable.");
    }

    sprintf(commandBuffer + offset, " -o %s", outputFile);

    printf("Executing command \"%s\"...\n", commandBuffer);
    fflush(stdout);

    system(commandBuffer);

    return outputFile;
}

int executeRun(RunCommandOptions* options) {
    if(options->pathCount > 0) {
        struct ParsedTestFile parsedFiles[options->pathCount];

        for(size_t i = 0; i < options->pathCount; ++i) {
            parsedFiles[i] = parseTestFile(options->paths[i]);
        }
        char* testFile = buildTestFile(parsedFiles, options->pathCount);

        char* compiled = compileTestEntry(testFile, *options);
        fflush(stdout);

        system(compiled);

        free(testFile);
        free(compiled);
    } else {
        printf("No tests found.\n");
    }

    return 0;
}

Command createRunCommand() {
    Command runCommand = {
            "run",
            "Command, which runs specified tests",
            (void* (*)(int, char*[])) parseRunArgs,
            (int (*)(void*)) executeRun,
    };

    return runCommand;
}
