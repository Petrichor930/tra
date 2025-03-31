#pragma once

#include "../../Base/MotorBase.hpp"

#include "CommonMacros.hpp"

#include <cstdint>
#include <cstring>

#include <unordered_map>

namespace PINYMOTOR {

enum class DMMotorRegId_e {
    DM_REG_UV_Value  = 0u, // 低压保护值	RW	(10.0,3.4E38]	float
    DM_REG_KT_Value  = 1u, // 扭矩系数	RW	[0.0,3.4E38]	float
    DM_REG_OT_Value  = 2u, // 过温保护值	RW	[80.0,200)	float
    DM_REG_OC_Value  = 3u, // 过流保护值	RW	(0.0,1.0)	float
    DM_REG_ACC       = 4u, // 加速度	RW	(0.0,3.4E38)	float
    DM_REG_DEC       = 5u, // 减速度	RW	[-3.4E38,0.0)	float
    DM_REG_MAX_SPD   = 6u, // 最大速度	RW	(0.0,3.4E38]	float
    DM_REG_MST_ID    = 7u, // 反馈ID	RW	[0,0x7FF]	uint32
    DM_REG_ESC_ID    = 8u, // 接收ID	RW	[0,0x7FF]	uint32
    DM_REG_TIMEOUT   = 9u, // 超时警报时间	RW	[0,2^32-1]	uint32
    DM_REG_CTRL_MODE = 10u, // 控制模式	RW	[1,4]	uint32
    DM_REG_Damp      = 11u, // 电机粘滞系数	RO	/	float
    DM_REG_Inertia   = 12u, // 电机转动惯量	RO	/	float
    DM_REG_hw_ver    = 13u, // 保留	RO	/	uint32
    DM_REG_sw_ver    = 14u, // 软件版本号	RO	/	uint32
    DM_REG_SN        = 15u, // 保留	RO	/	uint32
    DM_REG_NPP       = 16u, // 电机极对数	RO	/	uint32
    DM_REG_Rs        = 17u, // 电机相电阻	RO	/	float
    DM_REG_Ls        = 18u, // 电机相电感	RO	/	float
    DM_REG_Flux      = 19u, // 电机磁链值	RO	/	float
    DM_REG_Gr        = 20u, // 齿轮减速比	RO	/	float
    DM_REG_PMAX      = 21u, // 位置映射范围	RW	(0.0,3.4E38]	float
    DM_REG_VMAX      = 22u, // 速度映射范围	RW	(0.0,3.4E38]	float
    DM_REG_TMAX      = 23u, // 扭矩映射范围	RW	(0.0,3.4E38]	float
    DM_REG_I_BW      = 24u, // 电流环控制带宽	RW	[100.0,10000.0]	float
    DM_REG_KP_ASR    = 25u, // 速度环Kp	RW	[0.0,3.4E38]	float
    DM_REG_KI_ASR    = 26u, // 速度环Ki	RW	[0.0,3.4E38]	float
    DM_REG_KP_APR    = 27u, // 位置环Kp	RW	[0.0,3.4E38]	float
    DM_REG_KI_APR    = 28u, // 位置环Ki	RW	[0.0,3.4E38]	float
    DM_REG_OV_Value  = 29u, // 过压保护值	RW	TBD	float
    DM_REG_GREF      = 30u, // 齿轮力矩效率	RW	(0.0,1.0]	float
    DM_REG_Deta      = 31u, // 速度环阻尼系数	RW	[1.0,30.0]	float
    DM_REG_V_BW      = 32u, // 速度环滤波带宽	RW	(0.0,500.0)	float
    DM_REG_IQ_c1     = 33u, // 电流环增强系数	RW	[100.0,10000.0]	float
    DM_REG_VL_c1     = 34u, // 速度环增强系数	RW	(0.0,10000.0]	float
    DM_REG_can_br    = 35u, // CAN波特率代码	RW	[0,4]	uint32
    DM_REG_sub_ver   = 36u, // 子版本号	RO	/	uint32
    DM_REG_u_off     = 50u, // u相偏置	RO	　	float
    DM_REG_v_off     = 51u, // v相偏置	RO	　	float
    DM_REG_k1        = 52u, // 补偿因子1	RO	　	float
    DM_REG_k2        = 53u, // 补偿因子2	RO	　	float
    DM_REG_m_off     = 54u, // 角度偏移	RO	　	float
    DM_REG_dir       = 55u, // 方向	RO	　	float
    DM_REG_p_m       = 80u, // 电机位置	RO	　	float
    DM_REG_xout      = 81u, // 输出轴位置	RO	　	float   
};
// 寄存器列表


#pragma pack(push, 1)
typedef struct _DMMotorReg_s {
    DMMotorRegId_e regId;
    uint8_t dat[4];
    bool isWrite;
    bool isRead;
    bool isStorage;
} DMMotorReg_s;

typedef enum _DMMotorErrorCode_e{
    MotorDisable = 0x0u,
    MotorEnable = 0x1u,
    OverVoltage = 0x8u,
    LowVoltage = 0x9u,
    OverCurrent = 0xAu,
    MosOverHeat = 0xBu,
    RotorOverHeat = 0xCu,
    CommunicationLoss = 0xDu,
    Overload = 0xEu,
} DMMotorErrorCode_e;

typedef struct _DMMotorFeedback_s {
    uint8_t ID : 4;
    DMMotorErrorCode_e errorCode : 4;
    uint16_t rawScale : 14;
    uint16_t rawVel : 12;
    uint16_t torque : 12;
    uint8_t mosTemperature : 8;
    uint8_t rotorTemperature : 8;
} DMMotorFeedback_s;

typedef struct _DMMITMsg_s {
    int16_t exptScale : 16;
    int16_t exptVel : 12;
    int16_t Kp : 12;
    int16_t Kd : 12;
    int16_t torqueOffset : 12;
} DMMITMsg_s;

typedef struct _DMEMITMsg_s {
    float exptScale;
    uint16_t exptVelX100 : 16;
    uint16_t imaxX10000 : 16;
} DMEMITMsg_s;
typedef struct _DMPDESVDESMsg_s {
    float exptScale;
    float exptVel;
} DMPDESVDESMsg_s;

typedef struct _DMVDESMsg_s {
    float exptVel;
    float reserved;
} DMVDESMsg_s;
#pragma pack(pop)

typedef struct _DMMotorStats_s {
    float PMax;
    float VMax;
    float TMax;
    float MITKpMax;
    float MITKdMax;
    float currCodeSpan; 
    float currRated;    // A
    float torqRated;    // Nm
    float currMax;      // A
    float torqMax;      // Nm
    float torqConstant; // Nm/A

    _DMMotorStats_s& operator=(const _DMMotorStats_s& _other) {
        if (this != &_other) {
            PMax = _other.PMax;
            VMax = _other.VMax;
            TMax = _other.TMax;
            MITKpMax = _other.MITKpMax;
            MITKdMax = _other.MITKdMax;
            currCodeSpan = _other.currCodeSpan;
            currRated = _other.currRated;
            torqRated = _other.torqRated;
            currMax = _other.currMax;
            torqMax = _other.torqMax;
            torqConstant = _other.torqConstant;
        }
        return *this;
    }
} DMMotorStats_s;

template <typename Derived> class DMMotor : public MotorBase<Derived> {
    // 定义一个模板类DMMotor，继承自MotorBase类，并使用Derived作为模板参数
    using Base = MotorBase<Derived>;

protected:
    // 定义一个保护成员变量stats，类型为DMMotorStats_s
    DMMotorStats_s stats_;

    // 正在操作的寄存器对象
    std::unordered_map<DMMotorRegId_e,DMMotorReg_s*> regObjList_;

    // 定义两个保护成员变量MITKp和MITKd，类型为float
    float MITKp_ = 0;
    float MITKd_ = 0;

    // 达妙电机反馈自身状态
    DMMotorErrorCode_e errorCode_;

    // 定义一个Cmd_s结构体，包含三个成员变量torq、speed和pos，类型均为float
    typedef struct _Cmd_s {
        float torq;
        float speed;
        float pos;
    } Cmd_s;
    // 定义一个Cmd_s类型的成员变量cmd
    Cmd_s cmd_;

public:
    // 构造函数，接受一个InitConfig_s类型的参数_config，并调用基类的构造函数
    DMMotor(InitConfig_s _config) : Base(_config) {}

    // 重写电机属性
    void overrideStats(const _DMMotorStats_s& _newStats)
    {
        // 将新的电机属性赋值给stats
        stats_ = _newStats;
    }

    // 定义三个公有成员函数，分别用于设置torq、speed和pos
    MotorTypedef_e _cmd_(MotorCmdType_e _cmd, float _cmdData)
    {
        switch (_cmd) {
        case MotorCmdType_e::SET_SPD:
            cmd_.speed = _cmdData;
            break;
        case MotorCmdType_e::SET_POS:
            cmd_.pos = _cmdData;
            break;
        case MotorCmdType_e::SET_TORQ:
            cmd_.torq = _cmdData;
            break;
        default:
            return MotorTypedef_e::ERR;
        };
        return MotorTypedef_e::OK;
    }

    uint16_t canId() { return this->model_.txBaseId + this->offsetId_; }
    uint16_t masterId() { return this->model_.rxBaseId + this->offsetId_; }

    uint16_t _uid_() { return masterId(); } // TODO:

    // 定义两个公有成员函数，分别用于解析接收到的数据和控制电机
    MotorTypedef_e _parse_(uint8_t *_rxBuf)
    {
        // 先处理非常规数据反馈的帧
        if (_rxBuf[0] == static_cast<uint8_t>(canId()) &&
            _rxBuf[1] == static_cast<uint8_t>(canId() >> 8)) {
            uint32_t rawDat = (_rxBuf[7] << 24) | (_rxBuf[6] << 16) | (_rxBuf[5] << 8) | _rxBuf[4];
            auto it = regObjList_.find(static_cast<DMMotorRegId_e>(_rxBuf[3]));
            if (_rxBuf[2] == 0x33) {
                // 读反馈
                (*it).second->isRead = true;
                memcpy(&(*it).second->dat, &rawDat, 4);
            } else if (_rxBuf[2] == 0x55) {
                // 写反馈
                (*it).second->isWrite = true;
            } else if (_rxBuf[2] == 0xAA) {
                // 存储反馈
                (*it).second->isStorage = true;
            } else {
                // TODO:
                return MotorTypedef_e::ERR;
            }
        } else {
            DMMotorFeedback_s fb;
            fb.ID = _rxBuf[0] & 0x0F;
            fb.errorCode = static_cast<DMMotorErrorCode_e>(_rxBuf[0] >> 4);
            fb.rawScale = (_rxBuf[1] << 8) | _rxBuf[2];
            fb.rawVel = (_rxBuf[3] << 4) | (_rxBuf[4] >> 4);
            fb.torque = ((_rxBuf[4] & 0xF) << 8 | _rxBuf[5]);
            fb.mosTemperature = _rxBuf[6];
            fb.rotorTemperature = _rxBuf[7];

            errorCode_ = fb.errorCode;

            this->data_.rawScale = fb.rawScale;

            this->data_.spdRadps = uint2float(fb.rawVel, -stats_.VMax, stats_.VMax, 12) / this->RR();
            this->data_.spdRpm = radps2rpm(this->data_.spdRadps);
            this->data_.torq = uint2float(fb.torque, -stats_.TMax, stats_.TMax, 12) * this->RR();

            this->data_.curr = this->data_.torq / stats_.torqConstant;

            this->data_.tempture = fb.mosTemperature;

            float angDiff =
                    getMinorArc(this->data_.rawScale, this->data_.lastRawScale, this->span()) * 2 *
                    PI / (this->span() * this->RR());
            if (this->globalState_ == GlobalState_e::OFFLINE &&
                this->data_.lastRawScale != this->data_.rawScale) {
                this->globalState_ = GlobalState_e::ONLINE;
                angDiff = 0;
            }
            this->data_.lastRawScale = this->data_.rawScale;

            this->data_.MultipCirAng += angDiff;
            this->data_.singleCirAng += angDiff;
            this->data_.singleCirAng = rangeMap(this->data_.singleCirAng, 0, 2 * PI);
        }
        return MotorTypedef_e::OK;
    }
    MotorTypedef_e _ctrl_()
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        typedef union {
            DMMITMsg_s msgMIT;
            DMPDESVDESMsg_s msgPDESVDES;
            DMVDESMsg_s msgVDES;
            DMEMITMsg_s msgEMIT;
        } DMMsg_u;
        DMMsg_u DMMsg;
        uint8_t txBuf[8] = { 0 };
        bool isMIT = false;
        switch (this->workMode_) {
        case WorkMode_e::QUAD_CURR:
            // error
            break;
        case WorkMode_e::QUAD_VOLT:
            // error
            break;
        case WorkMode_e::MIT_TT:
            DMMsg.msgMIT.torqueOffset = float2uint(cmd_.torq, -stats_.TMax, stats_.TMax, 12);
            DMMsg.msgMIT.Kp = 0;
            DMMsg.msgMIT.Kd = 0;
            isMIT = true;
            break;
        case WorkMode_e::MIT_VDESPDES:
            DMMsg.msgMIT.exptScale = float2uint(cmd_.pos, -stats_.PMax, stats_.PMax, 16);
            DMMsg.msgMIT.exptVel = float2uint(cmd_.speed, -stats_.VMax, stats_.VMax, 12);
            DMMsg.msgMIT.Kd = float2uint(this->MITKd_, -stats_.MITKdMax, stats_.MITKdMax, 12);
            DMMsg.msgMIT.Kp = float2uint(this->MITKp_, -stats_.MITKpMax, stats_.MITKpMax, 12);
            DMMsg.msgMIT.torqueOffset = float2uint(cmd_.torq, -stats_.TMax, stats_.TMax, 12);
            isMIT = true;
            break;
        case WorkMode_e::MIT_VDES:
            DMMsg.msgMIT.exptVel = float2uint(cmd_.speed, -stats_.VMax, stats_.VMax, 12);
            DMMsg.msgMIT.Kd = float2uint(this->MITKd_, -stats_.MITKdMax, stats_.MITKdMax, 12);
            DMMsg.msgMIT.Kp = 0;
            DMMsg.msgMIT.torqueOffset = float2uint(cmd_.torq, -stats_.TMax, stats_.TMax, 12);
            isMIT = true;
            break;
        case WorkMode_e::PDESVDES:
            DMMsg.msgPDESVDES.exptScale = cmd_.pos;
            DMMsg.msgPDESVDES.exptVel = cmd_.speed;
            memcpy(txBuf, &DMMsg.msgPDESVDES.exptScale, 4);
            memcpy(&txBuf[4], &DMMsg.msgPDESVDES.exptVel, 4);
            // TODO:cantransmit data
            break;
        case WorkMode_e::VDES:
            DMMsg.msgVDES.exptVel = cmd_.speed;
            memcpy(txBuf, &DMMsg.msgVDES.exptVel, 4);
            // TODO:cantransmit data
            break;
        case WorkMode_e::EMIT:
            DMMsg.msgEMIT.exptScale = cmd_.pos;
            DMMsg.msgEMIT.exptVelX100 =
                    static_cast<uint16_t>(((cmd_.speed < 0) ? -cmd_.speed : cmd_.speed) * 100.f);
            DMMsg.msgEMIT.imaxX10000 = static_cast<uint16_t>(((cmd_.torq < 0) ? -cmd_.torq : cmd_.torq) /
                                                stats_.torqConstant / stats_.currMax * stats_.currCodeSpan);
            float f = DMMsg.msgEMIT.exptScale;
            memcpy(txBuf, &f, 4);
            txBuf[4] = static_cast<uint8_t>((DMMsg.msgEMIT.exptVelX100) >> 8);
            txBuf[5] = static_cast<uint8_t>(DMMsg.msgEMIT.exptVelX100);
            txBuf[6] = static_cast<uint8_t>((DMMsg.msgEMIT.imaxX10000) >> 8);
            txBuf[7] = static_cast<uint8_t>(DMMsg.msgEMIT.imaxX10000);
            // TODO:cantransmit data
            break;
        }
        if (isMIT) {
            txBuf[0] = static_cast<uint8_t>((DMMsg.msgMIT.exptScale & 0xFF00) >> 8);
            txBuf[1] = static_cast<uint8_t>(DMMsg.msgMIT.exptScale & 0x00FF);
            txBuf[2] = static_cast<uint8_t>((DMMsg.msgMIT.exptVel & 0x0FF0) >> 4);
            txBuf[3] = static_cast<uint8_t>((DMMsg.msgMIT.exptVel & 0x000F) << 4 | ((DMMsg.msgMIT.Kp & 0x0FF0) >> 8));
            txBuf[4] = static_cast<uint8_t>(DMMsg.msgMIT.Kp & 0x000F);
            txBuf[5] = static_cast<uint8_t>((DMMsg.msgMIT.Kd & 0x0FF0) >> 4);
            txBuf[6] = static_cast<uint8_t>((DMMsg.msgMIT.Kd & 0x000F) << 4 | ((DMMsg.msgMIT.torqueOffset & 0x0F00) >> 8));
            txBuf[7] = static_cast<uint8_t>(DMMsg.msgMIT.torqueOffset & 0x00FF);
            // TODO:cantransmit data
        }
        return rslt;
    }

    // 定义两个公有成员函数，分别用于设置MITKp和MITKd
    void setMITKp(float _kp) { MITKp_ = _kp; }
    void setMITKd(float _kd) { MITKd_ = _kd; }

    // 定义一个公有成员函数enable，用于使能电机
    MotorTypedef_e enable()
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        // 定义一个8字节的数组enableCmdPack，用于存储使能命令
        uint8_t enableCmdPack[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC };
        // rslt |= canTransmit(this->pCanhandle, this->model.txBaseId + this->offsetId_, enableCmdPack, 8);
        // TODO:调用canTransmit函数发送使能命令
        return rslt;
    }

    // 定义一个公有成员函数disable，用于禁用电机
    MotorTypedef_e disable()
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        // 定义一个8字节的数组disableCmdPack，用于存储禁用命令
        uint8_t disableCmdPack[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD };
        // rslt |= canTransmit(this->pCanhandle, this->model.txBaseId + this->offsetId_, disableCmdPack, 8);
        // TODO:调用canTransmit函数发送禁用命令
        return rslt;
    };
    // 定义两个公有成员函数writeReg和readReg，用于读写寄存器
    MotorTypedef_e registerReg(DMMotorReg_s *_regObj)
    {
        if (_regObj != nullptr) {
            regObjList_.insert({_regObj->regId, _regObj});
            return MotorTypedef_e::ERR;
        }
        return MotorTypedef_e::OK;
    }
    MotorTypedef_e cancelReg(DMMotorRegId_e regId)
    {
        regObjList_.erase(regId);
        return MotorTypedef_e::OK;
    }
    MotorTypedef_e writeReg(DMMotorRegId_e _regId, uint8_t dat[4])
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        // 报文ID : 0x7FF, D0 : CANID_L, D1 : CANID_H, D2 : 0x55, D3 : RID, D4 : dat1,
        // D5 : dat2, D6 : dat3, D7 : dat4
        auto it = regObjList_.find(_regId);
        if (it != regObjList_.end()) {
            (*it).second->isWrite = false;
            uint16_t id = static_cast<uint16_t>(canId());
            uint8_t writeTxBuffer[8] = //
                    { static_cast<uint8_t>(id),
                      static_cast<uint8_t>(id >> 8),
                      0x55,
                      static_cast<uint8_t>(_regId),
                      dat[0],
                      dat[1],
                      dat[2],
                      dat[3] };
            // TODO:cantransmit data
        }
        return rslt;
    }
    MotorTypedef_e readReg(DMMotorRegId_e _regId)
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        // 报文ID : 0x7FF, D0 : CANID_L, D1 : CANID_H, D2 : 0x33, D3 : RID, D4 : 0x00,
        // D5 : 0x00, D6 : 0x00, D7 : 0x00
        auto it = regObjList_.find(_regId);
        if (it != regObjList_.end()) {
            (*it).second->isRead = false;
            uint16_t id = static_cast<uint16_t>(canId());
            uint8_t readTxBuffer[8] = { static_cast<uint8_t>(id),
                                        static_cast<uint8_t>(id >> 8),
                                        0x33,
                                        static_cast<uint8_t>(_regId),
                                        0x00,
                                        0x00,
                                        0x00,
                                        0x00 };
            // TODO:cantransmit data
        }
        return rslt;
    }
    // 定义一个公有成员函数storageReg，用于存储寄存器
    MotorTypedef_e storageReg(DMMotorRegId_e _regId)
    {
        MotorTypedef_e rslt = MotorTypedef_e::OK;
        // 报文ID : 0x7FF, D0 : CANID_L, D1 : CANID_H, D2 : 0xAA, D3 : RID, D4 : 0x00,
        // D5 : 0x00, D6 : 0x00, D7 : 0x00
        auto it = regObjList_.find(_regId);
        if (it != regObjList_.end()) {
            (*it).second->isStorage = false;
            uint16_t id = static_cast<uint16_t>(canId());
            uint8_t storageTxBuf[8] = { static_cast<uint8_t>(id),
                                        static_cast<uint8_t>(id >> 8),
                                        0xAA,
                                        static_cast<uint8_t>(_regId),
                                        0x00,
                                        0x00,
                                        0x00,
                                        0x00 };
            // TODO:cantransmit data
        }
        return rslt;
    }
};
}