#include "config.hpp"
#include <urlmon.h>    
#include <filesystem>
#include <fstream>
#include "../menu.hpp"
#include "../../utils/xor.hpp"
#pragma comment(lib, "Urlmon")

static std::string path = "C:/ozungaware/config/";

template <typename T>
static void read_value(const nlohmann::json& src, T& dest) {
	if (src.is_null()) return;

	dest = src.get<T>();
}

template <typename T>
static T read_value(const nlohmann::json& src) {
	if (src.is_null()) return T{};

	return src.get<T>();
}

template <typename T>
static void write_value(nlohmann::json& dest, const T& src) {
	dest = src;
}

namespace c {
	void create_directory() {
		directory = "C:/ozungaware";
		if (!std::filesystem::exists(directory))
			std::filesystem::create_directory(directory);

		directory_path = "C:/ozungaware/config";
		if (!std::filesystem::exists(directory_path))
			std::filesystem::create_directory(directory_path);
		else
			update_configs();
	}

	void update_configs() {
		configs.clear();

		for (const auto& entry : std::filesystem::directory_iterator{ directory_path }) {
			if (!entry.is_regular_file() || entry.path().extension() != (".json")) continue;

			const auto& file_name = entry.path().stem().string();
			configs.emplace_back(file_name);
		}
	}

	void create_file(const std::string_view name) {
		if (name.empty() || name.find_first_not_of(' ') == std::string_view::npos) return;
		if (std::find(configs.cbegin(), configs.cend(), name) != configs.cend()) return;

		configs.emplace_back(name);
		save(configs.size() - 1);
	}

	void delete_file(const std::size_t index) {
		if (configs.size() <= index) return;

		path.append(configs.at(index));
		path.append((".json"));
		std::filesystem::remove(path);
		path.erase(path.size() - configs.at(index).size() - 5);

		configs.erase(configs.cbegin() + index);
		configs.shrink_to_fit();
		update_configs;
	}

	void save(const std::size_t index) {
		nlohmann::json json;

		path.append(configs.at(index));

		const bool has_json_in_filename = path.rfind((".json")) != std::string::npos;
		if (!has_json_in_filename) {
			path.append((".json"));
		}

		if (std::ofstream output_file{ path }; output_file.good()) {
			output_file << std::setw(4) << json;
		}

		path.erase(path.size() - configs.at(index).size() - (!has_json_in_filename ? 5 : 0));
	}

	void load(const std::size_t index) {
		path.append(configs.at(index));
		path.append((".json"));

		if (std::ifstream input_file{ path }; input_file.good()) {
			nlohmann::json json;
			input_file >> json;

		}

		path.erase(path.size() - configs.at(index).size() - 5);
	}
}