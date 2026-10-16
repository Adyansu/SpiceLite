#include "components/VoltageSource.h"

#include <stdexcept>

VoltageSource::VoltageSource(std::string name, int positiveNode, int negativeNode, double voltageVolts)
    : Component(std::move(name), positiveNode, negativeNode), voltageVolts_(voltageVolts) {}

void VoltageSource::applyToMatrix(Eigen::MatrixXd& system,
                                  Eigen::VectorXd& rhs,
                                  int nodeCount,
                                  int voltageSourceIndex) const {
    if (voltageSourceIndex < 0) {
        throw std::logic_error("Voltage source stamped without a current-variable index");
    }

    const int currentVariable = nodeCount + voltageSourceIndex;
    const int positive = positiveNode() - 1;
    const int negative = negativeNode() - 1;

    // B (top right) and C (bottom left); D stays zero for an ideal source.
    if (positive >= 0) {
        system(positive, currentVariable) += 1.0;
        system(currentVariable, positive) += 1.0;
    }
    if (negative >= 0) {
        system(negative, currentVariable) -= 1.0;
        system(currentVariable, negative) -= 1.0;
    }
    rhs(currentVariable) += voltageVolts_;
}
