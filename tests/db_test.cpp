/* Copyright 2020 (C) LACIT
 * Created by illidansr on 26.02.20.
 * @details tests for config_loader component
 */

#include <string>
#include <iostream>
#include <gtest/gtest.h>
//#include <connector/db_connect.hpp>

/*

class TestDB : public ::testing::Test {
public:
    TestDB() { */
/* init protected members here *//*
 }
    ~TestDB() { */
/* free protected members here *//*
 }
    void set_up() { */
/* called before every test *//*
 }
    void tear_down() { */
/* called after every test *//*
 }
};

TEST_F(TestDB, MongoDB) {
    MongoDB *mongoDb;
    mongoDb = MongoDB::get_instance();
    storage_service_package::StorageAddRequest request;
    storage_service_package::StorageGetRequest getRequest;
    storage_service_package::StorageGetResponse getResponse;
    ASSERT_NO_THROW(mongoDb->add(request));
    request.set_task_id(1111.1);
    ASSERT_ANY_THROW(mongoDb->add(request));
    getRequest.set_task_id(1);
    mongoDb->get(getRequest,getResponse);
    ASSERT_EQ(getRequest.task_id(), getResponse.task_id());
}
*/
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}