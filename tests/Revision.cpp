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
#include "doctest.h"

#include <iostream>

using namespace LibGit2pp;

TEST_SUITE_BEGIN("Revision");

TEST_CASE_FIXTURE(TestBase, "revwalk")
{
    Repository repo;
    try {
        // Open a local fixed path
        repo.open(ExistingRepository);

        RevWalk rw(repo);

        rw.setSorting(RevWalk::Topological);

        rw.pushHead();

        Commit commit;
        while(rw.next(commit)) {
            auto qb = commit.oid().format();
            std::cout << qb << std::endl;
        }

    } catch (const Exception& ex) {
        FAIL(ex.what());
    }
}

TEST_SUITE_END();
