#include "VectorMath.h"

int GiveRandomNumf(int min, int max, int sizeofarray, bool exclude_on_off, int numtoexclude) {

	static int timesthisfunctioncalled = 0;
	int* randarray = new int[sizeofarray];
	*randarray = NULL;
	int finalresult;

	srand((unsigned)time(NULL));
	if (min < max)
	{
		for (size_t i = 0; i < sizeofarray; i++)
		{
			randarray[i] = rand() % max;

			if (randarray[i] < min)
			{
				i--;
			}
			if (exclude_on_off == true)
			{
				if (randarray[i] == numtoexclude)
				{

					i--;

				}

			}

		}
	}

	timesthisfunctioncalled++;

	if (timesthisfunctioncalled > sizeofarray - 1)
	{
		timesthisfunctioncalled = (timesthisfunctioncalled % (sizeofarray - 1));
	}

	finalresult = randarray[timesthisfunctioncalled];
	delete[] randarray;
	return finalresult;

};
