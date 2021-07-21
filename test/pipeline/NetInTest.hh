#pragma once

#include "gtest/gtest.h"
#include "TestPackets.hh"
#include "hooya/pipeline/NetIn.hh"
#include "hooya/pipeline/FIFO.hh"
#include "hooya/net/NetData.hh"
#include "hooya/sock/DGram.hh"

class NetInTest : public ::testing::Test { };
