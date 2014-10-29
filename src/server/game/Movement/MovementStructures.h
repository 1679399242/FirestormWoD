/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MOVEMENT_STRUCTURES_H
#define _MOVEMENT_STRUCTURES_H

enum MovementStatusElements
{
    MSEHasTransportData,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEHasFallData,
    MSEHasFallDirection,
    MSEHasSpline,

    MSEGuid,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSETransportTime2,
    MSETransportTime3,
    MSEPitch,
    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,
    MSESplineElevation,
    MSEAlive32,
    MSEUnkCounter,
    MSEUnkCounterLoop,

    // Special
    MSEFlushBits,   //FlushBits()
    MSEZeroBit,     // writes bit value 1 or skips read bit
    MSEOneBit,      // writes bit value 0 or skips read bit
    MSEEnd,         // marks end of parsing
    MSE_COUNT
};

/// 6.0.1 18716
MovementStatusElements MovementGlobalSequence[] =
{
    MSEGuid,
    MSETimestamp,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSEPitch,
    MSESplineElevation,
    MSEUnkCounter,
    MSEAlive32,
    MSEUnkCounterLoop,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSEHasTransportData,
    MSEHasFallData,
    MSEZeroBit,
    MSEZeroBit,
    MSEZeroBit,

    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSETransportTime2,
    MSETransportTime3,

    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEHasFallDirection,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,

    MSEEnd,
};

/// 6.0.1 18716
MovementStatusElements PlayerMoveSequence[] =
{
    MSEGuid,
    MSEAlive32,
    MSEPositionX,
    MSEPositionY,
    MSEPositionZ,
    MSEOrientation,
    MSEPitch,
    MSESplineElevation,
    MSEUnkCounter,
    MSETimestamp,
    MSEMovementFlags,
    MSEMovementFlags2,
    MSEHasTransportData,
    MSEHasFallData,
    MSEZeroBit,
    MSEZeroBit,
    MSEZeroBit,
    MSEFlushBits,

    MSETransportGuid,
    MSETransportPositionX,
    MSETransportPositionY,
    MSETransportPositionZ,
    MSETransportOrientation,
    MSETransportSeat,
    MSETransportTime,
    MSEHasTransportTime2,
    MSEHasTransportTime3,
    MSEFlushBits,
    MSETransportTime2,
    MSETransportTime3,

    MSEFallTime,
    MSEFallVerticalSpeed,
    MSEHasFallDirection, 
    MSEFlushBits,
    MSEFallCosAngle,
    MSEFallSinAngle,
    MSEFallHorizontalSpeed,

    MSEEnd,
};

MovementStatusElements* GetMovementStatusElementsSequence(Opcodes opcode)
{
    // Cast spell has movement data part when castflags & 0x10, patched ClientSide to have same data of CMSG_PLAYER_MOVE
    if (opcode == CMSG_CAST_SPELL

        || opcode == CMSG_MOVE_START_FORWARD
        || opcode == CMSG_MOVE_START_TURN_LEFT
        || opcode == CMSG_MOVE_START_TURN_RIGHT
        || opcode == CMSG_MOVE_START_BACKWARD
        || opcode == CMSG_MOVE_START_STRAFE_LEFT
        || opcode == CMSG_MOVE_START_STRAFE_RIGHT
        || opcode == CMSG_MOVE_START_ASCEND
        || opcode == CMSG_MOVE_START_DESCEND
        || opcode == CMSG_MOVE_START_PITCH_UP
        || opcode == CMSG_MOVE_START_PITCH_DOWN
        || opcode == CMSG_MOVE_START_SWIM

        || opcode == CMSG_MOVE_JUMP
        || opcode == CMSG_MOVE_FALL_LAND

        || opcode == CMSG_MOVE_HEARTBEAT
        || opcode == CMSG_MOVE_SET_FACING
        || opcode == CMSG_MOVE_SET_PITCH
        || opcode == CMSG_MOVE_SET_RUN_MODE
        || opcode == CMSG_MOVE_SET_WALK_MODE
        || opcode == CMSG_MOVE_CHNG_TRANSPORT

        || opcode == CMSG_MOVE_STOP
        || opcode == CMSG_MOVE_STOP_TURN
        || opcode == CMSG_MOVE_STOP_STRAFE
        || opcode == CMSG_MOVE_STOP_SWIM
        || opcode == CMSG_MOVE_STOP_ASCEND
        || opcode == CMSG_MOVE_STOP_PITCH

        || opcode == CMSG_MOVE_SET_RUN_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_RUN_BACK_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_WALK_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_SWIM_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_FLIGHT_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_FLIGHT_BACK_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_TURN_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_PITCH_SPEED_CHEAT
        || opcode == CMSG_MOVE_SET_CAN_FLY_ACK
        || opcode == CMSG_MOVE_FEATHER_FALL_ACK
        || opcode == CMSG_MOVE_WATER_WALK_ACK
        || opcode == CMSG_MOVE_HOVER_ACK
        || opcode == CMSG_MOVE_SPLINE_DONE)
        return MovementGlobalSequence;

    if (opcode == SMSG_MOVE_UPDATE
        || opcode ==  SMSG_MOVE_UPDATE_TELEPORT
        || opcode ==  SMSG_MOVE_UPDATE_RUN_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_FLIGHT_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_PITCH_RATE
        || opcode ==  SMSG_MOVE_UPDATE_RUN_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_SWIM_BACK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_SWIM_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_TURN_RATE
        || opcode ==  SMSG_MOVE_UPDATE_WALK_SPEED
        || opcode ==  SMSG_MOVE_UPDATE_KNOCK_BACK)
        return PlayerMoveSequence;

    return NULL;
}

#endif
