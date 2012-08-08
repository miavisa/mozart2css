// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __MODRECORD_H
#define __MODRECORD_H

#include "../mozartcore.hh"

#include "modcompilersupport.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

namespace builtins {

///////////////////
// Record module //
///////////////////

class ModRecord: public Module {
public:
  ModRecord(): Module("Record") {}

  class Is: public Builtin<Is> {
  public:
    Is(): Builtin("is") {}

    OpResult operator()(VM vm, In value, Out result) {
      bool boolResult = false;
      MOZART_CHECK_OPRESULT(RecordLike(value).isRecord(vm, boolResult));

      result = Boolean::build(vm, boolResult);
      return OpResult::proceed();
    }
  };

  class Label: public Builtin<Label> {
  public:
    Label(): Builtin("label") {}

    OpResult operator()(VM vm, In record, Out result) {
      return RecordLike(record).label(vm, result);
    }
  };

  class Width: public Builtin<Width> {
  public:
    Width(): Builtin("width") {}

    OpResult operator()(VM vm, In record, Out result) {
      size_t intResult = 0;
      MOZART_CHECK_OPRESULT(RecordLike(record).width(vm, intResult));

      result = build(vm, intResult);
      return OpResult::proceed();
    }
  };

  class Arity: public Builtin<Arity> {
  public:
    Arity(): Builtin("arity") {}

    OpResult operator()(VM vm, In record, Out result) {
      return RecordLike(record).arityList(vm, result);
    }
  };

  class Clone: public Builtin<Clone> {
  public:
    Clone(): Builtin("clone") {}

    OpResult operator()(VM vm, In record, Out result) {
      return RecordLike(record).clone(vm, result);
    }
  };

  class WaitOr: public Builtin<WaitOr> {
  public:
    WaitOr(): Builtin("waitOr") {}

    OpResult operator()(VM vm, In record, Out result) {
      return RecordLike(record).waitOr(vm, result);
    }
  };

  class MakeDynamic: public Builtin<MakeDynamic> {
  public:
    MakeDynamic(): Builtin("makeDynamic") {}

    OpResult operator()(VM vm, In label, In contents, Out result) {
      using namespace patternmatching;

      OpResult res = OpResult::proceed();
      size_t contentsWidth = 0;
      std::unique_ptr<UnstableNode[]> contentsData;

      if (matchesVariadicSharp(vm, res, contents, contentsWidth,
                               contentsData) && (contentsWidth % 2 == 0)) {
        size_t width = contentsWidth / 2;
        std::unique_ptr<UnstableField[]> elements(new UnstableField[width]);

        for (size_t i = 0; i < width; i++) {
          // Moves are OK because we do not use contentsData afterwards
          elements[i].feature = std::move(contentsData[i*2]);
          elements[i].value = std::move(contentsData[i*2+1]);
        }

        return buildRecordDynamic(vm, result, label, width, elements.get());
      } else {
        return matchTypeError(vm, res, contents,
                              MOZART_STR("#-tuple with even arity"));
      }
    }
  };

  // Pattern matching helpers

  class Test: public Builtin<Test> {
  public:
    Test(): Builtin("test") {}

    OpResult operator()(VM vm, In value, In patLabel, In patFeatures,
                        Out result) {
      using namespace patternmatching;

      UnstableNode patArityUnstable;
      MOZART_CHECK_OPRESULT(
        ModCompilerSupport::MakeArityDynamic::builtin()(
          vm, patLabel, patFeatures, patArityUnstable));
      RichNode patArity = patArityUnstable;

      bool boolResult = false;

      if (patArity.is<Boolean>()) {
        assert(patArity.as<Boolean>().value() == false);
        size_t patWidth = 0;
        MOZART_ASSERT_PROCEED(RecordLike(patFeatures).width(vm, patWidth));

        MOZART_CHECK_OPRESULT(
          RecordLike(value).testTuple(vm, patLabel, patWidth, boolResult));
      } else {
        assert(patArity.is<mozart::Arity>());

        MOZART_CHECK_OPRESULT(
          RecordLike(value).testRecord(vm, patArity, boolResult));
      }

      result = build(vm, boolResult);
      return OpResult::proceed();
    }
  };

  class TestLabel: public Builtin<TestLabel> {
  public:
    TestLabel(): Builtin("testLabel") {}

    OpResult operator()(VM vm, In value, In patLabel, Out result) {
      bool boolResult = false;
      MOZART_CHECK_OPRESULT(
        RecordLike(value).testLabel(vm, patLabel, boolResult));

      result = build(vm, boolResult);
      return OpResult::proceed();
    }
  };

  class TestFeature: public Builtin<TestFeature> {
  public:
    TestFeature(): Builtin("testFeature") {}

    OpResult operator()(VM vm, In value, In patFeature,
                        Out found, Out fieldValue) {
      bool boolFound = false;
      MOZART_CHECK_OPRESULT(
        Dottable(value).lookupFeature(vm, patFeature, boolFound, fieldValue));

      found = build(vm, boolFound);
      if (!boolFound)
        fieldValue = build(vm, unit);

      return OpResult::proceed();
    }
  };
};

}

}

#endif // MOZART_GENERATOR

#endif // __MODRECORD_H
