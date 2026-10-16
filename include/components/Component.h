#pragma once

#include <Eigen/Dense>

#include <string>
#include <utility>

// Base class for all elements that contribute equations to the MNA system.
class Component {
public:
    Component(std::string name, int positiveNode, int negativeNode)
        : name_(std::move(name)), positiveNode_(positiveNode), negativeNode_(negativeNode) {}
    virtual ~Component() = default;

    const std::string& name() const noexcept { return name_; }
    int positiveNode() const noexcept { return positiveNode_; }
    int negativeNode() const noexcept { return negativeNode_; }

    // nodeCount excludes ground. voltageSourceIndex is -1 for passive elements.
    virtual void applyToMatrix(Eigen::MatrixXd& system,
                               Eigen::VectorXd& rhs,
                               int nodeCount,
                               int voltageSourceIndex) const = 0;
    virtual bool isVoltageSource() const noexcept { return false; }

private:
    std::string name_;
    int positiveNode_;
    int negativeNode_;
};
