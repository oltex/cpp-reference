#pragma once

namespace example {
	inline void time(void) noexcept {
		//{
			//	timeBeginPeriod(1);

			//	unsigned long my_time = timeGetTime();
			//	double frame = 1000. / 30;
			//	unsigned long current_time = timeGetTime();
			//	unsigned long last_time = timeGetTime();

			//	for (;;) {
			//		current_time = timeGetTime();
			//		auto delta = current_time - last_time;
			//		last_time = current_time;
			//		my_time += frame;
			//		printf("%d\n", delta);

			//		if (my_time > current_time) {
			//			auto sleep = my_time - current_time;
			//			//printf("%d, %d, %d\n", sleep, my_time, current_time);
			//			Sleep(sleep);
			//		}
			//	}
			//}

		{
			timeBeginPeriod(1);

			qtime my_time;
			my_time.counter();
			double frame = 1. / 30.;
			qtime current_time;
			current_time.counter();
			qtime last_time;
			last_time.counter();

			for (;;) {
				current_time.counter();
				auto delta = current_time - last_time;
				last_time = current_time;
				my_time += frame;
				printf("%f\n", delta);

				if (my_time > current_time) {
					auto sleep = my_time - current_time;
					//printf("%d, %d, %d\n", sleep, my_time, current_time);
					Sleep(sleep * 1000.);
				}
			}
		}
	}
}