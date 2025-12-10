// Author: Artiom Tretjakovas
// Description: This file contains implementations of string predicates. Function prototypes are described in
//              "stringPredicates.h" file.

#include "stringPredicates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *gcladosToEqualStringExpected(const char **value, const char *options, bool pass) {
    char *output = calloc(strlen(options) + 5, sizeof(char));

    sprintf(output, pass ? "not %s" : "%s", options);

    return output;
}

char *gcladosToEqualStringReceived(const char **value, const char *options, bool pass) {
    if(pass) {
        return NULL;
    } else {
        char *copy = calloc(strlen(*value), sizeof(char));
        strcpy(copy, *value);

        return copy;
    }
}

bool gcladosToEqualStringPredicate(StatementContext context, const char **value, const char *options) {
    return strcmp(*value, options) == 0;
}

GcladosPredicate gcladosToEqualString(char *string) {
    char *options = calloc(strlen(string), sizeof(char));

    strcpy(options, string);

    GcladosPredicate predicate = {
            .options = options,
            .usage = "gclados.toEqualString(%s)",
            .customOutput = false,
            .free = NULL,
            .execute = (bool(*)(StatementContext, void *, void *)) gcladosToEqualStringPredicate,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosToEqualStringExpected,
            .receivedValueToString = (GcladosValueToStringConverter) gcladosToEqualStringReceived,
    };

    return predicate;
}
