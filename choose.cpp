#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

void saveEntries(const json& data) {
    ofstream outFile("giveaway-entries.json");
    outFile << data.dump(2);
    outFile.close();
}

void printJson(const json& data) {
    cout << "\n" << data.dump(2) << "\n\n";
}

int main() {
    ifstream file("giveaway-entries.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open giveaway-entries.json\n";
        return 1;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        cerr << "Error parsing JSON: " << e.what() << "\n";
        return 1;
    }
    file.close();
    if (!data.contains("entries") || data["entries"].empty()) {
        cerr << "Error: No entries found in the JSON file\n";
        return 1;
    }
    cout << "    GIVEAWAY WINNER SELECTOR    \n";
    unsigned seed = static_cast<unsigned>(
        chrono::system_clock::now().time_since_epoch().count()
    );
    mt19937 rng(seed);

    while (true) {
        auto& entries = data["entries"];
        size_t numEntries = entries.size();

        if (numEntries == 0) {
            cout << "No more entries\n";
            return 1;
        }

        cout << "Total entries: " << numEntries << "\n\n";

        uniform_int_distribution<size_t> dist(0, numEntries - 1);
        size_t winnerIndex = dist(rng);
        auto winner = entries[winnerIndex];

        cout << "winner: \n";
        cout << "Name:    " << winner["fullName"].get<string>() << "\n";
        cout << "Email:   " << winner["email"].get<string>() << "\n";
        cout << "Discord: " << winner["discord"].get<string>() << "\n";
        cout << "Country: " << winner["country"].get<string>() << "\n";
        
        if (winner.contains("firms") && winner["firms"].is_array()) {
            cout << "Firms:   ";
            for (size_t i = 0; i < winner["firms"].size(); ++i) {
                if (i > 0) cout << ", ";
                cout << winner["firms"][i].get<string>();
            }
            cout << "\n";
        }

        cout << "\nIs this a verified account? (y/n/p to print json): ";
        string input;
        getline(cin, input);

        if (input == "y") {
            cout << "done\n";
            return 0;
        } else if (input == "p") {
            printJson(winner);
        } else {
            cout << "\nremoved unverified entry\n\n";
            entries.erase(entries.begin() + winnerIndex);
            saveEntries(data);
        }
    }

    return 0;
}
