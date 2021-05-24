#pragma once
class Tv 
{ 
	friend class Remote;

public: 
	enum {Off, On}; 
	enum {MinVal, MaxVal = 20}; 
	enum {Antenna, Cable}; 
	enum {TV, DVD}; 

	Tv(int s = Off, int mc = 125) : state_(s), 
		volume_(5), maxchannel_(mc), channel_(2), 
		mode_(Cable), input_(TV) 
	{} 

	void on_off(); 
	bool is_on() const; 
	bool vol_up(); 
	bool vol_down(); 
	void chan_up(); 
	void chan_down(); 
	void set_mode(); 
	void set_input(); 
	void settings() const; 

private: 
	int state_; // on or off 
	int volume_; // assumed to be digitized 
	int maxchannel_; // maximum number of channels 
	int channel_; // current channel setting 
	int mode_; // antenna or cable 
	int input_; // TV or DVD 
}; 
