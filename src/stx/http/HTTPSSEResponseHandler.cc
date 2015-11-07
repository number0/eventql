/**
 * This file is part of the "libstx" project
 *   Copyright (c) 2015 Paul Asmuth, FnordCorp B.V.
 *
 * libstx is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "stx/http/HTTPSSEResponseHandler.h"

namespace stx {
namespace http {

HTTPSSEResponseHandler::FactoryFn HTTPSSEResponseHandler::getFactory() {
  return [] (
      const Promise<http::HTTPResponse> promise) -> HTTPResponseFuture* {
    return new HTTPSSEResponseHandler(promise);
  };
}

}
}