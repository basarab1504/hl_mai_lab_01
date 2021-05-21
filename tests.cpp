#include <gtest/gtest.h>
#include "config/config.h"
#include "database/database.h"
#include "database/person.h"
#include <Poco/Data/SessionFactory.h>
using Poco::Data::Session;
using Poco::Data::Statement;
using namespace Poco::Data::Keywords;

long get_ai(int shard_num) {

    int result;

    Poco::Data::Session session = database::Database::get().create_session();
    Statement get_ai(session);
    std::string sql_request = "Select count(*) from Person -- sharding:" + std::to_string(shard_num);
    get_ai << sql_request, into(result), now;

    return result;
}

void reset_ai(int shard_num) {
    Poco::Data::Session session = database::Database::get().create_session();
    Statement reset_ai(session);
    std::string sql_request = "ALTER TABLE Person AUTO_INCREMENT = " + std::to_string((shard_num) + 1) + " -- sharding:" + std::to_string(shard_num);
    reset_ai << sql_request, now;
}

void remove_person(std::string login, int shard_num) {
    Poco::Data::Session session = database::Database::get().create_session();
    Statement cleanup(session);
    std::string sql_request = "DELETE FROM Person where login = ? -- sharding:" + std::to_string(shard_num);
    cleanup << sql_request, use(login), now;
}

class TestApp : public ::testing::Test {
protected:
    TestApp() {
        Config::get().host() = "127.0.0.1";
        Config::get().database() = "sql_test";
        Config::get().port() = "6033";
        Config::get().login() = "test";
        Config::get().password() = "pzjqUkMnc7vfNHET";
    }
    
    ~TestApp() {
        remove_person("20-06-007", 0);
        remove_person("19-85-007", 1);
        remove_person("666-666-666", 1);
        remove_person("19-87-007", 2);
        remove_person("20-06-007-19-85-007-333", 0);

        reset_ai(0);
        reset_ai(1);
        reset_ai(2);
    }
    
    void SetUp() {}
    
    void TearDown() {}

protected:

};

TEST_F(TestApp, TestPerson) {

    database::Person person;
    std::cout << "Shard #0: " << get_ai(0) << std::endl;
    std::cout << "Shard #1: " << get_ai(1) << std::endl;
    std::cout << "Shard #2: " << get_ai(2) << std::endl;
    //POST tests
    person.login() = "20-06-007";
    person.first_name() = "Daniel";
    person.last_name() = "Craig";
    person.age() = 53;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-- sharding:1\n");

    person.login() = "19-85-007";
    person.first_name() = "Pierce";
    person.last_name() = "Brosnan";
    person.age() = 67;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-- sharding:1\n");

    person.login() = "666-666-666";
    person.first_name() = "Dwayne";
    person.last_name() = "Johnson";
    person.age() = 48;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-- sharding:0\n");

    person.login() = "19-87-007";
    person.first_name() = "Tymothy";
    person.last_name() = "Dalton";
    person.age() = 75;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-- sharding:2\n");

    person.login() = "20-06-007-19-85-007-333";
    person.first_name() = "Dwayne";
    person.last_name() = "Jade";
    person.age() = 39;
    testing::internal::CaptureStdout();
    person.save_to_mysql();
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "-- sharding:1\n");

    //GET tests
    database::Person login_result1 = database::Person::read_by_login("20-06-007");
    ASSERT_EQ(login_result1.get_first_name(), "Daniel");
    ASSERT_EQ(login_result1.get_last_name(), "Craig");
    ASSERT_EQ(login_result1.get_age(), 53);

    database::Person login_result2 = database::Person::read_by_login("19-85-007");
    ASSERT_EQ(login_result2.get_first_name(), "Pierce");
    ASSERT_EQ(login_result2.get_last_name(), "Brosnan");
    ASSERT_EQ(login_result2.get_age(), 67);

    auto name_result1 = database::Person::search("Daniel", "Craig");
    ASSERT_EQ(name_result1.at(0).get_login(), "20-06-007");
    ASSERT_EQ(name_result1.size(), 1);

    auto name_result2 = database::Person::search("Dwayne", "J");
    ASSERT_EQ(name_result2.size(), 2);

    auto full_query = database::Person::read_all();
    ASSERT_EQ(full_query.size(), 99992);
}

// TEST_F(TestApp, TestPerson)
// {

//     database::Person person;

//     //POST tests
//     person.login() = "20-06-007";
//     person.first_name() = "Daniel";
//     person.last_name() = "Craig";
//     person.age() = 53;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100001\n");

//     person.login() = "19-85-007";
//     person.first_name() = "Pierce";
//     person.last_name() = "Brosnan";
//     person.age() = 67;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100002\n");

//     person.login() = "19-87-007";
//     person.first_name() = "Tymothy";
//     person.last_name() = "Dalton";
//     person.age() = 75;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100003\n");

//     person.login() = "19-74-007";
//     person.first_name() = "Roger";
//     person.last_name() = "Moore";
//     person.age() = 89;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100004\n");

//     person.login() = "19-62-007";
//     person.first_name() = "Sean";
//     person.last_name() = "Connery";
//     person.age() = 90;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100005\n");

//     person.login() = "666-666-666";
//     person.first_name() = "Dwayne";
//     person.last_name() = "Johnson";
//     person.age() = 48;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100006\n");

//     person.login() = "20-06-007-19-85-007-333";
//     person.first_name() = "Dwayne";
//     person.last_name() = "Jade";
//     person.age() = 39;
//     testing::internal::CaptureStdout();
//     person.save_to_mysql();
//     ASSERT_EQ(testing::internal::GetCapturedStdout(), "inserted:100007\n");

//     //GET tests
//     database::Person login_result1 = database::Person::read_by_login("20-06-007");
//     ASSERT_EQ(login_result1.get_first_name(), "Daniel");
//     ASSERT_EQ(login_result1.get_last_name(), "Craig");
//     ASSERT_EQ(login_result1.get_age(), 53);

//     database::Person login_result2 = database::Person::read_by_login("19-62-007");
//     ASSERT_EQ(login_result2.get_first_name(), "Sean");
//     ASSERT_EQ(login_result2.get_last_name(), "Connery");
//     ASSERT_EQ(login_result2.get_age(), 90);

//     auto name_result1 = database::Person::search("Roger", "Moore");
//     ASSERT_EQ(name_result1.at(0).get_login(), "19-74-007");
//     ASSERT_EQ(name_result1.size(), 1);

//     auto name_result2 = database::Person::search("Dwayne", "J");
//     ASSERT_EQ(name_result2.size(), 2);
//     ASSERT_EQ(name_result2.at(0).get_last_name(), "Johnson");
// }


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}