#include "testutils/FuseReadTest.h"

#include "fspp/impl/FuseErrnoException.h"

using ::testing::_;
using ::testing::StrEq;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::Action;

using std::min;

using namespace fspp;

class FuseReadOverflowTest: public FuseReadTest {
public:
  const size_t FILESIZE = 1000;
  const size_t READSIZE = 2000;
  const size_t OFFSET = 500;

  void SetUp() override {
    ReturnIsFileOnLstatWithSize(FILENAME, FILESIZE);
    OnOpenReturnFileDescriptor(FILENAME, 0);
    EXPECT_CALL(fsimpl, read(0, _, _, _)).WillRepeatedly(ReturnSuccessfulReadRegardingSize(FILESIZE));
  }
};


TEST_F(FuseReadOverflowTest, ReadMoreThanFileSizeFromBeginning) {
  char buf[READSIZE];
  size_t read_bytes = ReadFileAllowError(FILENAME, buf, READSIZE, 0);
  EXPECT_EQ(FILESIZE, read_bytes);
}

TEST_F(FuseReadOverflowTest, ReadMoreThanFileSizeFromMiddle) {
  char buf[READSIZE];
  size_t read_bytes = ReadFileAllowError(FILENAME, buf, READSIZE, OFFSET);
  EXPECT_EQ(FILESIZE-OFFSET, read_bytes);
}