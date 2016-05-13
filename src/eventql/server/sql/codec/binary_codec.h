/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <eventql/util/uri.h>
#include <eventql/util/io/file.h>
#include <eventql/util/autoref.h>
#include <eventql/util/http/httpmessage.h>
#include "eventql/util/http/httprequest.h"
#include "eventql/util/http/httpresponse.h"
#include "eventql/util/http/httpstats.h"
#include "eventql/util/http/httpconnectionpool.h"
#include "eventql/util/http/httpclient.h"
#include "eventql/sql/svalue.h"
#include "eventql/sql/runtime/QueryPlan.h"
#include "eventql/sql/runtime/ExecutionContext.h"

using namespace stx;

namespace csql {


/**
 * Binary SQL Query Result/Response Format
 *
 *   <response> :=
 *       <header>
 *       <event>*
 *       <footer>
 *
 *   <header> :=
 *       %0x01                      // version
 *
 *   <event> :=
 *        <table_header_event> / <row_event> / <progress_even>
 *
 *   <table_header_event>
 *       %0xf1
 *       <lenenc_int>               // number_of_columns
 *       <lenenc_string>*           // column names
 *
 *   <row_event> :=
 *       %0xf2
 *       <lenenc_int>               // number of fields
 *       <svalue>*                  // svalues
 *
 *   <progress_event> :=
 *       %0xf3
 *       <ieee754>                  // progress percent
 *
 *   <error_event> :=
 *       %0xf4
 *       <lenenc_str>               // error message
 *
 *   <footer>
 *       %0xff
 *
 */

class BinaryResultParser : public stx::RefCounted {
public:

  BinaryResultParser();

  void onTableHeader(stx::Function<void (const Vector<String>& columns)> fn);
  void onRow(stx::Function<void (int argc, const SValue* argv)> fn);
  void onProgress(stx::Function<void (const ExecutionStatus& status)> fn);
  void onError(stx::Function<void (const String& error)> fn);

  void parse(const char* data, size_t size);

  bool eof() const;

protected:

  size_t parseTableHeader(const void* data, size_t size);
  size_t parseRow(const void* data, size_t size);
  size_t parseProgress(const void* data, size_t size);
  size_t parseError(const void* data, size_t size);

  stx::Buffer buf_;
  stx::Function<void (const Vector<String>& columns)> on_table_header_;
  stx::Function<void (int argc, const SValue* argv)> on_row_;
  stx::Function<void (const ExecutionStatus& status)> on_progress_;
  stx::Function<void (const String& error)> on_error_;

  bool got_header_;
  bool got_footer_;
};

class BinaryResultFormat  {
public:
  typedef Function<void (void* data, size_t size)> WriteCallback;

  BinaryResultFormat(WriteCallback write_cb);
  ~BinaryResultFormat();

  //void formatResults(
  //    QueryPlan* query,
  //    ExecutionContext* context) override;

  void sendResults(QueryPlan* query);
  void sendError(const String& error_msg);

protected:

  void sendTable(QueryPlan* query, size_t stmt_idx);
  void sendProgress(double progress);

  void sendHeader();
  void sendFooter();

  WriteCallback write_cb_;
};


}
