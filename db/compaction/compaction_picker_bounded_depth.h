//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#pragma once
#ifndef ROCKSDB_LITE

#include <vector>

#include "db/compaction/compaction_picker_stack.h"

namespace ROCKSDB_NAMESPACE {

class BoundedDepthCompactionPicker : public StackCompactionPicker {
 public:
  BoundedDepthCompactionPicker(const ImmutableCFOptions& ioptions,
                               const InternalKeyComparator* icmp)
      : StackCompactionPicker(ioptions, icmp) {}

  virtual Compaction* PickCompaction(
      const std::string& cf_name, const MutableCFOptions& mutable_cf_options,
      const MutableDBOptions& mutable_db_options, VersionStorageInfo* vstorage,
      LogBuffer* log_buffer,
      SequenceNumber earliest_memtable_seqno = kMaxSequenceNumber) override;

 protected:
  virtual std::vector<CompactionInputFiles> getCompactionFiles(
      const MutableCFOptions& mutable_cf_options, VersionStorageInfo* vstorage,
      uint64_t* estimated_total_size, CompactionStyle* style) = 0;
};

}  // namespace ROCKSDB_NAMESPACE

#endif  // !ROCKSDB_LITE
