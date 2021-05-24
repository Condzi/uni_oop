#include "tv.h"

#include <iostream>

void Tv::on_off()
{
	if( state_ == On )
		state_ = Off;
	else
		state_ = On;
}

bool Tv::is_on() const
{
	return state_ == On;
}

bool Tv::vol_up()
{
	if( volume_ == MaxVal )
		return false;

	volume_++;
	return true;
}

bool Tv::vol_down()
{
	if( volume_ == MinVal )
		return false;

	volume_--;
	return true;
}

void Tv::chan_up()
{
	if( channel_ == maxchannel_ )
		channel_ = 0;
	else
		channel_++;
}

void Tv::chan_down()
{
	if( channel_ == 0 )
		channel_ = maxchannel_;
	else
		channel_--;
}

void Tv::set_mode()
{
	if( mode_ == Antenna )
		mode_ = Cable;
	else
		state_ = Antenna;
}

void Tv::set_input()
{
	if( mode_ == DVD )
		mode_ = TV;
	else
		state_ = DVD;
}

void Tv::settings() const
{
	std::cout << " state_   = " << ((state_==On)?"On":"Off") << std::endl;
	std::cout << " volume_  = " << volume_ << std::endl;
	std::cout << " channel_ = " << channel_ << std::endl;
	std::cout << " mode_    = " << ((mode_==Antenna)?"Antenna":"Cable") << std::endl;
	std::cout << " input_   = " << ((input_==TV)?"TV":"DVD") << std::endl;
}
