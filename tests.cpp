#include <gtest/gtest.h>
#include "config/config.h"
#include "database/database.h"
#include "database/person.h"
#include <Poco/Data/SessionFactory.h>

using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

class TestApp : public ::testing::Test
{
protected:
    TestApp()
    {
        Config::get().host() = "127.0.0.1";
        Config::get().database() = "petrushin_labs";
        Config::get().port() = "8080";
        Config::get().login() = "petrushin";
        Config::get().password() = "qwerty123";
    }
    ~TestApp()
    {
        Poco::Data::Session session = database::Database::get().create_session();
        Statement reset_data(session);
        reset_data << "DELETE FROM Person WHERE id > 100000", now;
        Statement reset_increment(session);
        reset_increment << "ALTER TABLE Person AUTO_INCREMENT=100001", now;
    }
    void SetUp() {}
    void TearDown() {}

protected:
};

TEST_F(TestApp, TestPerson)
{

    database::Person person;

    //POST tests
    person.login() = "20-06-007";
    person.first_name() = "Daniel";
    person.last_name() = "Craig";
    person.age() = 53;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100001\n");

    person.login() = "19-85-007";
    person.first_name() = "Pierce";
    person.last_name() = "Brosnan";
    person.age() = 67;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100002\n");

    person.login() = "19-87-007";
    person.first_name() = "Tymothy";
    person.last_name() = "Dalton";
    person.age() = 75;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100003\n");

    person.login() = "19-74-007";
    person.first_name() = "Roger";
    person.last_name() = "Moore";
    person.age() = 89;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100004\n");

    person.login() = "19-62-007";
    person.first_name() = "Sean";
    person.last_name() = "Connery";
    person.age() = 90;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100005\n");

    person.login() = "666-666-666";
    person.first_name() = "Dwayne";
    person.last_name() = "Johnson";
    person.age() = 48;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100006\n");

    person.login() = "111-222-333";
    person.first_name() = "Dwayne";
    person.last_name() = "Jade";
    person.age() = 39;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100007\n");

    //GET tests
    database::Person login_result1 = database::Person::read_by_login("20-06-007");
    ASSERT_EQ(login_result1.get_first_name(), "Daniel");
    ASSERT_EQ(login_result1.get_last_name(), "Craig");
    ASSERT_EQ(login_result1.get_age(), 53);

    database::Person login_result2 = database::Person::read_by_login("19-62-007");
    ASSERT_EQ(login_result2.get_first_name(), "Sean");
    ASSERT_EQ(login_result2.get_last_name(), "Connery");
    ASSERT_EQ(login_result2.get_age(), 90);

    auto name_result1 = database::Person::search("Roger", "Moore");
    ASSERT_EQ(name_result1.at(0).get_login(), "19-74-007");
    ASSERT_EQ(name_result1.size(), 1);

    auto name_result2 = database::Person::search("Dwayne", "J");
    ASSERT_EQ(name_result2.size(), 2);
    ASSERT_EQ(name_result2.at(0).get_last_name(), "Johnson");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}