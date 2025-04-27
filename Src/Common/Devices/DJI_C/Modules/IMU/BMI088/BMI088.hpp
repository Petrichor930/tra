#pragma once
#include <stdint.h>

#include "./DrvLib/bmi08x_defs.h"

struct BMI088Config_s {
    // TODO: Add SPI interface here
    // TODO: Add I2C interface here
    
    void (*delayUs)(uint32_t us, void *intfPtr);

    struct accelCfg_s {
        uint8_t range;
        uint8_t odr;
        uint8_t bw;
        uint8_t powerMode;

        inline accelCfg_s(uint8_t _range = BMI088_ACCEL_RANGE_3G,
                   uint8_t _odr = BMI08X_ACCEL_ODR_1600_HZ,
                   uint8_t _bw = BMI08X_ACCEL_BW_OSR4,
                   uint8_t _powerMode = BMI08X_ACCEL_PM_ACTIVE)
                : range(_range), odr(_odr), bw(_bw), powerMode(_powerMode){}
    } accelCfg;

    void (*accelCsHigh)();
    void (*accelCsLow)();

    struct gyroCfg_s {
        uint8_t range;
        uint8_t odr;
        uint8_t bw;
        uint8_t powerMode;

        inline gyroCfg_s(uint8_t _range = BMI08X_GYRO_RANGE_1000_DPS,
                   uint8_t _odr = BMI08X_GYRO_BW_230_ODR_2000_HZ,
                   uint8_t _bw = BMI08X_GYRO_BW_230_ODR_2000_HZ,
                   uint8_t _powerMode = BMI08X_GYRO_PM_NORMAL)
                : range(_range), odr(_odr), bw(_bw), powerMode(_powerMode){}
    } gyroCfg;

    void (*gyroCsHigh)();
    void (*gyroCsLow)();

    bmi08x_intf intf = BMI08X_SPI_INTF; // default SPI
    uint8_t maxReadWriteLength = 64u; // default 64 bytes

    // 构造函数 注册函数指针
    BMI088Config_s(void (*_delayUs)(uint32_t us, void *intfPtr), void (*_accelCsHigh)(),
                   void (*_accelCsLow)(), void (*_gyroCsHigh)(),
                   void (*_gyroCsLow)(),
                   bmi08x_intf _intf = BMI08X_SPI_INTF)
            : delayUs(_delayUs)
            , accelCsHigh(_accelCsHigh)
            , accelCsLow(_accelCsLow)
            , gyroCsHigh(_gyroCsHigh)
            , gyroCsLow(_gyroCsLow)
            , intf(_intf)
    {
    }
};

struct BMI088Data {
    // m/s^2  
    int16_t rawAccelX;
    int16_t rawAccelY;
    int16_t rawAccelZ;
    // rad/s
    int16_t rawGyroX;
    int16_t rawGyroY;
    int16_t rawGyroZ;
    // Celsius
    float temperature;
};

class BMI088 {
public:
    BMI088(BMI088Config_s _config);
    ~BMI088();

    BMI088Data data;

    int8_t init();

    int8_t getData();

    int8_t enableInterrupt();
    int8_t disableInterrupt();

    inline void setAccelConfig(uint8_t _odr, uint8_t _range, uint8_t _bw,
                     uint8_t _powerMode)
    {
        this->config_.accelCfg.odr = _odr;
        this->config_.accelCfg.range = _range;
        this->config_.accelCfg.bw = _bw;
        this->config_.accelCfg.powerMode = _powerMode;
    }

    inline void setGyroConfig(uint8_t _odr, uint8_t _range, uint8_t _bw,
                    uint8_t _powerMode)
    {
        this->config_.gyroCfg.odr = _odr;
        this->config_.gyroCfg.range = _range;
        this->config_.gyroCfg.bw = _bw;
        this->config_.gyroCfg.powerMode = _powerMode;
    }

    inline void reset() { init(); }
    inline void reset(BMI088Config_s _config)
    {
        this->config_ = _config;
        init();
    }

    template<typename... Args>
    inline void log(const char *_type, const char *_color, const char *_format,
                    Args... _args)
    {
        // TODO: Implement logging function
    }

private:
    uint8_t rwByte(uint8_t _byte);
    uint8_t rByte(uint8_t _regAddr, uint8_t *_data, uint32_t _len,
                  void *_intfPtr);
    uint8_t wByte(uint8_t _regAddr, uint8_t *_data, uint32_t _len,
                  void *_intfPtr);
    int8_t interfaceInit(struct bmi08x_dev *_bmi08x, uint8_t _intfType,
                         enum bmi08x_variant _variant);

    BMI088Config_s config_;

    /* Lib variable required*/
    struct bmi08x_dev bmi08xDev_;
    struct bmi08x_sensor_data bmi08xAccel_;
    struct bmi08x_sensor_data bmi08xGyro_;
    struct bmi08x_accel_int_channel_cfg accelIntConfig_;
    struct bmi08x_gyro_int_channel_cfg gyroIntConfig_;
    struct bmi08x_err_reg err_;

    /*! Variable that holds the I2C device address or SPI chip selection for accel or gyro*/
    uint8_t accDevAdd_ = 0u;
    uint8_t gyroDevAdd_ = 0u;
};