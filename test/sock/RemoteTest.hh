#pragma once

#include <thread>
#include <unistd.h>
#include "gtest/gtest.h"
#include "hooya/sock/Remote.hh"
#include "hooya/sock/DGram.hh"
#include "hooya/sock/Exception.hh"
#include "TestPackets.hh"

class RemoteTest : public ::testing::Test { };
