//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#pragma once

#include "db/compaction/compaction_picker.h"

namespace ROCKSDB_NAMESPACE {
class StackCompactionPicker : public CompactionPicker {
 public:
  StackCompactionPicker(const ImmutableCFOptions& ioptions,
                        const InternalKeyComparator* icmp)
      : CompactionPicker(ioptions, icmp) {}

  virtual Compaction* PickCompaction(
      const std::string& cf_name, const MutableCFOptions& mutable_cf_options,
      const MutableDBOptions& mutable_db_options, VersionStorageInfo* vstorage,
      LogBuffer* log_buffer,
      SequenceNumber earliest_memtable_seqno = kMaxSequenceNumber) override;

  virtual int MaxOutputLevel() const override { return NumberLevels() - 1; }

  virtual bool NeedsCompaction(
      const VersionStorageInfo* vstorage) const override {
    if (vstorage->CompactionScore(0) >= 1) {
      return true;
    }
    if (!vstorage->FilesMarkedForPeriodicCompaction().empty()) {
      return true;
    }
    if (!vstorage->FilesMarkedForCompaction().empty()) {
      return true;
    }
    return false;
  }

 protected:
  static uint32_t GetPathId(const ImmutableCFOptions& ioptions,
                            const MutableCFOptions& mutable_cf_options,
                            uint64_t file_size) {
    // Two conditions need to be satisfied:
    // (1) the target path needs to be able to hold the file's size
    // (2) Total size left in this and previous paths need to be not
    //     smaller than expected future file size before this new file is
    //     compacted, which is estimated based on size_ratio.
    // For example, if now we are compacting files of size (1, 1, 2, 4, 8),
    // we will make sure the target file, probably with size of 16, will be
    // placed in a path so that eventually when new files are generated and
    // compacted to (1, 1, 2, 4, 8, 16), all those files can be stored in or
    // before the path we chose.
    //
    // TODO(sdong): now the case of multiple column families is not
    // considered in this algorithm. So the target size can be violated in
    // that case. We need to improve it.
    uint64_t accumulated_size = 0;
    uint64_t future_size =
        file_size *
        (100 - mutable_cf_options.compaction_options_universal.size_ratio) /
        100;
    uint32_t p = 0;
    assert(!ioptions.cf_paths.empty());
    for (; p < ioptions.cf_paths.size() - 1; p++) {
      uint64_t target_size = ioptions.cf_paths[p].target_size;
      if (target_size > file_size &&
          accumulated_size + (target_size - file_size) > future_size) {
        return p;
      }
      accumulated_size += target_size;
    }
    return p;
  }
};

}  // namespace ROCKSDB_NAMESPACE
