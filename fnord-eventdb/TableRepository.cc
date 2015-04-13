/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <fnord-eventdb/TableRepository.h>

namespace fnord {
namespace eventdb {

void TableRepository::addTable(RefPtr<Table> table) {
  std::unique_lock<std::mutex> lk(mutex_);
  tables_.emplace(table->name(), table);
}

RefPtr<Table> TableRepository::findTable(const String& name) const {
  std::unique_lock<std::mutex> lk(mutex_);
  auto table = tables_.find(name);
  if (table == tables_.end()) {
    RAISEF(kIndexError, "unknown table: '$0'", name);
  }

  return table->second;
}

Vector<RefPtr<Table>> TableRepository::tables() const {
  Vector<RefPtr<Table>> tables;

  std::unique_lock<std::mutex> lk(mutex_);
  for (auto& t : tables_) {
    tables.emplace_back(t.second);
  }

  return tables;
}

} // namespace eventdb
} // namespace fnord

