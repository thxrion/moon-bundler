#include "main.hpp"

hook_shared_t<void(*)()> hk_game_loop;

bool hk_rpc(unsigned char &id, RakNet::BitStream *&&bs) {
	return true;
};

void game_loop() {
	hk_game_loop->call_original();

	static bool initialized = false;

	if (initialized || !rakhook::initialize()) {
		return;
	}
	rakhook::on_receive_rpc += hk_rpc;

	initialized = true;
}

class plugin {
public:
	plugin() {
		rakhook::samp_version version = rakhook::get_samp_version();
		if (rakhook::samp_addr() && version != rakhook::SAMP_UNKNOWN) {
			hk_game_loop = std::make_shared<hook_t<void(*)()>>(
				reinterpret_cast<void(*)()>(GAME_LOOP_ENTRY),
				game_loop
			);
		}
	
	}
} _plugin;