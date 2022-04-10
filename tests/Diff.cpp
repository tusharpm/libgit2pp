/******************************************************************************
* Permission to use, copy, modify, and distribute the software
* and its documentation for any purpose and without fee is hereby
* granted, provided that the above copyright notice appear in all
* copies and that both that the copyright notice and this
* permission notice and warranty disclaimer appear in supporting
* documentation, and that the name of the author not be used in
* advertising or publicity pertaining to distribution of the
* software without specific, written prior permission.
*
* The author disclaim all warranties with regard to this
* software, including all implied warranties of merchantability
* and fitness.  In no event shall the author be liable for any
* special, indirect or consequential damages or any damages
* whatsoever resulting from loss of use, data or profits, whether
* in an action of contract, negligence or other tortious action,
* arising out of or in connection with the use or performance of
* this software.
*/

#include "TestHelpers.h"
#include "git2pp/repository.h"
#include "git2pp/tree.h"
#include "git2pp/diff.h"
#include "git2pp/diffdelta.h"
#include "git2pp/difffile.h"

using namespace LibGit2pp;

class TestDiff : public TestBase
{
    Q_OBJECT

private slots:
    void testDiffFileList();
};


void TestDiff::testDiffFileList()
{
    Repository repo;
    repo.open(ExistingRepository);

    try {
        Tree oldTree = repo.lookupRevision("4146952e67^{tree}").toTree(); // These commits are chosen right from the start of
        Tree newTree = repo.lookupRevision("e3f21f35e5^{tree}").toTree(); // the development history of libgit2pp
        Diff diff = repo.diffTrees(oldTree, newTree);
        size_t numD = diff.numDeltas();
        std::stringList expectedPaths = std::stringList() << "CMakeLists.txt" << "src/blob.cpp";
        for (size_t i = 0; i < numD; ++i) {
            expectedPaths.removeAll(diff.delta(i).newFile().path());
        }

        QVERIFY2(expectedPaths.isEmpty(), qPrintable("Paths not in diff: " + expectedPaths.join(", ")));
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}

QTEST_MAIN(TestDiff)

#include "Diff.moc"
