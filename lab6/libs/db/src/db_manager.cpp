#include "db/db_manager.hpp"

#include <iostream>

#include "db/sql_loader.hpp"

namespace classifier::db {

DatabaseFacade::DatabaseFacade(std::string connection_string)
    : conn_str_(std::move(connection_string)) {
}

void DatabaseFacade::connect() {
    connection_ = std::make_unique<pqxx::connection>(conn_str_);
    file_repo_ = std::make_unique<FileRepository>(*connection_);
}

void DatabaseFacade::createSchema() {
    pqxx::work tx(*connection_);
    std::string schema = SqlLoader::load("sql/schema.sql");
    tx.exec(schema);
    tx.commit();
}

void FileRepository::addFile(const models::FileRecord& file) {
    pqxx::work tx(conn_);
    conn_.prepare("insert_file",
                  "INSERT INTO Files (filename, size_bytes, type_id) VALUES ($1, $2, $3)");
    tx.exec_prepared("insert_file", file.filename, file.size, file.type_id);
    tx.commit();
    conn_.unprepare("insert_file");
}

std::vector<models::FileRecord> FileRepository::getAllFiles() {
    pqxx::nontransaction nt(conn_);
    pqxx::result res = nt.exec("SELECT id, filename, size_bytes, type_id FROM Files");
    std::vector<models::FileRecord> files;
    for (auto row : res) {
        files.push_back(
            {row[0].as<int>(), row[1].as<std::string>(), row[2].as<long long>(), row[3].as<int>()});
    }
    return files;
}

void DatabaseFacade::runComplexAnalytics(double min_confidence) {
    pqxx::nontransaction nt(*connection_);
    std::string sql = SqlLoader::load("sql/analytics.sql");
    auto res = nt.exec_params(sql, min_confidence);
    for (auto row : res) {
        std::cout << "Analysis Result: " << row[0].as<std::string>()
                  << " | Value: " << row[1].as<std::string>() << std::endl;
    }
}

void DatabaseFacade::runNamedQuery(const std::string& query_name) {
    pqxx::nontransaction nt(*connection_);
    std::string sql = SqlLoader::load("sql/" + query_name + ".sql");
    auto res = nt.exec(sql);
    std::cout << "--- Results for " << query_name << " ---" << std::endl;
    for (auto row : res) {
        for (auto field : row) std::cout << field.c_str() << " ";
        std::cout << std::endl;
    }
}

void FileRepository::testInjection(const std::string& attack_name, const std::string& payload) {
    pqxx::work tx(conn_);
    std::string sql = SqlLoader::load("sql/injection_test.sql");
    try {
        conn_.prepare("test_inj", sql);
        tx.exec_prepared("test_inj", payload);
        std::cout << "[SUCCESS] " << attack_name << " neutralized (handled as parameter)."
                  << std::endl;
        conn_.unprepare("test_inj");
    } catch (const std::exception& e) {
        std::cout << "[BLOCKED] " << attack_name << " failed by DB engine: " << e.what()
                  << std::endl;
    }
}

void DatabaseFacade::executeRawQuery(const std::string& query) {
    pqxx::work tx(*connection_);
    tx.exec(query);
    tx.commit();
}

}  // namespace classifier::db