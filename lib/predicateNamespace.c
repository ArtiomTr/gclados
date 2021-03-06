// Author: Artiom Tretjakovas
// Description: This file contains bindings for all available predicates. The types are described in
//              "predicateNamespace.h" file.

#include "predicateNamespace.h"

#include "arrayPredicates.h"
#include "boolPredicates.h"
#include "bytePredicates.h"
#include "floatPredicates.h"
#include "notPredicate.h"
#include "numberPredicates.h"
#include "snapshots.h"
#include "stringPredicates.h"

#define NUMBER_PREDICATE_ASSIGNMENTS(name)                                                                             \
    .toEqual##name = gcladosToEqual##name, .toBeLessThan##name = gcladosToBeLessThan##name,                            \
    .toBeGreaterThan##name = gcladosToBeGreaterThan##name,                                                             \
    .toBeLessThanOrEqual##name = gcladosToBeLessThanOrEqual##name,                                                     \
    .toBeGreaterThanOrEqual##name = gcladosToBeGreaterThanOrEqual##name

#define FLOAT_PREDICATE_ASSIGNMENTS(name)                                                                              \
    .toEqual##name = gcladosToEqual##name, .toBeLessThan##name = gcladosToBeLessThan##name,                            \
    .toBeGreaterThan##name = gcladosToBeGreaterThan##name

GcladosPredicateNamespace gclados = {
        NUMBER_PREDICATE_ASSIGNMENTS(Char),
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedChar),
        NUMBER_PREDICATE_ASSIGNMENTS(Short),
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedShort),
        NUMBER_PREDICATE_ASSIGNMENTS(Int),
        NUMBER_PREDICATE_ASSIGNMENTS(Unsigned),
        NUMBER_PREDICATE_ASSIGNMENTS(Long),
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedLong),

        FLOAT_PREDICATE_ASSIGNMENTS(Float),
        FLOAT_PREDICATE_ASSIGNMENTS(Double),
        FLOAT_PREDICATE_ASSIGNMENTS(LongDouble),

        .toEqualString = gcladosToEqualString,
        .toBeTruthy = gcladosToBeTruthy,
        .toBeFalsy = gcladosToBeFalsy,
        .each = gcladosEach,
        .toEqualBytes = gcladosToEqualBytes,
        .toMatchSnapshot = gcladosToMatchSnapshot,
        .not = gcladosNot,
};

#undef NUMBER_PREDICATE_ASSIGNMENTS
