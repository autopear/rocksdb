//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "db/compaction/compaction_picker_constant.h"

#ifndef ROCKSDB_LITE

namespace ROCKSDB_NAMESPACE {

std::vector<CompactionInputFiles> ConstantCompactionPicker::getCompactionFiles(
    const MutableCFOptions& mutable_cf_options, VersionStorageInfo* vstorage,
    uint64_t* estimated_total_size, CompactionStyle* style) {
  uint32_t cnt = 0;
  std::vector<CompactionInputFiles> files(vstorage->num_levels());
  uint64_t esize = 0;
  for (size_t i = 0; i < files.size(); ++i) {
    files[i].level = static_cast<int>(i);
    for (FileMetaData* fm : vstorage->LevelFiles(static_cast<int>(i))) {
      if (fm->being_compacted) {
        *estimated_total_size = 0;
        *style = kCompactionStyleConstant;
        return std::vector<CompactionInputFiles>();
      }
      esize += fm->fd.GetFileSize();
      files[i].files.push_back(fm);
      cnt++;
    }
  }
  if (cnt <= mutable_cf_options.compaction_options_constant.k) {
    *estimated_total_size = 0;
    *style = kCompactionStyleConstant;
    return std::vector<CompactionInputFiles>();
  }
  *style = kCompactionStyleConstant;
  return files;
}

}  // namespace ROCKSDB_NAMESPACE

#endif  // !ROCKSDB_LITE
