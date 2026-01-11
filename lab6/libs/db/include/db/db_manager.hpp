#pragma once
#include <memory>
#include <models/entities.hpp>
#include <pqxx/pqxx>
#include <string>
#include <vector>

namespace classifier::db {

class IDatabase {
   public:
    virtual ~IDatabase() = default;
    virtual void connect() = 0;
    virtual void executeRawQuery(const std::string& query) = 0;
};

class FileRepository {
   public:
    explicit FileRepository(pqxx::connection& conn) : conn_(conn) {
    }

    void addFile(const models::FileRecord& file);
    std::vector<models::FileRecord> getAllFiles();

    void testInjection(const std::string& attack_name, const std::string& payload);

   private:
    pqxx::connection& conn_;
};

class DatabaseFacade : public IDatabase {
   public:
    explicit DatabaseFacade(std::string connection_string);
    void connect() override;
    void executeRawQuery(const std::string& query) override;

    FileRepository& getFileRepo() {
        return *file_repo_;
    }

    void createSchema();
    void runComplexAnalytics(double min_confidence = 0.0);
    void runNamedQuery(const std::string& query_name);

   private:
    std::string conn_str_;
    std::unique_ptr<pqxx::connection> connection_;
    std::unique_ptr<FileRepository> file_repo_;
};

}  // namespace classifier::db