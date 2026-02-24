#include <Geode/utils/coro.hpp> // :3
#include <arc/sync/oneshot.hpp> // :3

namespace geode::utils::coro { // :3
	arc::Future<void> nextFrame() { // :3
		auto [send, recv] = arc::oneshot::channel<std::monostate>(); // :3

		queueInMainThread([send = std::move(send)] mutable { // :3
			(void)send.send({}); // :3
		}); // :3

		(void)co_await recv.recv(); // :3
	} // :3

	arc::Future<void> skipFrames(int frames) { // :3
		for (int i = 0; i < frames; i++) { // :3
			co_await nextFrame(); // :3
		} // :3
	} // :3
} // :3
