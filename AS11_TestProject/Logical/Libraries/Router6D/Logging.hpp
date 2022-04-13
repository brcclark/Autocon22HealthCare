#ifndef __RL6D_LOGGING_H__
#define __RL6D_LOGGING_H__

#include <Router6D.h>
#include <string>
#include <cstring>

extern plcstring router_6d_log_name [];

bool log_routed_move_start(UINT shuttle, std::string destination);

bool log_direct_move_start(UINT shuttle, std::string destination);

bool log_destination_arrival(UINT shuttle, std::string destination);

bool log_destination_update(UINT shuttle, std::string destination);

bool log_path_start(UINT shuttle, std::string path);

bool log_waypoint_move(UINT shuttle, std::string waypoint);

bool log_asm_get_sh_start(void);

bool log_asm_get_sh_done(UINT shuttles);

bool log_asm_recovery_start(rl6dRecoverModeEnum mode);

bool log_asm_recovery_done(void);

bool log_asm_sh_assignment(UINT shuttle, std::string destination);

bool log_sh_error(UINT shuttle, DINT error);

bool log_asm_get_sh_error(DINT error);

bool log_move_in_plane_error(DINT error);

#endif

