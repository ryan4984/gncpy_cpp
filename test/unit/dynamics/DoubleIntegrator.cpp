#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include <gncpy/dynamics/Parameters.h>
#include <gncpy/dynamics/DoubleIntegrator.h>
#include <gncpy/math/Matrix.h>
#include <gncpy/math/Vector.h>


TEST(DoubleInt, Propagate) {
    double dt = 0.1;
    lager::gncpy::dynamics::DoubleIntegrator dyn(dt);
    lager::gncpy::matrix::Vector xk({0., 0., 1., 0.});

    for(uint16_t kk = 0; kk < 10; kk++) {
        double timestep = kk * dt;
        std::cout << "t = " << timestep << ": ";

        xk = dyn.propagateState(timestep, xk);
        for(auto const& x : xk) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_DOUBLE_EQ(xk(0), 1.);

    SUCCEED();
}


TEST(DoubleInt, serialize) {
    double dt = 0.1;
    lager::gncpy::dynamics::DoubleIntegrator dyn(dt);
    dyn.setControlModel([]([[maybe_unused]] double t, [[maybe_unused]] const lager::gncpy::dynamics::ControlParams* params) -> lager::gncpy::matrix::Matrix<double> {
        return lager::gncpy::matrix::identity<double>(4);
    });

    std::cout << "Original class:\n" << dyn.toJSON() << std::endl;

    std::stringstream filtState = dyn.saveClassState();
    auto dyn2 = lager::gncpy::dynamics::DoubleIntegrator<double>::loadClass(filtState);

    std::cout << "Loaded class:\n" << dyn2.toJSON() << std::endl;

    EXPECT_DOUBLE_EQ(dyn.dt(), dyn2.dt());

    // can not save control model or state constraint function
    EXPECT_NE(dyn.hasControlModel(), dyn2.hasControlModel());

    EXPECT_EQ(dyn.hasStateConstraint(), dyn2.hasStateConstraint());

    SUCCEED();
}