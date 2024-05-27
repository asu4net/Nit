#include "NitPCH.h"
#include "Core\Engine.h"
#include "WiimoteInputSystem.h"
#include "wiiuse.h"
#include "Input/KeyCodes.h"
#include "InputSystem.h"
#include <thread>

#define MAX_WIIMOTES            1
#define MAX_NUM_WIIMOTE_BUTTONS 14
#define MAX_IR_DOTS             4

namespace Nit::InputSystem::Wiimote
{
    struct ControllerState
    {
        /** Last frame's button states, so we only send events on edges */
        bool ButtonStates[MAX_NUM_WIIMOTE_BUTTONS];

        /** Next time a repeat event should be generated for each button */
        double NextRepeatTime[MAX_NUM_WIIMOTE_BUTTONS];
        /** Raw Left thumb x analog value */
        short LeftXAnalog;

        /** Raw left thumb y analog value */
        short LeftYAnalog;

        /** Id of the controller */
        int ControllerId;
        float TimeBetweenUpdatesIR = 3.f;

        // Values used when one of the IR points is lost
        float LastIRDistance;
        Vector2 LastIRDirection;
        float LastAngle;
        bool bIRWrongOrder = false;

        wiimote_t* Wiimote;

        /* Accelerometer */
        /** IR */
        /** MotionPlus */
        /** NunChuk */
    };

    /** Controller states */
    ControllerState controllerStates[MAX_WIIMOTES];
    wiimote_t** wiimotes;

    const String Buttons[MAX_NUM_WIIMOTE_BUTTONS]
    {
        WiimoteKeyNames::Wiimote_Button_A,
        WiimoteKeyNames::Wiimote_Button_B,
        WiimoteKeyNames::Wiimote_Button_1,
        WiimoteKeyNames::Wiimote_Button_2,
        WiimoteKeyNames::Wiimote_Plus,
        WiimoteKeyNames::Wiimote_Minus,
        WiimoteKeyNames::Wiimote_DPad_Up,
        WiimoteKeyNames::Wiimote_DPad_Down,
        WiimoteKeyNames::Wiimote_DPad_Left,
        WiimoteKeyNames::Wiimote_DPad_Right,
        WiimoteKeyNames::Wiimote_Home,
        WiimoteKeyNames::Nunchuck_Button_C,
        WiimoteKeyNames::Nunchuck_Button_Z,
        WiimoteKeyNames::Wiimote_PointerVisible
    };

    bool CurrentStates[MAX_NUM_WIIMOTE_BUTTONS];

    const float InitialButtonRepeatDelay = 0.2f;
    const float ButtonRepeatDelay = 0.1f;
    const float MaxTimeBetweenUpdatesIR = 4.f; 
    std::thread pollWiimotesThread;

    const float ForceFeedbackThreshold = 0.3f;

    bool LastForceFeedbackValues[MAX_WIIMOTES];

    float RepeatForceFeedbackTimer = 0.f;
    const float RepeatForceFeedbackTime = 4.f;

    void OnUpdate();
    void PollWiimotes();
    void OnDestroy();
    void HandleEvent(struct wiimote_t* _wiimote, int _id);
    void HandleCtrlStatus(struct wiimote_t* _wiimote, int _id);
    void HandleDisconnect(struct wiimote_t* _wiimote, int _id);
    void MotionEvents(struct wiimote_t* _wiimote, int _id);
    void IREvents(struct wiimote_t* _wiimote, int _id);
    bool CheckGravityIR(const Vector3& _gravity, int _directionX, int _directionY);
    void SetRumbleEnabled(int _controllerId, bool _bIsEnabled);

    void Register()
    {
        Engine::CreateSystem("WiimoteInputSystem", InputExecutionOrder, ExecutionContext::Always);
        Engine::SetSystemCallback(SystemStage::Update, &OnUpdate);
        Engine::SetSystemCallback(SystemStage::Destroy, &OnDestroy);

        wiimotes = wiiuse_init(MAX_WIIMOTES);
        ScanWiimotes();
    }

    void OnUpdate()
    {
        if (AnyWiimoteConnected() && !pollWiimotesThread.joinable())
        {
            pollWiimotesThread = std::thread(PollWiimotes);
        }
    }

    void PollWiimotes()
    {
        if (!wiiuse_poll(wiimotes, MAX_WIIMOTES)) 
        {
            pollWiimotesThread.detach();
            return;
        }
        /*
        *   This happens if something happened on any wiimote.
        *   So go through each one and check if anything happened.
        */
        const double CurrentTime = Time::GetApplicationTime();
        for (int i = 0; i < MAX_WIIMOTES; ++i)
        {
            ControllerState& controllerState = controllerStates[i];
            controllerState.Wiimote = wiimotes[i];
            if (controllerState.Wiimote && WIIMOTE_IS_CONNECTED(controllerState.Wiimote))
            {
                // Check Analog state
                if (controllerState.Wiimote->exp.type == EXP_NUNCHUK ||
                    controllerState.Wiimote->exp.type == EXP_MOTION_PLUS_NUNCHUK)
                {
                    /* nunchuk */
                    struct nunchuk_t* nc = (nunchuk_t*)&controllerState.Wiimote->exp.nunchuk;


                    InputSystem::OnControllerAnalog(WiimoteKeyNames::Nunchuck_StickX, controllerState.ControllerId, nc->js.x);
                    InputSystem::OnControllerAnalog(WiimoteKeyNames::Nunchuck_StickY, controllerState.ControllerId, nc->js.y);
                }
                else
                {
                    InputSystem::OnControllerAnalog(WiimoteKeyNames::Nunchuck_StickX, controllerState.ControllerId, 0.f);
                    InputSystem::OnControllerAnalog(WiimoteKeyNames::Nunchuck_StickY, controllerState.ControllerId, 0.f);
                }
            }

            memset(CurrentStates, 0, sizeof(CurrentStates));
            HandleEvent(controllerState.Wiimote, i);

            switch (controllerState.Wiimote->event)
            {
            case WIIUSE_STATUS:
                /* a status event occurred */
                HandleCtrlStatus(controllerState.Wiimote, i);
                break;

            case WIIUSE_DISCONNECT:
            case WIIUSE_UNEXPECTED_DISCONNECT:
                /* the wiimote disconnected */
                HandleDisconnect(controllerState.Wiimote, i);
                break;

            case WIIUSE_READ_DATA:
                /*
                *   Data we requested to read was returned.
                *   Take a look at ControllerState.Wiimote>read_req
                *   for the data.
                */
                break;

            case WIIUSE_NUNCHUK_INSERTED:
                /*
                *   a nunchuk was inserted
                *   This is a good place to set any nunchuk specific
                *   threshold values.  By default they are the same
                *   as the wiimote.
                */
                /* wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f); */
                /* wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100); */
                NIT_LOG("Nunchuk inserted.");
                break;

            case WIIUSE_MOTION_PLUS_ACTIVATED:
                NIT_LOG("Motion+ was activated");
                break;

            case WIIUSE_NUNCHUK_REMOVED:
            case WIIUSE_MOTION_PLUS_REMOVED:
                /* some expansion was removed */
                HandleCtrlStatus(controllerState.Wiimote, i);
                NIT_LOG("An expansion was removed.");
                break;

            default:
                break;
            }

            // For each button check against the previous state and send the correct message if any
            for (int ButtonIndex = 0; ButtonIndex < MAX_NUM_WIIMOTE_BUTTONS; ++ButtonIndex)
            {
                if (CurrentStates[ButtonIndex] != controllerState.ButtonStates[ButtonIndex])
                {
                    if (CurrentStates[ButtonIndex])
                    {
                        InputSystem::OnControllerButtonPressed(Buttons[ButtonIndex], controllerState.ControllerId, false);
                    }
                    else
                    {
                        InputSystem::OnControllerButtonReleased(Buttons[ButtonIndex], controllerState.ControllerId, false);
                    }

                    if (CurrentStates[ButtonIndex] != 0)
                    {
                        // this button was pressed - set the button's NextRepeatTime to the InitialButtonRepeatDelay
                        controllerState.NextRepeatTime[ButtonIndex] = CurrentTime + InitialButtonRepeatDelay;
                    }
                }
                else if (CurrentStates[ButtonIndex] != 0 && controllerState.NextRepeatTime[ButtonIndex] <= CurrentTime)
                {
                    InputSystem::OnControllerButtonPressed(Buttons[ButtonIndex], controllerState.ControllerId, true);

                    // set the button's NextRepeatTime to the ButtonRepeatDelay
                    controllerState.NextRepeatTime[ButtonIndex] = CurrentTime + ButtonRepeatDelay;
                }

                // Update the state for next time
                controllerState.ButtonStates[ButtonIndex] = CurrentStates[ButtonIndex];
            }

        }
        pollWiimotesThread.detach();
    }
    
    void OnDestroy()
    {
        pollWiimotesThread.join();
        wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
    }

    void SetIREnabled(int _controllerId, bool _bIsEnabled)
    {
        if (wiimotes[_controllerId] == nullptr) return;
        if (_bIsEnabled)
        {
            wiiuse_set_ir(wiimotes[_controllerId], 1);
            wiiuse_set_aspect_ratio(wiimotes[_controllerId], aspect_t::WIIUSE_ASPECT_16_9);
            wiiuse_set_ir_position(wiimotes[_controllerId], ir_position_t::WIIUSE_IR_ABOVE);
            wiiuse_set_ir_sensitivity(wiimotes[_controllerId], 3);
        }
        else
        {
            wiiuse_set_ir(wiimotes[_controllerId], 0);
        }
    }

    void SetMotionSensingEnabled(int _controllerId, bool _bIsEnabled)
    {
        if (wiimotes[_controllerId] == nullptr) return;
        if (_bIsEnabled)
        {
            wiiuse_motion_sensing(wiimotes[_controllerId], 1);
        }
        else
        {
            wiiuse_motion_sensing(wiimotes[_controllerId], 0);
        }
    }

    void ScanWiimotes()
    {
        int found, connected;

        wiimotes = wiiuse_init(MAX_WIIMOTES);

        found = wiiuse_find(wiimotes, MAX_WIIMOTES, 1);
        if (!found)
        {
            NIT_LOG("No wiimotes found.");
            return;
        }

        connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
        if (connected)
        {
            NIT_LOG("Connected to %i wiimotes (of %i found).", connected, found);
        }
        else
        {
            NIT_LOG("Failed to connect to any wiimote.");
            return;
        }

        wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
        //wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
        //wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
        //wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

        SetIREnabled(0, true);
        SetMotionSensingEnabled(0, true);

        for (int i = 0; i < MAX_WIIMOTES; ++i)
        {
            controllerStates[i].ControllerId = i;
            controllerStates[i].Wiimote = wiimotes[i];
        }
    }

    bool AnyWiimoteConnected()
    {
        if (!wiimotes)
        {
            return false;
        }

        for (int i = 0; i < MAX_WIIMOTES; i++)
        {
            if (wiimotes[i] && WIIMOTE_IS_CONNECTED(wiimotes[i]))
            {
                return true;
            }
        }

        return false;
    }

    void HandleEvent(struct wiimote_t* _wiimote, int _id)
    {
        CurrentStates[0] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_A) || IS_HELD(_wiimote, WIIMOTE_BUTTON_A);
        CurrentStates[1] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_B) || IS_HELD(_wiimote, WIIMOTE_BUTTON_B);
        CurrentStates[2] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_ONE) || IS_HELD(_wiimote, WIIMOTE_BUTTON_ONE);
        CurrentStates[3] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_TWO) || IS_HELD(_wiimote, WIIMOTE_BUTTON_TWO);
        CurrentStates[4] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_PLUS) || IS_HELD(_wiimote, WIIMOTE_BUTTON_PLUS);
        CurrentStates[5] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_MINUS) || IS_HELD(_wiimote, WIIMOTE_BUTTON_MINUS);
        CurrentStates[6] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_UP) || IS_HELD(_wiimote, WIIMOTE_BUTTON_UP);
        CurrentStates[7] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_DOWN) || IS_HELD(_wiimote, WIIMOTE_BUTTON_DOWN);
        CurrentStates[8] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_LEFT) || IS_HELD(_wiimote, WIIMOTE_BUTTON_LEFT);
        CurrentStates[9] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_RIGHT) || IS_HELD(_wiimote, WIIMOTE_BUTTON_RIGHT);
        CurrentStates[10] = IS_PRESSED(_wiimote, WIIMOTE_BUTTON_HOME) || IS_HELD(_wiimote, WIIMOTE_BUTTON_HOME);

        /* show events specific to supported expansions */
        if (_wiimote->exp.type == EXP_NUNCHUK || _wiimote->exp.type == EXP_MOTION_PLUS_NUNCHUK)
        {
            /* nunchuk */
            struct nunchuk_t* nc = (nunchuk_t*)&_wiimote->exp.nunchuk;

            const float DEADZONE = 0.6f;

            CurrentStates[11] = IS_PRESSED(nc, NUNCHUK_BUTTON_C) || IS_HELD(_wiimote, NUNCHUK_BUTTON_C);
            CurrentStates[12] = IS_PRESSED(nc, NUNCHUK_BUTTON_Z) || IS_HELD(_wiimote, NUNCHUK_BUTTON_Z);

        }

        MotionEvents(_wiimote, _id);
        IREvents(_wiimote, _id);

        InputSystem::OnControllerAnalog(WiimoteKeyNames::Wiimote_BatteryLevel, _id, _wiimote->battery_level);
    }

    void HandleCtrlStatus(struct wiimote_t* _wiimote, int _id)
    {
        /*
        NIT_LOG("\n\n--- CONTROLLER STATUS [wiimote id %i] ---", _wiimote->unid);

        NIT_LOG("attachment:      %i", _wiimote->exp.type);
        NIT_LOG("speaker:         %i", WIIUSE_USING_SPEAKER(_wiimote));
        NIT_LOG("ir:              %i", WIIUSE_USING_IR(_wiimote));
        NIT_LOG("leds:            %i %i %i %i"), WIIUSE_IS_LED_SET(_wiimote, 1), WIIUSE_IS_LED_SET(_wiimote, 2), WIIUSE_IS_LED_SET(_wiimote, 3), WIIUSE_IS_LED_SET(_wiimote, 4));
        NIT_LOG("battery:         %f %%"), _wiimote->battery_level);
        */
    }

    void HandleDisconnect(struct wiimote_t* _wiimote, int _id)
    {
        //NIT_LOG("\n\n--- DISCONNECTED [wiimote id %i] ---", _wiimote->unid);
    }

    void MotionEvents(struct wiimote_t* _wiimote, int _id)
    {
        // Update motion controls.
        Vector3 Tilt(0, 0, 0);
        Vector3 RotationRate(0, 0, 0);
        Vector3 Gravity(0, 0, 0);
        Vector3 Acceleration(0, 0, 0);

        /* if the accelerometer is turned on then print angles */
        if (WIIUSE_USING_ACC(_wiimote))
        {
            Tilt.x = -_wiimote->orient.pitch;
            Tilt.y = _wiimote->orient.yaw;
            Tilt.z = _wiimote->orient.roll;

            Acceleration.x = _wiimote->accel.x;
            Acceleration.y = _wiimote->accel.y;
            Acceleration.z = _wiimote->accel.z;
        }

        if (_wiimote->exp.type == EXP_MOTION_PLUS || _wiimote->exp.type == EXP_MOTION_PLUS_NUNCHUK)
        {
            RotationRate.x = -_wiimote->exp.mp.angle_rate_gyro.pitch;
            RotationRate.y = _wiimote->exp.mp.angle_rate_gyro.yaw;
            RotationRate.z = _wiimote->exp.mp.angle_rate_gyro.roll;
        }

        Gravity.x = _wiimote->gforce.x;
        Gravity.y = _wiimote->gforce.y;
        Gravity.z = _wiimote->gforce.z;

        InputSystem::OnControllerVector3(WiimoteKeyNames::Wiimote_Tilt3D, _id, Tilt);
        InputSystem::OnControllerVector3(WiimoteKeyNames::Wiimote_RotationRate3D, _id, RotationRate);
        InputSystem::OnControllerVector3(WiimoteKeyNames::Wiimote_Gravity3D, _id, Gravity);
        InputSystem::OnControllerVector3(WiimoteKeyNames::Wiimote_Acceleration3D, _id, Acceleration);

    }

    void IREvents(struct wiimote_t* _wiimote, int _id)
    {
        if (!WIIUSE_USING_IR(_wiimote)) return;

        CurrentStates[13] = false;
        bool bBothVisible = true;
        Vector2	Pos = Vector2(static_cast<float>(_wiimote->ir.x), static_cast<float>(_wiimote->ir.y));
        /* go through each of the 2 possible IR sources */
        for (int i = 0; i < 2; ++i)
        {
            CurrentStates[13] = CurrentStates[13] || _wiimote->ir.dot[i].visible;
            bBothVisible = bBothVisible && _wiimote->ir.dot[i].visible;
        }

        if (bBothVisible)
        {
            int DirectionX = static_cast<int>(_wiimote->ir.dot[1].rx) - static_cast<int>(_wiimote->ir.dot[0].rx);
            int DirectionY = static_cast<int>(_wiimote->ir.dot[1].ry) - static_cast<int>(_wiimote->ir.dot[0].ry);

            controllerStates[_id].TimeBetweenUpdatesIR += Time::GetDeltaTime();
            if (controllerStates[_id].TimeBetweenUpdatesIR >= MaxTimeBetweenUpdatesIR)
            {
                controllerStates[_id].TimeBetweenUpdatesIR = 0.f;

                Vector3 Gravity = Vector3(_wiimote->gforce.x, _wiimote->gforce.y, _wiimote->gforce.z);
                controllerStates[_id].bIRWrongOrder = CheckGravityIR(Gravity, DirectionX, DirectionY);
            }

            Vector2 Direction = Vector2(static_cast<float>(DirectionX), static_cast<float>(DirectionY));
            Direction = controllerStates[_id].bIRWrongOrder ? Vector2::Zero - Direction : Direction;

            float Angle = Math::ToDegrees(std::acos(Vector2::Dot(Direction, Vector2(1, 0)) / (Direction.Magnitude()))) * Math::Sign(Direction.y);

            controllerStates[_id].LastAngle = Angle;
            controllerStates[_id].LastIRDistance = _wiimote->ir.z;
            controllerStates[_id].LastIRDirection = Direction;

        }
        else
        {
            controllerStates[_id].TimeBetweenUpdatesIR = MaxTimeBetweenUpdatesIR;

            float Angle = Math::ToDegrees(Math::Atan2(_wiimote->gforce.z, _wiimote->gforce.x)) - 90.f;

            controllerStates[_id].LastAngle = Angle;
        }

        InputSystem::OnControllerAnalog(WiimoteKeyNames::Wiimote_PointerDistance, _id, controllerStates[_id].LastIRDistance);
        InputSystem::OnControllerAnalog(WiimoteKeyNames::Wiimote_PointerAngle, _id, controllerStates[_id].LastAngle);

        Pos.x /= _wiimote->ir.vres[0];
        Pos.y /= _wiimote->ir.vres[1];

        InputSystem::OnControllerVector2(WiimoteKeyNames::Wiimote_Pointer2D, _id, Pos);
    }

    bool CheckGravityIR(const Vector3& _gravity, int _directionX, int _directionY)
    {
        return _gravity.z > 0.5f && _directionX < 0 ||
            _gravity.z < -0.5f && _directionX > 0 ||
            _gravity.x < -0.5f && _directionY < 0 ||
            _gravity.x >  0.5f && _directionY > 0;
    }

    void SendForceFeedback(int _controllerId, float _leftMotor, float _rightMotor)
    {
        bool bShouldRumble = Math::Max(_leftMotor, _rightMotor) > ForceFeedbackThreshold;
        if (bShouldRumble == LastForceFeedbackValues[_controllerId])
        {
            RepeatForceFeedbackTimer += Time::GetDeltaTime();
            if (RepeatForceFeedbackTimer < RepeatForceFeedbackTime) return;
        }

        RepeatForceFeedbackTimer = 0.f;
        LastForceFeedbackValues[_controllerId] = bShouldRumble;

        SetRumbleEnabled(_controllerId, bShouldRumble);
    }


    void SetRumbleEnabled(int _controllerId, bool _bIsEnabled)
    {
        if (wiimotes[_controllerId] == nullptr) return;

        wiiuse_rumble(wiimotes[_controllerId], _bIsEnabled);
    }
}
