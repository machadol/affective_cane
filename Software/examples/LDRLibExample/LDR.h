#ifndef LDR_H
#define LDR_H


class LDRManager
{
	public:
		LDRManager(int analogport);
		bool LightOn();
		int  CurrentValue();
	private:
		int port;
		int Kernel(int analog);
		bool StateOfLight();
		const static int lightLevel = 2;
};
#endif