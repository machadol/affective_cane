#ifndef FSR_H
#define FSR_H


class FSRManager
{
	public:
		FSRManager(int analogport);
		bool IsPressed();
		int  CurrentValue();
	private:
		int port;
		int Kernel(int analog);
		bool StateOfButton();
		const static int buttonLevel = 2;
};
#endif