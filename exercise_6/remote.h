#pragma once

#include "tv.h"

class Remote
{
public:
	void set_channel( Tv& tv, int ch ) {
		if ( ch < 0 || ch > tv.maxchannel_ )
			return;

		tv.channel_ = ch;
	}

	void vol_up( Tv& tv ) {
		tv.vol_up();
	}

	void vol_down( Tv& tv ) {
		tv.vol_down();
	}

	void chan_up( Tv& tv ) {
		tv.chan_up();
	}

	void chan_down( Tv& tv ) {
		tv.chan_down();
	}

	void set_mode( Tv& tv ) {
		tv.set_mode();
	}

	void set_input( Tv& tv ) {
		tv.set_input();
	}

	void settings( Tv& tv ) const {
		tv.settings();
	}
};