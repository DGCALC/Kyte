//
// Created by Lucius Traylor Schoenbaum on 8/14/20.
//

#pragma once

// Edit this list by adding arguments
// like argv for command line control of Catch2.
/*
 * Example:
 * $ foo -?
 * Corresponds to
 * const char * catch_argv[]{"path_of_executable", "-?", nullptr};
 *
 */
const char * catch_argv[]{
        "path_of_executable",
        "-?",
        nullptr
};

#include <catch2/catch.hpp>

#include <Tconvert.h>
#include <Tbytearray.h>
#include <Tfield.h>

