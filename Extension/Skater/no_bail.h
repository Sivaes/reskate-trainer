#pragma once
#include <cstdint>

namespace dingosdk {
bool start_no_bail(std::uintptr_t image_base) noexcept;
bool no_bail_available() noexcept;
// Publish from the validated local client tick. Returns owner availability even
// when both controls are off. Manual protection expires if ticks stop arriving.
bool update_no_bail(std::uintptr_t client, std::uintptr_t entity, bool manual,
    bool flying, std::uint64_t flight_expires) noexcept;
void clear_no_bail() noexcept;
// S.K.A.T.E.: while `locked`, the local skater cannot get back on the board once it is off
// (its mount request is dropped). Publish from the client tick; it expires if ticks stop.
// Releasing it also leaves the skater's teleport option on the board again, since a turn's
// teleport may have set it off (skater component +0xc0) and the SDK's own teleports keep it.
void update_board_lock(std::uintptr_t client, std::uintptr_t entity, bool locked) noexcept;
// The physics state the local skater's selector last chose, for the trainer (air time, bail
// markers). Publish the skater to watch from the client tick; it expires if ticks stop.
struct PhysicsStateWatch {
    bool valid{};
    std::uint32_t state{};
    std::uint64_t changes{}, wipeouts{}; // counted since the process started
};
void watch_physics_state(std::uintptr_t client, std::uintptr_t entity) noexcept;
PhysicsStateWatch watched_physics_state() noexcept;
// Dark Pop: while enabled and `held`, the moment the local skater's flight would end (the selector
// leaves the air states 200-299) it chooses the pop state (200) instead, once per second at most.
// Needs the No Bail lease to be active (it identifies the local skater). Client tick only.
// `held`: D-pad Right is down. `catching`: RB is down (the catch). With `require_catch` a pop also needs RB
// to have been held within the last 2.5 s. A pop is then kept going for 160 ms, because the game keeps
// asking to leave the air for a few ticks after the launch.
void dark_pop_update(bool enabled, bool held, bool catching, bool require_catch) noexcept;
struct DarkPopLast {
    std::uint64_t count{}; // pops forced since the process started
    std::uint64_t hold_ticks{}; // physics ticks the last pop was kept in the air
    std::uint32_t from{}, to{}; // the air state it left and the state the game wanted
};
DarkPopLast dark_pop_last() noexcept;
// Stopping flight must not discard the independent manual preference.
void clear_no_bail_flight() noexcept;
}
