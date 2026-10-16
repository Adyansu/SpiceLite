#pragma once

#include "components/Component.h"

class Resistor final : public Component {
public:
    Resistor(std::string name, int positiveNode, int negativeNode, double resistanceOhms);

    void applyToMatrix(Eigen::MatrixXd& system,
                       Eigen::VectorXd& rhs,
                       int nodeCount,
                       int voltageSourceIndex) const override;

    double resistanceOhms() const noexcept { return resistanceOhms_; }

private:
    double resistanceOhms_;
};
