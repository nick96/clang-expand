//===----------------------------------------------------------------------===//
//
//                           The MIT License (MIT)
//                    Copyright (c) 2017 Peter Goldsborough
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//===----------------------------------------------------------------------===//

#ifndef CLANG_EXPAND_SYMBOL_SEARCH_MATCH_HANDLER_HPP
#define CLANG_EXPAND_SYMBOL_SEARCH_MATCH_HANDLER_HPP

// Clang includes
#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace clang {
class SourceLocation;
}

namespace ClangExpand {
struct Query;
}

namespace ClangExpand {
namespace SymbolSearch {

/// \ingroup SymbolSearch
///
/// Handles candidate functions in the source.
///
/// This class does the heaviest lifting of any components inside clang-expand
/// as it has to handle all aspects of:
/// 1. Checking for a function call expression that matches the ASTMatchers
/// expression from the `SymbolSearch::Consumer` (i.e. with the correct name) if
/// that call location is at the right place (under the user's cursor).
/// 2. Verifying if it is safe to expand the function call wherever it is in the
/// source. For example, it is not safe to expand a function call inside another
/// function call.
/// 3. Collecting `CallData`, including the source extent of the call and
/// assignee of any return value.
/// 4. Collecting `DeclarationData`, i.e. as much information as possible to
/// serialize our knowledge for later use in the definition search phase.
/// 5. If the declaration is in fact also a definition, collecting
/// `DefinitionData`.
class MatchHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  using MatchResult = clang::ast_matchers::MatchFinder::MatchResult;

  /// Constructor.
  explicit MatchHandler(const clang::SourceLocation& targetLocation,
                        Query& query);

  /// Runs the `MatchHandler` for a matching expression.
  void run(const MatchResult& result) override;

 private:
  /// The target location of the function call.
  const clang::SourceLocation& _targetLocation;

  /// The ongoing `Query` object.
  Query& _query;
};

}  // namespace SymbolSearch
}  // namespace ClangExpand

#endif  // CLANG_EXPAND_SYMBOL_SEARCH_MATCH_HANDLER_HPP
