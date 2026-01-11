#include <gtest/gtest.h>
#include <db/db_manager.hpp>

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        facade = std::make_unique<classifier::db::DatabaseFacade>("host=localhost dbname=test_db user=postgres password=root");
        facade->connect();
    }
    std::unique_ptr<classifier::db::DatabaseFacade> facade;
};

TEST_F(DatabaseTest, TestInjectionProtection) {
    EXPECT_NO_THROW({
        classifier::models::FileRecord malicious;
        malicious.filename = "'; DROP TABLE Files; --";
        malicious.size = 100;
        malicious.type_id = 1;
        facade->getFileRepo().addFile(malicious);
    });
}

TEST_F(DatabaseTest, TestJoinQuery) {
    EXPECT_NO_THROW({
        facade->runComplexAnalytics();
    });
}