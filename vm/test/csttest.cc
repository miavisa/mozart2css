#include "mozart.hh"

#include <climits>
#include <gtest/gtest.h>
#include "testutils.hh"

using namespace mozart;

class CstTest : public MozartTest {};

TEST_F(CstTest, SpaceCreation) {
  Space *currentSpace = vm->getCurrentSpace();
  GecodeSpace& cst = currentSpace->getCstSpace();
  cst.dumpSpaceInformation();
}

TEST_F(CstTest, InvalidElements) {
  UnstableNode n = SmallInt::build(vm,INT_MAX);
  EXPECT_FALSE(IntVarLike(n).isIntVarLike(vm));
  
  UnstableNode o = SmallInt::build(vm,INT_MAX-1);
  EXPECT_TRUE(IntVarLike(o).isIntVarLike(vm));

  UnstableNode p = SmallInt::build(vm,-(INT_MAX-1));
  EXPECT_TRUE(IntVarLike(p).isIntVarLike(vm));
}
