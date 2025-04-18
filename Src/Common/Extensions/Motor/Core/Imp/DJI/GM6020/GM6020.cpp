#include "GM6020.hpp"

#include "DJIMotor.hpp"

#include <cstring>

using namespace PINYMOTOR;
GM6020::GM6020(const char _name[16], InitConfig_s _config)
        : DJIMotor<GM6020>(_name, _config)
{
    this->model_.measureMax = 8191;
    this->model_.measureMin = 0;
    this->model_.reductionRatio = 1.f;
    this->model_.rxBaseId = 0x200;
    if(_config.offsetId > 3)
        this->model_.txBaseId = 0x2FE;
    else
        this->model_.txBaseId = 0x1FE;

    this->stats_ = DJIMotorStats_s(8192, 1.62f, 1.2f, 0.9f, 0.86f, 0.741f);
    this->log("INFO", "",
                          "Motor %s: An instance of DJIMotor created", this->name_);
}
