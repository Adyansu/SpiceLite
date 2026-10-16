#include "components/Resistor.h"

#include <stdexcept>

Resistor::Resistor(std::string name, int positiveNode, int negativeNode, double resistanceOhms)
    : Component(std::move(name), positiveNode, negativeNode), resistanceOhms_(resistanceOhms) {
    if (resistanceOhms_ <= 0.0) {
        throw std::invalid_argument("Resistor " + this->name() + " must have positive resistance");
    }
}

void Resistor::applyToMatrix(Eigen::MatrixXd& system,
                             Eigen::VectorXd&,
                             int,
                             int) const {
    const double conductance = 1.0 / resistanceOhms_;
    const int positive = positiveNode() - 1; // Node zero is not an unknown.
    const int negative = negativeNode() - 1;

    if (positive >= 0) {
        system(positive, positive) += conductance;
    }
    if (negative >= 0) {
        system(negative, negative) += conductance;
    }
    if (positive >= 0 && negative >= 0) {
        system(positive, negative) -= conductance;
        system(negative, positive) -= conductance;
    }
}
