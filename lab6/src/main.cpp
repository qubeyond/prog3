#include <db/db_manager.hpp>
#include <iostream>
#include <vector>
#include <map>

int main() {
    using namespace classifier;
    try {
        db::DatabaseFacade db("host=localhost port=5432 dbname=test_db user=postgres password=root");
        db.connect();
        db.createSchema();

        std::cout << "\nDATABASE ANALYTICS SUMMARY\n";
        std::cout << "==========================\n\n";

        db.runComplexAnalytics(0.7);
        std::cout << "\n";

        db.runNamedQuery("get_malware_stats");
        std::cout << "\n";

        db.runNamedQuery("get_conflicting_results");
        std::cout << "\n";

        db.runNamedQuery("get_files_by_type");
        std::cout << "\n";

        db.runNamedQuery("get_top_models");
        std::cout << "\n\n";

        std::cout << "SECURITY AUDIT & VULNERABILITY TESTS\n";
        std::cout << "====================================\n";
        
        std::map<std::string, std::string> attacks = {
            {"Tautology Attack", "' OR 1=1 --"},
            {"Stacked Query Attack", "'; DROP TABLE Files; --"},
            {"Union-Based Attack", "' UNION SELECT NULL, model_name, version, algorithm FROM MLModels --"},
            {"Error-Based Attack", "1 AND 1=(SELECT CAST(VERSION() AS INT))"},
            {"Blind-Boolean Attack", "1 AND (SELECT SUBSTR(model_name,1,1) FROM MLModels LIMIT 1)='R'"}
        };

        for (const auto& [name, payload] : attacks) {
            db.getFileRepo().testInjection(name, payload);
        }
        std::cout << "\n";

    } catch (const std::exception& e) {
        std::cerr << "\n[CRITICAL ERROR]: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}