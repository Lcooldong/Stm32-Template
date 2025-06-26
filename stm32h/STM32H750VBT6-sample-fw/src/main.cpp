#include "main.h"
#include "def.h"

int main(void)
{
	bspInit();

	hwInit();	
  apInit();
	apMain();
	
	return 0;
}
