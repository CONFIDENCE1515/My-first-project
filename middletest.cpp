#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <iomanip>

class Planner {
private:
    std::string material;
    std::string wing_type;
    std::string takeoff;       // "from the ground" or "from a hill"
    double weight;
    std::string manufacturer;
    std::string model;

public:
    // ── Default constructor ──────────────────────────────────────────────────
    Planner()
        : material(""), wing_type(""), takeoff(""), weight(0.0),
          manufacturer(""), model("") {
        std::cout << "[Default constructor called]\n";
    }

    // ── Parameterized constructor ────────────────────────────────────────────
    Planner(const std::string& mat, const std::string& wt,
            const std::string& to, double w,
            const std::string& mfr, const std::string& mdl)
        : material(mat), wing_type(wt), takeoff(to), weight(w),
          manufacturer(mfr), model(mdl) {
        std::cout << "[Parameterized constructor called for: " << model << "]\n";
    }

    // ── Destructor ───────────────────────────────────────────────────────────
    ~Planner() {
        std::cout << "[Destructor called for: " << model << "]\n";
    }

    // ── Copy constructor ─────────────────────────────────────────────────────
    Planner(const Planner& other)
        : material(other.material), wing_type(other.wing_type),
          takeoff(other.takeoff), weight(other.weight),
          manufacturer(other.manufacturer), model(other.model) {
        std::cout << "[Copy constructor called for: " << model << "]\n";
    }

    // ── Copy assignment operator ─────────────────────────────────────────────
    Planner& operator=(const Planner& other) {
        std::cout << "[Copy assignment called]\n";
        if (this != &other) {
            material     = other.material;
            wing_type    = other.wing_type;
            takeoff      = other.takeoff;
            weight       = other.weight;
            manufacturer = other.manufacturer;
            model        = other.model;
        }
        return *this;
    }

    // ── Move constructor ─────────────────────────────────────────────────────
    Planner(Planner&& other) noexcept
        : material(std::move(other.material)),
          wing_type(std::move(other.wing_type)),
          takeoff(std::move(other.takeoff)),
          weight(other.weight),
          manufacturer(std::move(other.manufacturer)),
          model(std::move(other.model)) {
        other.weight = 0.0;
        std::cout << "[Move constructor called]\n";
    }

    // ── Move assignment operator ─────────────────────────────────────────────
    Planner& operator=(Planner&& other) noexcept {
        std::cout << "[Move assignment called]\n";
        if (this != &other) {
            material     = std::move(other.material);
            wing_type    = std::move(other.wing_type);
            takeoff      = std::move(other.takeoff);
            weight       = other.weight;
            manufacturer = std::move(other.manufacturer);
            model        = std::move(other.model);
            other.weight = 0.0;
        }
        return *this;
    }

    // ── Getters ──────────────────────────────────────────────────────────────
    std::string getMaterial()     const { return material; }
    std::string getWingType()     const { return wing_type; }
    std::string getTakeoff()      const { return takeoff; }
    double      getWeight()       const { return weight; }
    std::string getManufacturer() const { return manufacturer; }
    std::string getModel()        const { return model; }

    // ── Setters ──────────────────────────────────────────────────────────────
    void setMaterial    (const std::string& v) { material     = v; }
    void setWingType    (const std::string& v) { wing_type    = v; }
    void setTakeoff     (const std::string& v) { takeoff      = v; }
    void setWeight      (double v)             { weight       = v; }
    void setManufacturer(const std::string& v) { manufacturer = v; }
    void setModel       (const std::string& v) { model        = v; }
};

// ── Helper: input one Planner from the user ──────────────────────────────────
Planner inputPlanner(int index) {
    std::string mat, wt, to, mfr, mdl;
    double w;

    std::cout << "\n--- Enter data for Planner #" << index << " ---\n";

    std::cout << "Material: ";
    std::getline(std::cin, mat);

    std::cout << "Wing type: ";
    std::getline(std::cin, wt);

    std::cout << "Takeoff (from the ground / from a hill): ";
    std::getline(std::cin, to);

    std::cout << "Weight (kg): ";
    std::cin >> w;
    std::cin.ignore();

    std::cout << "Manufacturer: ";
    std::getline(std::cin, mfr);

    std::cout << "Model: ";
    std::getline(std::cin, mdl);

    return Planner(mat, wt, to, w, mfr, mdl);
}

// ── Helper: print table header ───────────────────────────────────────────────
void printHeader() {
    std::cout << "\n"
              << std::left
              << std::setw(12) << "Model"
              << std::setw(14) << "Material"
              << std::setw(12) << "Wing Type"
              << std::setw(18) << "Takeoff"
              << std::setw(10) << "Weight"
              << std::setw(14) << "Manufacturer"
              << "\n";
    std::cout << std::string(80, '-') << "\n";
}

// ── Helper: print one row ────────────────────────────────────────────────────
void printRow(const Planner& p) {
    std::cout << std::left
              << std::setw(12) << p.getModel()
              << std::setw(14) << p.getMaterial()
              << std::setw(12) << p.getWingType()
              << std::setw(18) << p.getTakeoff()
              << std::setw(10) << p.getWeight()
              << std::setw(14) << p.getManufacturer()
              << "\n";
}

// ── main ─────────────────────────────────────────────────────────────────────
int main() {
    std::list<Planner> planners;

    // Input 3 objects
    for (int i = 1; i <= 3; ++i) {
        planners.push_back(inputPlanner(i));
    }

    // Sort by weight in REVERSE (descending) order
    planners.sort([](const Planner& a, const Planner& b) {
        return a.getWeight() > b.getWeight();
    });

    // Print table
    std::cout << "\n========== Planners sorted by weight (heaviest first) ==========";
    printHeader();
    for (const auto& p : planners) {
        printRow(p);
    }

    // Delete all data from the list using appropriate methods
    std::cout << "\n--- Deleting all objects from the list ---\n";
    while (!planners.empty()) {
        planners.pop_front();   // calls destructor for each element
    }
    // Alternative: planners.clear();

    std::cout << "\nList is now empty. Size = " << planners.size() << "\n";

    return 0;
}