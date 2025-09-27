#pragma once

#include "Array.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory> 
#include <limits>

using namespace std;

template<typename T>
class ArrayCLI {
public:
    ArrayCLI() 
    { 
        init_commands(); 
    }

    void run() {
        string line;
        while (true) {
            cout << "> ";
            if (!getline(cin, line)) break;

            istringstream iss(line);
            string cmd;
            iss >> cmd;

            auto it = commands.find(cmd);

            if (it != commands.end()) 
            {
                try { 
                    it->second(iss); 
                }
                catch (const exception& e) { 
                    cout << "Error: " << e.what() << "\n"; 
                }
            } 
            else 
            {
                cout << "Unknown command. Type 'help'.\n";
            }
        }
    }

private:
    Array<T> arr;
    unordered_map<string, function<void(istringstream&)>> commands;

    void init_commands() {
        commands["push"] = [this](istringstream &iss) 
        {
            T x;

            if (!(iss >> x)) 
            { 
                cout << "Error: missing or invalid value\n"; 
                return; 
            }

            arr.push(x);
        };

        commands["insert"] = [this](istringstream &iss) 
        {
            int idx; T val;

            if (!(iss >> idx)) 
            { 
                cout << "Error: missing index\n"; 
                return; 
            }
            if (!(iss >> val)) 
            { 
                cout << "Error: missing value\n"; 
                return; 
            }

            try { 
                arr.insert(idx, val); 
            } 
            catch (const out_of_range& e) { 
                cout << "Error: " << e.what() << "\n"; 
            }
        };

        commands["remove"] = [this](istringstream &iss) 
        {
            int idx;
            if (!(iss >> idx)) 
            { 
                cout << "Error: missing index\n"; 
                return; 
            }

            try { 
                arr.remove(idx); 
            } 
            catch (const out_of_range& e) { 
                cout << "Error: " << e.what() << "\n"; 
            }
        };

        commands["print"] = [this](istringstream&) 
        { 
            cout << arr << "\n"; 
        };

        commands["size"]  = [this](istringstream&) 
        { 
            cout << arr.size() << "\n"; 
        };

        commands["clear"] = [this](istringstream&) 
        { 
            arr = Array<T>();  
            cout << "Array cleared.\n";
        };

        commands["help"] = [this](istringstream&) 
        {
            cout << "Commands:\n"
                 << "\tpush X\n"
                 << "\tinsert IDX VAL\n"
                 << "\tremove IDX\n"
                 << "\tprint\n"
                 << "\tsize\n"
                 << "\tclear\n"
                 << "\thelp\n"
                 << "\texit\n"
                 << endl;
        };

        commands["exit"] = [this](istringstream&) 
        { 
            exit(0); 
        };
    }
};

class DynamicCLI {
public:
    void run() 
    {   
        cout << "Choose type:\n" 
             << "\t1. int\n"
             << "\t2. double\n"
             << "\t3. string\n"
             << "\t0. exit\n"
             << endl;

        while (true) {
            int choice;
            cout << "> ";

            if (!(cin >> choice))
            { 
                cout << "Invalid input. Please enter a number.\n";

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (choice) 
        {
            case 0: 
            {
                cout << "Exiting program.\n";
                return; 
            }
            case 1: 
            {
                ArrayCLI<int> cli;
                cli.run();
                break;
            }
            case 2: 
            {
                ArrayCLI<double> cli;
                cli.run();
                break;
            }
            case 3: 
            {
                ArrayCLI<string> cli;
                cli.run();
                break;
            }
            default: 
            {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
        }
    }
};

