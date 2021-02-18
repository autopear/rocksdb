// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#pragma once

namespace ROCKSDB_NAMESPACE {

class CompactionOptionsBoundedDepth {
 public:
  unsigned int k;

  // Default set of parameters
  CompactionOptionsBoundedDepth() : k(8) {}
};

class CompactionOptionsBigtable : public CompactionOptionsBoundedDepth {
 public:
  // Default set of parameters
  CompactionOptionsBigtable() : CompactionOptionsBoundedDepth() {}
};

class CompactionOptionsBinomial : public CompactionOptionsBoundedDepth {
 public:
  // Default set of parameters
  CompactionOptionsBinomial() : CompactionOptionsBoundedDepth() {}
};

class CompactionOptionsConstant : public CompactionOptionsBoundedDepth {
 public:
  // Default set of parameters
  CompactionOptionsConstant() : CompactionOptionsBoundedDepth() {}
};

class CompactionOptionsMinLatency : public CompactionOptionsBoundedDepth {
 public:
  // Default set of parameters
  CompactionOptionsMinLatency() : CompactionOptionsBoundedDepth() {}
};

}  // namespace ROCKSDB_NAMESPACE
