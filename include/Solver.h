#pragma once

#include <Eigen/Dense>
#include <complex>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <vector>

class Solver {
public:
    enum class Analysis { Dc, Transient, Ac };
    enum class Kind { Resistor, Capacitor, Inductor, Current, Voltage, Vccs, Vcvs, Cccs, Ccvs };
    struct Element {
        Kind kind;
        std::string name;
        int p = 0, n = 0, cp = 0, cn = 0;
        double value = 0.0, acMagnitude = 0.0, acPhaseDegrees = 0.0;
        std::string controlName;
        double previousVoltage = 0.0, previousCurrent = 0.0;
    };

    void addElement(Element element);
    void setTransient(double stepSeconds, double stopSeconds);
    void setAcSweep(int points, double startHz, double stopHz);
    void solve();
    void solveTransient();
    void solveAc();
    void solveConfigured();
    void printResults(std::ostream& out) const;
    void exportResults(const std::string& filePath) const;
    const Eigen::VectorXd& solution() const;
    int nodeCount() const noexcept { return nodeCount_; }

private:
    using Complex = std::complex<double>;
    struct BranchLayout { std::unordered_map<std::string, int> index; int count = 0; };
    BranchLayout makeBranchLayout(Analysis analysis) const;
    Eigen::VectorXcd assembleAndSolve(Analysis analysis, double frequencyHz, double timeStep);
    void stampConductance(Eigen::MatrixXcd&, int, int, Complex) const;
    void stampCurrent(Eigen::VectorXcd&, int, int, Complex) const;
    void stampBranch(Eigen::MatrixXcd&, Eigen::VectorXcd&, const Element&, int, Complex, int, int) const;
    int nodeIndex(int node) const noexcept { return node == 0 ? -1 : node - 1; }
    std::vector<Element> elements_;
    int nodeCount_ = 0;
    double transientStep_ = 0.0, transientStop_ = 0.0;
    int acPoints_ = 0;
    double acStartHz_ = 0.0, acStopHz_ = 0.0;
    Eigen::VectorXd solution_;
    std::vector<std::pair<double, Eigen::VectorXcd>> sweepResults_;
    Analysis lastAnalysis_ = Analysis::Dc;
    bool solved_ = false;
};
