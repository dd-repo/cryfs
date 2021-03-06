#pragma once
#ifndef MESSMER_FSPP_FSTEST_FSPPNODETEST_STAT_H_
#define MESSMER_FSPP_FSTEST_FSPPNODETEST_STAT_H_

#include "testutils/FsppNodeTest.h"
#include "../fuse/FuseErrnoException.h"

template<class ConcreteFileSystemTestFixture>
class FsppNodeTest_Stat: public FsppNodeTest<ConcreteFileSystemTestFixture> {
public:
    void Test_Nlink() {
        this->CreateNode("/mynode");
        auto node = this->Load("/mynode");
        this->IN_STAT(node.get(), [] (struct stat st) {
            EXPECT_EQ(1u, st.st_nlink);
        });
    }
};

// Test cases only run for file nodes
template<class ConcreteFileSystemTestFixture>
class FsppNodeTest_Stat_FileOnly: public FileSystemTest<ConcreteFileSystemTestFixture>, public FsppNodeTestHelper {};

TYPED_TEST_CASE_P(FsppNodeTest_Stat_FileOnly);

TYPED_TEST_P(FsppNodeTest_Stat_FileOnly, CreatedFileIsEmpty) {
    this->CreateFile("/myfile");
    auto node = this->Load("/myfile");
    this->EXPECT_SIZE(0, node.get());
}

TYPED_TEST_P(FsppNodeTest_Stat_FileOnly, FileIsFile) {
    this->CreateFile("/myfile");
    auto node = this->Load("/myfile");
    this->IN_STAT(node.get(), [] (struct stat st) {
        EXPECT_TRUE(S_ISREG(st.st_mode));
    });
}

// Test cases only run for dir nodes
template<class ConcreteFileSystemTestFixture>
class FsppNodeTest_Stat_DirOnly: public FileSystemTest<ConcreteFileSystemTestFixture>, public FsppNodeTestHelper {};

TYPED_TEST_CASE_P(FsppNodeTest_Stat_DirOnly);

TYPED_TEST_P(FsppNodeTest_Stat_DirOnly, DirIsDir) {
    this->CreateDir("/mydir");
    auto node = this->Load("/mydir");
    this->IN_STAT(node.get(), [] (struct stat st) {
        EXPECT_TRUE(S_ISDIR(st.st_mode));
    });
}

// Test cases only run for symlink nodes
template<class ConcreteFileSystemTestFixture>
class FsppNodeTest_Stat_SymlinkOnly: public FileSystemTest<ConcreteFileSystemTestFixture>, public FsppNodeTestHelper {};

TYPED_TEST_CASE_P(FsppNodeTest_Stat_SymlinkOnly);

TYPED_TEST_P(FsppNodeTest_Stat_SymlinkOnly, SymlinkIsSymlink) {
    this->CreateSymlink("/mysymlink");
    auto node = this->Load("/mysymlink");
    this->IN_STAT(node.get(), [] (struct stat st) {
        EXPECT_TRUE(S_ISLNK(st.st_mode));
    });
}

REGISTER_NODE_TEST_CASE(FsppNodeTest_Stat,
    Nlink
);

REGISTER_TYPED_TEST_CASE_P(FsppNodeTest_Stat_FileOnly,
    CreatedFileIsEmpty,
    FileIsFile
);

REGISTER_TYPED_TEST_CASE_P(FsppNodeTest_Stat_DirOnly,
    DirIsDir
);

REGISTER_TYPED_TEST_CASE_P(FsppNodeTest_Stat_SymlinkOnly,
    SymlinkIsSymlink
);

#endif

//TODO More test cases
