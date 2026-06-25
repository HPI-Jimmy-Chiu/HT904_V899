using System;

namespace HT9045.HardwareAbstraction
{
    // Mirrors the BCB6 C++ HTMotor base class (Motor/HTMotor.h).
    // Method names are kept identical to the C++ virtuals so the team can
    // reverse-look-up. Return convention follows the C++ source:
    //   - InitMotor returns int (0 = ok in HTMotor::InitMotor default).
    //   - MoveToPos / HomeObject / HomeFlag / GetAlarm / MotionDone return bool.
    //   - ReadPos returns the current pulse position (int).
    public interface IMotorPort
    {
        // virtual int InitMotor(int IoAddress);
        int InitMotor(int IoAddress);

        // virtual bool MoveToPos(int Tar);
        bool MoveToPos(int Tar);

        // virtual int ReadPos();
        int ReadPos();

        // virtual bool HomeObject();
        bool HomeObject();

        // virtual bool HomeFlag(void);
        bool HomeFlag();

        // virtual void Stop();
        void Stop();

        // virtual bool MotionDone();
        bool MotionDone();

        // virtual bool GetAlarm(void);
        bool GetAlarm();

        // virtual void SetSpeed(unsigned int x, bool bSetJog = false);
        void SetSpeed(uint x, bool bSetJog);

        // virtual void ServerOnOff(bool bStatus);   // (BCB6 spelling kept verbatim)
        void ServerOnOff(bool bStatus);

        // virtual void SetSoftLimit(int iPLimit, int iNLimit);
        void SetSoftLimit(int iPLimit, int iNLimit);

        // virtual bool JogP();
        bool JogP();

        // virtual bool JogN();
        bool JogN();

        // virtual void ScanMotorStatus(bool *Led);  // 10 LED status flags (see HTMotor.h enum)
        void ScanMotorStatus(bool[] Led);
    }
}
