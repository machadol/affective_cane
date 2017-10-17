#ifndef FSR_H
#define FSR_H

class FSR_manager
{
	public:
		FSR_manager(int analogport);
		bool FSR_is_pressed();
		int  FSR_current_value();
	private:
		int FSR_port;
		int kernel_FSR(int analog);
		bool state_of_button();
		const static int button_level = 2;
};
#endif