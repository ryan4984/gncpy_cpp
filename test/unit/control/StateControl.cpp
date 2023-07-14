#include <gtest/gtest.h>
#include <math.h>

#include <Eigen/Dense>

#include "gncpy/Exceptions.h"
#include "gncpy/math/Math.h"
#include "gncpy/control/StateControl.h"

TEST(ControlTest, StateControlGetControlInputs) {
    Eigen::Vector3d x;
    x << 3.0, 4.0, 1.0;

    Eigen::Vector3d u;
    u << 1.0, 1.0, 1.0;

    lager::gncpy::control::StateControl controller;
    std::vector<uint8_t> inds = {0, 1, 2};

    EXPECT_THROW(controller.getControlInput(x, u, nullptr),
                 lager::gncpy::exceptions::BadParams);

    auto params = lager::gncpy::control::StateControlParams(inds);

    auto out = controller.getControlInput(x, u, &params);

    EXPECT_EQ(x.size(), out.size());

    for (uint8_t ii=0; ii<out.size();ii++) {
        EXPECT_DOUBLE_EQ(out(ii), u(ii));
    }

    SUCCEED();
}

TEST(ControlTest, StateControlGetInputMat) {
    Eigen::Vector3d x;
    x << 3.0, 4.0, 1.0;

    lager::gncpy::control::StateControl controller;
    std::vector<uint8_t> inds = {0, 1, 2};

    auto params = lager::gncpy::control::StateControlParams(inds);

    EXPECT_THROW(controller.getInputMat(x, nullptr),
                 lager::gncpy::exceptions::BadParams);

    auto out = controller.getInputMat(x, &params);
    Eigen::Matrix3d exp({{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}});

    EXPECT_EQ(exp.rows(), out.rows());
    EXPECT_EQ(exp.cols(), out.cols());

    for (uint8_t r = 0; r < exp.rows(); r++) {
        for (uint8_t c = 0; c < exp.cols(); c++) {
            EXPECT_EQ(exp(r, c), out(r, c));
        }
    }

    SUCCEED();
}