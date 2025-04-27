#include "BMI088.hpp"

#include "./DrvLib/bmi08x.h"

#define BMI088_SPI_ACCEL 0
#define BMI088_SPI_GYRO 1

uint8_t BMI088::rwByte(uint8_t _byte)
{
    uint8_t _readByte = 0;
    // TODO: Implement SPI transfer here
    return _readByte;
}

uint8_t BMI088::rByte(uint8_t _regAddr, uint8_t *_data, uint32_t _len,
                      void *_intfPtr)
{
    uint8_t devAddr = *(uint8_t *)_intfPtr;
    if (devAddr == BMI088_SPI_ACCEL) { // accel
        this->config_.accelCsLow();
        this->config_.delayUs(2, _intfPtr);
        rwByte(_regAddr); // Read command
        while (_len) {
            *_data = rwByte(0x55);
            _data++;
            _len--;
        }
        this->config_.accelCsHigh();
        this->config_.delayUs(2, _intfPtr);
    } else { // gyro
        this->config_.gyroCsLow();
        this->config_.delayUs(2, _intfPtr);
        rwByte(_regAddr); // Read command
        while (_len) {
            *_data = rwByte(0x55);
            _data++;
            _len--;
        }
        this->config_.gyroCsHigh();
        this->config_.delayUs(2, _intfPtr);
    }
    return 0;
}

uint8_t BMI088::wByte(uint8_t _regAddr, uint8_t *_data, uint32_t _len,
                      void *_intfPtr)
{
    uint8_t devAddr = *(uint8_t *)_intfPtr;
    if (devAddr == BMI088_SPI_ACCEL) { // accel
        this->config_.accelCsLow();
        this->config_.delayUs(2, _intfPtr);
        rwByte(_regAddr); // Write command
        while (_len) {
            rwByte(*_data);
            _data++;
            _len--;
        }
        this->config_.accelCsHigh();
        this->config_.delayUs(2, _intfPtr);
    } else { // gyro
        this->config_.gyroCsLow();
        this->config_.delayUs(2, _intfPtr);
        rwByte(_regAddr); // Write command
        while (_len) {
            rwByte(*_data);
            _data++;
            _len--;
        }
        this->config_.gyroCsHigh();
        this->config_.delayUs(2, _intfPtr);
    }
    return 0;
}

int8_t BMI088::interfaceInit(struct bmi08x_dev *_bmi08x, uint8_t _intfType,
                             enum bmi08x_variant _variant)
{
    int8_t rslt = BMI08X_OK;
    if (_bmi08x != nullptr) {
        if(_intfType == BMI08X_I2C_INTF) {
            this->log("INFO", "green", "BMI088 I2C interface init\n");
            this->accDevAdd_ = BMI08X_ACCEL_I2C_ADDR_PRIMARY;
            this->gyroDevAdd_ = BMI08X_GYRO_I2C_ADDR_PRIMARY;
            _bmi08x->intf = BMI08X_I2C_INTF;
        } else if (_intfType == BMI08X_SPI_INTF) {
            this->log("INFO", "green", "BMI088 SPI interface init\n");
            this->accDevAdd_ = BMI088_SPI_ACCEL;
            this->gyroDevAdd_ = BMI088_SPI_GYRO;
            _bmi08x->intf = BMI08X_SPI_INTF;
        } else {
            rslt = BMI08X_E_INVALID_CONFIG;
        }
        /* Selection of bmi085 or bmi088 sensor variant */
        _bmi08x->variant = _variant;

        /* Assign accel device address to accel interface pointer */
        _bmi08x->intf_ptr_accel = &this->accDevAdd_;

        /* Assign gyro device address to gyro interface pointer */
        _bmi08x->intf_ptr_gyro = &this->gyroDevAdd_;

        /* Configure delay in microseconds */
        _bmi08x->delay_us = this->config_.delayUs;

        /* Configure max read/write length (in bytes) ( Supported length depends on
        * target machine) */
        _bmi08x->read_write_len = this->config_.maxReadWriteLength;
    } else {
        rslt = BMI08X_E_NULL_PTR;
    }
    return rslt;
}

int8_t BMI088::init()
{
    int8_t rslt = BMI08X_OK;
    rslt = this->interfaceInit(&this->bmi08xDev_, this->config_.intf,
                               BMI088_VARIANT);
    if (rslt == BMI08X_OK) {
        rslt = bmi08a_soft_reset(&this->bmi08xDev_);
        if( rslt == BMI08X_OK) {
            rslt = bmi08a_init(&this->bmi08xDev_);
        }
    }
    if (rslt == BMI08X_OK) {
        rslt = bmi08g_soft_reset(&this->bmi08xDev_);
        if( rslt == BMI08X_OK) {
            rslt = bmi08g_init(&this->bmi08xDev_);
        }
    }
    this->bmi08xDev_.accel_cfg.odr = this->config_.accelCfg.odr;
    this->bmi08xDev_.accel_cfg.range = this->config_.accelCfg.range;
    this->bmi08xDev_.accel_cfg.bw = this->config_.accelCfg.bw;
    this->bmi08xDev_.accel_cfg.power = this->config_.accelCfg.powerMode;

    this->bmi08xDev_.gyro_cfg.odr = this->config_.gyroCfg.odr;
    this->bmi08xDev_.gyro_cfg.range = this->config_.gyroCfg.range;
    this->bmi08xDev_.gyro_cfg.bw = this->config_.gyroCfg.bw;
    this->bmi08xDev_.gyro_cfg.power = this->config_.gyroCfg.powerMode;

    if(rslt == BMI08X_OK) {
        rslt = bmi08a_set_power_mode(&this->bmi08xDev_);
        if (rslt != BMI08X_OK) {
            this->log("ERROR", "red", "BMI088 set power mode failed\n");
            return rslt;
        }
        rslt = bmi08a_set_meas_conf(&this->bmi08xDev_);
        if (rslt != BMI08X_OK) {
            this->log("ERROR", "red", "BMI088 set meas conf failed\n");
            return rslt;
        }
        rslt = bmi08a_get_power_mode(&this->bmi08xDev_);
        if (rslt != BMI08X_OK ||
            this->bmi08xDev_.accel_cfg.power != BMI08X_ACCEL_PM_ACTIVE) {
            this->log("ERROR", "red", "BMI088 get power mode failed\n");
            return -47; // IMU_CONF_ERR = -47
        }
        rslt = bmi08g_set_power_mode(&this->bmi08xDev_);
        if (rslt != BMI08X_OK) {
            this->log("ERROR", "red", "BMI088 set power mode failed\n");
            return rslt;
        }
        rslt = bmi08g_set_meas_conf(&this->bmi08xDev_);
        if (rslt != BMI08X_OK) {
            this->log("ERROR", "red", "BMI088 set meas conf failed\n");
            return rslt;
        }
        rslt = bmi08g_get_power_mode(&this->bmi08xDev_);
        if (rslt != BMI08X_OK ||
            this->bmi08xDev_.gyro_cfg.power != BMI08X_GYRO_PM_NORMAL) {
            this->log("ERROR", "red", "BMI088 get power mode failed\n");
            return -24; // IMU_ACCEL_ERR = -24
        }    
    }
    rslt = bmi08a_get_error_status(&this->err_, &this->bmi08xDev_);
    if (rslt == BMI08X_OK) {
        if (this->err_.err_code != 0 || this->err_.fatal_err != 0) {
            this->log("ERROR", "red", "BMI088 error status failed\n");
            return -24;
        }
    }
    this->log("INFO", "green", "BMI088 init done\n");
    return rslt;
}

int8_t BMI088::enableInterrupt()
{
    int8_t rslt = BMI08X_OK;
    uint8_t data = 0;

    /* Set accel interrupt pin configuration */
    this->accelIntConfig_.int_channel = BMI08X_INT_CHANNEL_1;
    this->accelIntConfig_.int_type = BMI08X_ACCEL_INT_DATA_RDY;
    this->accelIntConfig_.int_pin_cfg.output_mode = BMI08X_INT_MODE_PUSH_PULL;
    this->accelIntConfig_.int_pin_cfg.lvl = BMI08X_INT_ACTIVE_HIGH;
    this->accelIntConfig_.int_pin_cfg.enable_int_pin = BMI08X_ENABLE;

    /* Enable accel data ready interrupt channel */
    rslt = bmi08a_set_int_config((const struct bmi08x_accel_int_channel_cfg *)&this->accelIntConfig_,
                                 &this->bmi08xDev_);
    if (rslt != BMI08X_OK) {
        /* Set gyro interrupt pin configuration */
        this->gyroIntConfig_.int_channel = BMI08X_INT_CHANNEL_3;
        this->gyroIntConfig_.int_type = BMI08X_GYRO_INT_DATA_RDY;
        this->gyroIntConfig_.int_pin_cfg.output_mode =
                BMI08X_INT_MODE_PUSH_PULL;
        this->gyroIntConfig_.int_pin_cfg.lvl = BMI08X_INT_ACTIVE_HIGH;
        this->gyroIntConfig_.int_pin_cfg.enable_int_pin = BMI08X_ENABLE;
        /* Enable gyro data ready interrupt channel */
        rslt = bmi08g_set_int_config((const struct bmi08x_gyro_int_channel_cfg
                                              *)&this->gyroIntConfig_,
                                     &this->bmi08xDev_);
        rslt = bmi08g_get_regs(BMI08X_REG_GYRO_INT3_INT4_IO_MAP, &data, 1,
                                     &this->bmi08xDev_);
    }
    return rslt;
}

int8_t BMI088::disableInterrupt()
{
    int8_t rslt;

    /* Set accel interrupt pin configuration */
    this->accelIntConfig_.int_channel = BMI08X_INT_CHANNEL_1;
    this->accelIntConfig_.int_type = BMI08X_ACCEL_INT_DATA_RDY;
    this->accelIntConfig_.int_pin_cfg.output_mode = BMI08X_INT_MODE_PUSH_PULL;
    this->accelIntConfig_.int_pin_cfg.lvl = BMI08X_INT_ACTIVE_HIGH;
    this->accelIntConfig_.int_pin_cfg.enable_int_pin = BMI08X_DISABLE;

    /* Disable accel data ready interrupt channel */
    rslt = bmi08a_set_int_config(
            (const struct bmi08x_accel_int_channel_cfg *)&this->accelIntConfig_,
            &this->bmi08xDev_);
    if (rslt != BMI08X_OK) {
        /* Set gyro interrupt pin configuration */
        this->gyroIntConfig_.int_channel = BMI08X_INT_CHANNEL_3;
        this->gyroIntConfig_.int_type = BMI08X_GYRO_INT_DATA_RDY;
        this->gyroIntConfig_.int_pin_cfg.output_mode =
                BMI08X_INT_MODE_PUSH_PULL;
        this->gyroIntConfig_.int_pin_cfg.lvl = BMI08X_INT_ACTIVE_HIGH;
        this->gyroIntConfig_.int_pin_cfg.enable_int_pin = BMI08X_DISABLE;

        /* Disable gyro data ready interrupt channel */
        rslt = bmi08g_set_int_config(
                (const struct bmi08x_gyro_int_channel_cfg *)&this->gyroIntConfig_,
                &this->bmi08xDev_);
    }
    return rslt;
}

int8_t BMI088::getData()
{
    int8_t rslt = BMI08X_OK;
    int32_t temp;
    rslt = bmi08a_get_data(&this->bmi08xAccel_, &this->bmi08xDev_);
    if (rslt == BMI08X_OK) {
        this->data.rawAccelX = this->bmi08xAccel_.x;
        this->data.rawAccelY = this->bmi08xAccel_.y;
        this->data.rawAccelZ = this->bmi08xAccel_.z;
    }
    rslt = bmi08g_get_data(&this->bmi08xGyro_, &this->bmi08xDev_);
    if (rslt == BMI08X_OK) {
        this->data.rawGyroX = this->bmi08xGyro_.x;
        this->data.rawGyroY = this->bmi08xGyro_.y;
        this->data.rawGyroZ = this->bmi08xGyro_.z;
    }
    rslt = bmi08a_get_sensor_temperature(&this->bmi08xDev_, &temp);
    if(rslt == BMI08X_OK) {
        this->data.temperature = static_cast<float>(temp) / 1000.0f;
    }
    return rslt;
}
