//@BridgetACasey

#include "Mandelbrot.h"

int main()
{
	cout << "Please wait..." << endl;

	// Start timing
	the_clock::time_point start = the_clock::now();

	// This shows the whole set.

	mandelbrot mandelbrot = { -2.0, 1.0, 1.125, -1.125 };

	thread tred[16];

	for (int i = 0; i < 16; ++i)
	{
		tred[i] = thread(compute_thread, &mandelbrot, 0, WIDTH, HEIGHT * (i / 16.0f), HEIGHT * (1.0f / 16.0f));

		if (tred[i].joinable())
			tred[i].join();
	}

	// This zooms in on an interesting bit of detail.
	//compute_mandelbrot(-0.751085, -0.734975, 0.118378, 0.134488);

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();
	cout << "Computing the Mandelbrot set took " << time_taken << " ms." << endl;

	write_tga("moreslices.tga");

	return 0;
}