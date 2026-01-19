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

int main() {
    // Read the JSON file
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

    // Get the entries array
    if (!data.contains("entries") || data["entries"].empty()) {
        cerr << "Error: No entries found in the JSON file\n";
        return 1;
    }

    cout << "=================================\n";
    cout << "    GIVEAWAY WINNER SELECTOR    \n";
    cout << "=================================\n\n";

    // Seed random number generator with current time
    unsigned seed = static_cast<unsigned>(
        chrono::system_clock::now().time_since_epoch().count()
    );
    mt19937 rng(seed);

    while (true) {
        auto& entries = data["entries"];
        size_t numEntries = entries.size();

        if (numEntries == 0) {
            cout << "No more entries remaining!\n";
            return 1;
        }

        cout << "Total entries: " << numEntries << "\n\n";

        uniform_int_distribution<size_t> dist(0, numEntries - 1);
        size_t winnerIndex = dist(rng);
        auto winner = entries[winnerIndex];

        cout << "*** AND THE WINNER IS... ***\n\n";
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

        cout << "\nIs this a verified account? (y/n): ";
        string input;
        getline(cin, input);

        if (input == "y" || input == "Y" || input == "yes" || input == "Yes") {
            cout << "\n=================================\n";
            cout << "Congratulations to the winner!\n";
            cout << "=================================\n";
            return 0;
        } else {
            cout << "\nRemoving unverified entry and selecting another winner...\n\n";
            entries.erase(entries.begin() + winnerIndex);
            saveEntries(data);
            cout << "---------------------------------\n\n";
        }
    }

    return 0;
}
