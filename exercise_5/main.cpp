#include <iostream>
#include <string>

class Game_Character 
{
public:
	Game_Character( std::string name_ ) 
		: name( name_ ), alive{ true }, position{ 0 }
	{}

	virtual ~Game_Character() = default;

	bool is_alive() const { return alive; }
	int get_position() const { return position; }
	std::string get_name() const { return name; }

	void kill() { alive = false; }
	void move( int dt ) { position += dt; }

	virtual void update() = 0;
	virtual void display_status() const = 0;

private:
	bool alive;
	int position;
	std::string name;
};

class Archer final : public Game_Character
{
public:
	Archer( int arrows_, std::string name = "Legolas" ) 
		: Game_Character{ name }, arrows( arrows_), shots{ 0 }
	{}

	int get_arrows() const { return arrows; }
	void add_arrows( int count ) { arrows += count; }

	void update() override {
		if( arrows > 0 && is_alive() ) {
			shots++;
			arrows--;
		}
	}

	void display_status() const override {
		if( is_alive() ) {
			std::cout << " Archer " << get_name() << " shot " << shots;
			std::cout << " arrows and ";
			if( arrows > 0 ) {
				std::cout << "has " << arrows << " arrows." << std::endl;
			} else {
				std::cout << "doesn't have any arrows left!" << std::endl;
			}
		} else {
			std::cout << " Archer " << get_name() << " is dead :(." << std::endl;
		}
	}

private:
	int arrows;
	int shots;
};

class Horse final : public Game_Character
{
public:
	Horse( int speed_ = 10, std::string name_ = "Bullseye" ) 
		: Game_Character{ name_ }, speed{ speed_ }
	{}

	int get_speed() const { return speed; }
	void set_speed( int new_speed ) { speed = new_speed; }

	void update() override {
		if( is_alive() ) {
			move( speed );
		}
	}

	void display_status() const override {
		if( is_alive() ) {
			std::cout << " Horse " << get_name() << " is moving at speed " << speed;
			std::cout << " and is at position " << get_position() << std::endl;
		} else {
			std::cout << " Horse " << get_name() << " is dead :(" << std::endl;
		}
	}

private:
	int speed;
};

int main()
{
	Game_Character* characters[2];

	characters[0] = new Archer{5};
	characters[1] = new Horse{};

	for( int i = 0; i < 7; i++ ) {
		std::cout << "Iteration " << i << ":" << std::endl;
		for( Game_Character* ch : characters ) {
			ch->update();
			ch->display_status();
		}
		std::cout << "=======================" << std::endl;
	}

	delete characters[0];
	delete characters[1];
}