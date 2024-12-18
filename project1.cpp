//Author - Anirudh Sarkar & Shashank Sharma
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Distributor {
public:
    int id;
    string name;
    double sales;
    double commission;

    Distributor() : id(0), name(""), sales(0), commission(0) {}

    Distributor(int id, string name) : id(id), name(name), sales(0), commission(0) {}
};

class MLMNetwork {
private:
    unordered_map<int, Distributor> distributors;
    unordered_map<int, vector<int>> adjacencyList;

public:
    void addDistributor(int id, string name) {
        distributors[id] = Distributor(id, name);
        cout << "Added distributor: " << name << " with ID: " << id << endl;
    }

    void addRelationship(int parentId, int childId) {
        if (distributors.find(parentId) == distributors.end()) {
            cout << "Parent distributor ID " << parentId << " not found!" << endl;
            return;
        }
        if (distributors.find(childId) == distributors.end()) {
            cout << "Child distributor ID " << childId << " not found!" << endl;
            return;
        }
        adjacencyList[parentId].push_back(childId);
        cout << "Added relationship: Parent ID " << parentId << " -> Child ID " << childId << endl;
    }

    // Delete dist&relation
    void deleteDistributor(int id) {
        if (distributors.find(id) == distributors.end()) {
            cout << "Distributor ID " << id << " not found!" << endl;
            return;
        }

        // Remove adjacency list parent-child relationships
        for (auto& entry : adjacencyList) {
            vector<int>& children = entry.second;
            children.erase(remove(children.begin(), children.end(), id), children.end());
        }

        // Remove the distributor from the main list
        distributors.erase(id);
        adjacencyList.erase(id);

        cout << "Deleted distributor with ID: " << id << endl;
    }

    // Update relationship: change childId's parent to a new parentId
    void updateRelationship(int oldParentId, int newParentId, int childId) {
        if (distributors.find(oldParentId) == distributors.end()) {
            cout << "Old parent distributor ID " << oldParentId << " not found!" << endl;
            return;
        }
        if (distributors.find(newParentId) == distributors.end()) {
            cout << "New parent distributor ID " << newParentId << " not found!" << endl;
            return;
        }
        if (distributors.find(childId) == distributors.end()) {
            cout << "Child distributor ID " << childId << " not found!" << endl;
            return;
        }

        // Remove child from the old parent
        auto& oldParentChildren = adjacencyList[oldParentId];
        oldParentChildren.erase(remove(oldParentChildren.begin(), oldParentChildren.end(), childId), oldParentChildren.end());

        // Add child to the new parent
        adjacencyList[newParentId].push_back(childId);
        cout << "Updated relationship: Child ID " << childId << " moved from Parent ID " << oldParentId << " to Parent ID " << newParentId << endl;
    }

    void updateSales(int id, double sales) {
        if (distributors.find(id) == distributors.end()) {
            cout << "Distributor ID " << id << " not found! Sales update failed." << endl;
            return;
        }
        distributors[id].sales += sales;
        cout << "Updated sales for Distributor " << distributors[id].name << ": " << sales << endl;
    }

    void calculateCommissions() {
        cout << "Calculating commissions..." << endl;
        for (auto& distributor : distributors) {
            int id = distributor.first;
            calculateCommissionsRecursive(id);
        }
    }

    double calculateCommissionsRecursive(int id) {
        double sales = distributors[id].sales;
        distributors[id].commission = sales * 0.1;
        cout << "Distributor " << distributors[id].name << " has base commission: " << distributors[id].commission << endl;

        for (auto& childId : adjacencyList[id]) {
            double childCommission = calculateCommissionsRecursive(childId);
            distributors[id].commission += childCommission * 0.05;
        }

        return distributors[id].commission;
    }

    void printNetwork() {
        cout << "Final network details: " << endl;
        for (auto& distributor : distributors) {
            cout << "Distributor " << distributor.second.name << " (ID: " << distributor.first << "):" << endl;
            cout << "  Sales: " << distributor.second.sales << endl;
            cout << "  Commission: " << distributor.second.commission << endl;
            cout << endl;
        }
    }
};

int main() {
    MLMNetwork network;
    int choice;

    while (true) {
        cout << endl << endl;
        cout << "1. Add Distributor\n";
        cout << "2. Add Relationship\n";
        cout << "3. Update Sales\n";
        cout << "4. Calculate Commissions\n";
        cout << "5. Print Network\n";
        cout << "6. Delete Distributor\n";
        cout << "7. Update Relationship\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl << endl;

        if (choice == 1) {
            int id;
            string name;
            cout << "Enter distributor ID: ";
            cin >> id;
            cout << "Enter distributor name: ";
            cin >> name;
            network.addDistributor(id, name);
        }
        else if (choice == 2) {
            int parentId, childId;
            cout << "Enter parent distributor ID: ";
            cin >> parentId;
            cout << "Enter child distributor ID: ";
            cin >> childId;
            network.addRelationship(parentId, childId);
        }
        else if (choice == 3) {
            int id;
            double sales;
            cout << "Enter distributor ID: ";
            cin >> id;
            cout << "Enter sales amount: ";
            cin >> sales;
            network.updateSales(id, sales);
        }
        else if (choice == 4) {
            network.calculateCommissions();
        }
        else if (choice == 5) {
            network.printNetwork();
        }
        else if (choice == 6) {
            int id;
            cout << "Enter distributor ID to delete: ";
            cin >> id;
            network.deleteDistributor(id);
        }
        else if (choice == 7) {
            int oldParentId, newParentId, childId;
            cout << "Enter old parent distributor ID: ";
            cin >> oldParentId;
            cout << "Enter new parent distributor ID: ";
            cin >> newParentId;
            cout << "Enter child distributor ID: ";
            cin >> childId;
            network.updateRelationship(oldParentId, newParentId, childId);
        }
        else if (choice == 8) {
            break;
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}
