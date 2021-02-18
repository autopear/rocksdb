//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/compaction/compaction_picker_bounded_depth.h"

#ifndef ROCKSDB_LITE

namespace ROCKSDB_NAMESPACE {

Compaction* BoundedDepthCompactionPicker::PickCompaction(
    const std::string& cf_name, const MutableCFOptions& mutable_cf_options,
    const MutableDBOptions& mutable_db_options, VersionStorageInfo* vstorage,
    LogBuffer* log_buffer, SequenceNumber earliest_memtable_seqno) {
  std::ignore = cf_name;
  std::ignore = log_buffer;
  std::ignore = earliest_memtable_seqno;
  uint64_t estimated_total_size;
  CompactionStyle style;
  std::vector<CompactionInputFiles> files = getCompactionFiles(
      mutable_cf_options, vstorage, &estimated_total_size, &style);
  if (files.empty()) {
    return nullptr;
  }

  uint32_t path_id =
      GetPathId(ioptions_, mutable_cf_options, estimated_total_size);

  // output files at the bottom most level, unless it's reserved
  int output_level = vstorage->num_levels() - 1;
  // last level is reserved for the files ingested behind
  if (ioptions_.allow_ingest_behind) {
    assert(output_level > 1);
    output_level--;
  }

  return new Compaction(
      vstorage, ioptions_, mutable_cf_options, mutable_db_options,
      std::move(files), output_level,
      MaxFileSizeForLevel(mutable_cf_options, output_level, style), LLONG_MAX,
      path_id,
      GetCompressionType(ioptions_, vstorage, mutable_cf_options, output_level,
                         1, true /* enable_compression */),
      GetCompressionOptions(mutable_cf_options, vstorage, output_level,
                            true /* enable_compression */),
      /* max_subcompactions */ 0, /* grandparents */ {}, /* is manual */ false,
      vstorage->CompactionScore(0), false /* deletion_compaction */,
      CompactionReason::kBoundedDepth);
}

}  // namespace ROCKSDB_NAMESPACE

#endif  // !ROCKSDB_LITE
