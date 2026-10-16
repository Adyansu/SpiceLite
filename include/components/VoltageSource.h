#pragma once

#include "components/Component.h"

class VoltageSource final : public Component {
public:
    VoltageSource(std::string name, int positiveNode, int negativeNode, double voltageVolts);

    void applyToMatrix(Eigen::MatrixXd& system,
                       Eigen::VectorXd& rhs,
                       int nodeCount,
                       int voltageSourceIndex) const override;
    bool isVoltageSource() const noexcept override { return true; }

    double voltageVolts() const noexcept { return voltageVolts_; }

private:
    double voltageVolts_;
};
