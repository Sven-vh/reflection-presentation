/* Comment any of these out to disable */
#define USE_TEMPLATE
#define USE_REFLECTION

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "extern/visit_struct.hpp"

/* Simple enum */
enum class Difficulty {
	Easy,
	Medium,
	Hard
};

/* Example struct */
struct GameSettings {
	int			volume = 50;
	float		sensitivity = 1.0f;
	bool		fullscreen = true;
	Difficulty	difficulty = Difficulty::Medium;
};
#ifdef USE_REFLECTION

/* Define member variables */
VISITABLE_STRUCT(GameSettings, volume, sensitivity, fullscreen, difficulty);

#endif

namespace Logger {
#ifdef USE_TEMPLATE
#ifdef USE_REFLECTION


	/* Anything, except visitable struct */
	template<typename T>
	std::enable_if_t<visit_struct::traits::is_visitable<T>::value == false, void>
	Log(const char* name, const T& value) {
		std::cout << name << ": " << value << std::endl;
	}

#else
	template<typename T>
	void Log(const char* name, const T& value) {
		std::cout << name << ": " << value << std::endl;
	}
#endif
#else
	void Log(const char* name, const int& value) {
		std::cout << name << ": " << value << std::endl;
	}

	void Log(const char* name, const float& value) {
		std::cout << name << ": " << value << std::endl;
	}

	void Log(const char* name, const double& value) {
		std::cout << name << ": " << value << std::endl;
	}

	void Log(const char* name, const std::string& value) {
		std::cout << name << ": " << value << std::endl;
	}

	//... Add more overloads as needed
#endif

	/* Bool */
	void Log(const char* name, const bool& value) {
		std::cout << name << ": " << (value ? "True" : "False") << std::endl;
	}

	/* Vectors */
	template<typename T>
	void Log(const char* name, const std::vector<T>& vec) {
		std::cout << name << ": " << std::endl;
		for (int i = 0; i < vec.size(); ++i) {
			const auto& item = vec[i];
			const auto label = "Index " + std::to_string(i);
			Log(label.c_str(), item);
		}
	}

	/* Maps */
	template<typename K, typename V>
	void Log(const char* name, const std::map<K, V>& p) {
		std::cout << name << ": " << std::endl;
		for (const auto& [key, value] : p) {
			std::cout << "-----" << std::endl;
			Log("Key", key);
			Log("Value", value);
		}
	}

	/* Enum */
	/* Hardcoded right now for simplicity, I normally use https://github.com/Neargye/magic_enum */
	void Log(const char* name, const Difficulty& value) {
		std::cout << name << ": ";
		switch (value) {
		case Difficulty::Easy:   std::cout << "Easy"; break;
		case Difficulty::Medium: std::cout << "Medium"; break;
		case Difficulty::Hard:   std::cout << "Hard"; break;
		}
		std::cout << std::endl;
	}

#ifdef USE_REFLECTION
	/* Generic function for every visitable struct */
	/* Uses SFINAE to only enable this overload for visitable structs */

	template<typename T>
	std::enable_if_t<visit_struct::traits::is_visitable<T>::value, void>
		Log(const char* name, const T& obj) {
		std::cout << name << ": " << std::endl;
		visit_struct::for_each(obj, [](const char* name, const auto& value) {
			Log(name, value);
			});
	}

#else
	/* Game Settings, manual way*/
	void Log(const GameSettings& settings) {
		Log("Volume", settings.volume);
		Log("Sensitivity", settings.sensitivity);
		Log("Fullscreen", settings.fullscreen);
		Log("Difficulty", settings.difficulty);
	}
#endif

	/* Game Settings, reflection, visit_struct */
}

/* SFINAE examples */
namespace Logger {

	/* Only works on integrals, like int, bool, char, etc */
	template<typename T>
	std::enable_if_t<std::is_integral_v<T>, void> Log(const T& value) {
		std::cout << "Integer: " << value << std::endl;
	}

	/* Only works on floating point types, like float, double, long double */
	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>, void> Log(const T& value) {
		std::cout << "Float: " << value << std::endl;
	}
}

void newline() {
	std::cout << std::endl;
}

void distance() {
	std::cout << std::endl << "------------------------" << std::endl << std::endl;
}

int main() {

	/* Simple primitive types */
	{
		int my_int = 42;
		Logger::Log("My Int", my_int);

		float my_float = 3.14f;
		Logger::Log("My Float", my_float);

		bool my_bool = true;
		Logger::Log("My Bool", my_bool);

		newline();

		std::string my_string = "Hello, World!";
		Logger::Log("My String", my_string);
	}

	distance();

	/* STL containers */
	{
		std::vector<float> my_vector = { 1.5f, 2.0f, 3.0f };
		Logger::Log("My Vector", my_vector);

		newline();

		std::map<std::string, int> my_map = { {"One", 1}, {"Two", 2}, {"Three", 3} };
		Logger::Log("My Map", my_map);
	}

	distance();

	/* Enum */
	{
		Difficulty my_difficulty = Difficulty::Hard;
		Logger::Log("My Difficulty", my_difficulty);
	}

	distance();

	/* Structs */
	{
		GameSettings settings;
		Logger::Log("Settings", settings);
	}

	distance();

	/* SFINAE examples */
	{
		Logger::Log(42);

		newline();

		Logger::Log(3.14f);
	}

	//wait for user input before closing
	std::cin.get();

}
