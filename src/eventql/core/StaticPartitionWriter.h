/**
 * This file is part of the "tsdb" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <eventql/util/stdtypes.h>
#include <eventql/util/autoref.h>
#include <eventql/core/PartitionWriter.h>
#include <eventql/util/util/PersistentHashSet.h>

using namespace stx;

namespace zbase {

class StaticPartitionWriter : public PartitionWriter {
public:

  StaticPartitionWriter(PartitionSnapshotRef* head);

  Set<SHA1Hash> insertRecords(
      const Vector<RecordRef>& records) override;

  bool needsCompaction() override;

  bool commit() override;
  bool compact() override;

};

} // namespace tdsb
