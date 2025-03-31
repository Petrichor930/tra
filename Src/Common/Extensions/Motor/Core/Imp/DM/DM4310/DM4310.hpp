#pragma once
#include "../DMMotor.hpp"

namespace PINYMOTOR {
class DM4310 : public DMMotor<DM4310> {
private:

public:
    DM4310(InitConfig_s _config);
};
} // namespace PINYMOTOR
