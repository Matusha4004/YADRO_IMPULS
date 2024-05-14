#include <iostream>
#include <array>
#include <string>
#include <queue>
#include <vector>
#include <fstream>

const uint8_t CONST_CONVERT_ASCII_TO_INT = 48;

const uint16_t CONST_MAX_INT_TIME_IN_DAY = 1439;

int stringTimeToInt(std::string string_time) {

	// string format xx:yy 
	//               01234
	// convert time to one int, for easier math
	if (string_time.size() != 5 || string_time[2] != ':') return -1;
	return (((((int)string_time[0] - CONST_CONVERT_ASCII_TO_INT) * 10 + ((int)string_time[1] - CONST_CONVERT_ASCII_TO_INT))) * 60) + ((((int)string_time[3] - CONST_CONVERT_ASCII_TO_INT) * 10 + ((int)string_time[4] - CONST_CONVERT_ASCII_TO_INT)));

}


std::string intTimeToString(uint16_t int_time) {

	std::string string_time;

	string_time.resize(5);

	char small_time_int = (char)(CONST_CONVERT_ASCII_TO_INT + int_time/600);

	string_time[0] += small_time_int;

	small_time_int = (char)(CONST_CONVERT_ASCII_TO_INT + ((int_time / 60) % 10));

	string_time[1] = small_time_int;

	string_time[2] = ':';

	small_time_int = (char)(CONST_CONVERT_ASCII_TO_INT + (int_time % 60 / 10));

	string_time[3] = small_time_int;

	string_time[4] = (char)(CONST_CONVERT_ASCII_TO_INT + int_time % 10);

	return string_time;

	// Convert int to String for output
}

struct table {

	uint16_t start_time_ = 0;

	uint32_t money_per_day_ = 0;

	uint16_t time_per_day_ = 0;

	bool is_using_ = false;

};


struct visitor {

	std::string name_ = "";

	table* place_on_table_ = nullptr;


	visitor(std::string name) {
		name_ = name;
	}

	visitor() {

	}

};

int find(std::string name, std::vector<visitor> q) {

	for(int i = 0; i < q.size(); ++i) {

		if(q[i].name_ == name) return i;

	}

	return -1;

}

class clubLogger {
private:

	uint16_t time_of_openning_ = 0;

	uint16_t time_of_closing_ = 0;

	uint16_t quantity_of_tables_ = 0;

	uint16_t cost_per_hour_ = 0;

	std::unique_ptr<table[]> tables_;

	std::vector<visitor> visitors_;

	std::queue<visitor*> queue_visitors_;

public:

	clubLogger(uint16_t time_to_open, uint16_t time_to_close, uint16_t quanity_of_tables, uint16_t cost_per_hour) {


		time_of_openning_ = time_to_open;

		time_of_closing_ = time_to_close;

		quantity_of_tables_ = quanity_of_tables;

		cost_per_hour_ = cost_per_hour;

		tables_ = std::make_unique<table[]>(quantity_of_tables_+1);

	}
	
	int mainStack(uint16_t int_time, uint8_t ID, std::string visitors_name, uint16_t number_of_table = 0, std::string error = "") {

		if(CONST_MAX_INT_TIME_IN_DAY > 1439) {
			return 1;
		}

		if(number_of_table > quantity_of_tables_) {
			return 1;
		}



		switch(ID) {

		case(1): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << '\n';
			event_ID_1(int_time,visitors_name);
			return 0;

		}

		case(2): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << " " << number_of_table << '\n';
			event_ID_2(int_time, visitors_name, number_of_table);
			return 0;

		}

		case(3): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << '\n';
			event_ID_3(int_time, visitors_name);
			return 0;

		}
		case(4): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << '\n';
			event_ID_4(int_time, visitors_name);
			return 0;

		}
		case(11): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << '\n';
			event_ID_11(int_time, visitors_name);
			return 0;
		}
		case(12): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << visitors_name << " " << number_of_table << '\n';
			event_ID_12(int_time, visitors_name, number_of_table);
			return 0;

		}
		case(13): {

			std::cout << intTimeToString(int_time) << " " << (int)ID << " " << error << '\n';
			return 0;
		}
		default:
			return 1;

		}


	}

	void event_ID_1(uint16_t int_time, std::string visitor_name) {

		if(int_time < time_of_openning_ || int_time > time_of_closing_) {
			mainStack(int_time, 13, visitor_name, 0, "NotOpenYet");
			return;
		}

		if(find(visitor_name, visitors_) != -1) {
			mainStack(int_time, 13, visitor_name, 0, "YouShallNotPass");
			return;
		}

		visitors_.push_back(visitor_name);

	}

	void event_ID_2(uint16_t int_time, std::string visitor_name, uint16_t number_of_table) {

		if(tables_[number_of_table].is_using_) {

			mainStack(int_time, 13, visitor_name, 0, "PlaceIsBusy");
			return;
		} 

		if(find(visitor_name, visitors_) == -1) {
			
			mainStack(int_time, 13, visitor_name, 0, "ClientUnknown");
			return;

		} 
		
		if(visitors_[find(visitor_name, visitors_)].place_on_table_ == nullptr) {

			visitors_[find(visitor_name, visitors_)].place_on_table_ = &tables_[number_of_table];

			tables_[number_of_table].is_using_ = true;

			tables_[number_of_table].start_time_ = int_time;

		} else {

			visitor& wotk_with = visitors_[find(visitor_name, visitors_)];

			wotk_with.place_on_table_->is_using_ = false; 

			wotk_with.place_on_table_->time_per_day_ += (int_time - wotk_with.place_on_table_->start_time_);

			wotk_with.place_on_table_->money_per_day_ += (int_time - wotk_with.place_on_table_->start_time_)/60;

			if((int_time - wotk_with.place_on_table_->start_time_) % 60 != 0) {

				wotk_with.place_on_table_->money_per_day_++;

			}

			wotk_with.place_on_table_ = &tables_[number_of_table];

			tables_[number_of_table].is_using_ = true;

			tables_[number_of_table].start_time_ = int_time;

		}

	}

	void event_ID_3(uint16_t int_time, std::string visitor_name) {

		for(int i = 1; i < quantity_of_tables_; ++i) {

			if(!tables_[i].is_using_) {
				mainStack(int_time, 13, visitor_name, 0, "ICanWaitNoLonger!");
				return;
			}
			
		}

		if(queue_visitors_.size() >= quantity_of_tables_) {
			mainStack(int_time, 11, visitor_name);
			return;
		}

		queue_visitors_.push(&visitors_[find(visitor_name,visitors_)]);

	}

	void event_ID_4(uint16_t int_time, std::string visitor_name) {

		if(find(visitor_name, visitors_) == -1) {

			mainStack(int_time, 13, visitor_name, 0, "ClientUnknown");
			return;

		}

		if(visitors_[find(visitor_name, visitors_)].place_on_table_ != nullptr) {

			visitor& wotk_with = visitors_[find(visitor_name, visitors_)];

			wotk_with.place_on_table_->is_using_ = false;

			wotk_with.place_on_table_->time_per_day_ += (int_time - wotk_with.place_on_table_->start_time_);

			wotk_with.place_on_table_->money_per_day_ += ((int_time - wotk_with.place_on_table_->start_time_) / 60);

			if(int_time - wotk_with.place_on_table_->start_time_ % 60 != 0) {
				wotk_with.place_on_table_->money_per_day_++;
			}

			

			if(!queue_visitors_.empty()) {

				int i = 0;

				while(wotk_with.place_on_table_ != &tables_[i]) {
					i++;
				}

				mainStack(int_time, 12, queue_visitors_.front()->name_, i);

				queue_visitors_.pop();

			}

		}

		visitors_.erase(visitors_.begin() + find(visitor_name, visitors_));

	}

	void event_ID_11(uint16_t int_time, std::string visitor_name) {

		if(visitors_[find(visitor_name, visitors_)].place_on_table_ != nullptr) {

			visitor& wotk_with = visitors_[find(visitor_name, visitors_)];

			wotk_with.place_on_table_->is_using_ = false;

			wotk_with.place_on_table_->time_per_day_ += (int_time - wotk_with.place_on_table_->start_time_);

			wotk_with.place_on_table_->money_per_day_ += (int_time - wotk_with.place_on_table_->start_time_) / 60;

			if(int_time - wotk_with.place_on_table_->start_time_ % 60 != 0) {
				wotk_with.place_on_table_->money_per_day_++;
			}


			if(!queue_visitors_.empty()) {

				int i = 0;

				while(wotk_with.place_on_table_ != &tables_[i]) {
					i++;
				}

				mainStack(int_time,12,queue_visitors_.front()->name_,i);
				queue_visitors_.pop();

			}
			
		}

		

		visitors_.erase(visitors_.begin() + find(visitor_name, visitors_));

		
		
	}

	void event_ID_12(uint16_t int_time, std::string visitor_name, uint16_t number_of_table) {

		visitors_[find(visitor_name, visitors_)].place_on_table_ = &tables_[number_of_table];

		tables_[number_of_table].is_using_ = true;
		tables_[number_of_table].start_time_ = int_time;

	}

	void event_end_of_day() {

		
		for(int i = visitors_.size(); i > 0; i--){

			mainStack(time_of_closing_,11,visitors_[0].name_);

		}
			

		std::cout<<intTimeToString(time_of_closing_) << '\n';

		for(int i = 1; i <= quantity_of_tables_; ++i){

			std::cout << i << " " << tables_[i].money_per_day_ * cost_per_hour_ << " " << intTimeToString(tables_[i].time_per_day_) << '\n';

		}

	}

};


int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		std::cout << "Error: Wrong number of arguments";
		return 1;
	}



	std::ifstream input_file(argv[1]);

	if(!input_file.is_open()) {
		std::cout << "Error: file is not opened";
		return 1;
	}

	int tables_;

	input_file>>tables_;

	std::string first_time, last_time;

	input_file>>first_time>>last_time;

	int money_per_hour;

	input_file >> money_per_hour;
	
	if(stringTimeToInt(first_time) < 0 || stringTimeToInt(last_time) < 0 || money_per_hour < 0 || tables_ < 0) {
		std::cout << "Error: Wrong input info from file";
		return 1;
	}

	clubLogger club_logger_in_main(stringTimeToInt(first_time),stringTimeToInt(last_time),tables_,money_per_hour);


	std::cout << first_time << '\n';

	std::string prev_time = "00:00";

	while(!input_file.eof()) {
		
		std::string time;
		int ID;
		std::string client_name;
		int table;

		input_file>>time>>ID>>client_name;

		if(ID == 2) {

			input_file>>table;

			if(stringTimeToInt(time) == -1) {

				std::cout << "Error: Wrong input info from file";
				return 1;

			} else {

				if(stringTimeToInt(time) < stringTimeToInt(prev_time)) {

					std::cout << "Error: Wrong input info from file";
					return 1;

				}
				prev_time = time;
			}

			if(club_logger_in_main.mainStack(stringTimeToInt(time), ID, client_name, table) == 1) {

				std::cout<<"Error: Wrong input info from file";
				return 1;

			}

		} else {

			if(stringTimeToInt(time) == -1) {

				std::cout << "Error: Wrong input info from file";
				return 1;

			} else {

				if(stringTimeToInt(time) < stringTimeToInt(prev_time)) {
					std::cout << "Error: Wrong input info from file";
					return 1;
				}
				prev_time = time;

			}

			if(club_logger_in_main.mainStack(stringTimeToInt(time), ID, client_name) == 1) {
				std::cout << "Error: Wrong input info from file";
				return 1;
			}
			
		}
	}
	
	club_logger_in_main.event_end_of_day();


}
