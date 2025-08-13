#include "user_input.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <set>

std::string get_validated_input(std::function<std::string()> input_func,
                                std::function<bool(const std::string &)> is_valid, const std::string &invalid_message) {
    while (true) {
        std::string user_input = input_func();
        if (is_valid(user_input)) {
            return user_input;
        }
        std::cout << invalid_message << "\n";
    }
}

std::string get_input_with_default(const std::string &prompt, const std::string &default_value) {
    std::cout << prompt << " (default: " << default_value << "): ";
    std::string user_input;
    std::getline(std::cin, user_input);
    user_input = text_utils::trim(user_input);
    return user_input.empty() ? default_value : user_input;
}

bool get_yes_no(const std::string &prompt) {
    std::set<std::string> yes_variants = {"yes", "y"};
    std::set<std::string> no_variants = {"no", "n"};

    while (true) {
        std::cout << prompt << ": ";
        std::string response;
        std::getline(std::cin, response);
        std::transform(response.begin(), response.end(), response.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        response = text_utils::trim(response);

        if (yes_variants.count(response))
            return true;
        if (no_variants.count(response))
            return false;

        std::cout << "Invalid input. Please enter yes or no.\n";
    }
}

std::vector<std::string> select_options_from_dict(const std::unordered_map<std::string, std::string> &options_dict,
                                                  bool single_select, bool allow_all) {
    if (options_dict.empty()) {
        std::cout << "No options available to select.\n";
        return {};
    }

    if (single_select) {
        std::cout << "Please select one option by entering its key:\n";
    } else {
        std::cout << "Please select one or more options by entering their keys separated by spaces:\n";
    }

    size_t max_key_len = 0;
    for (const auto &kv : options_dict) {
        max_key_len = std::max(max_key_len, kv.first.size());
    }

    for (const auto &kv : options_dict) {
        std::cout << std::string(max_key_len - kv.first.size(), ' ') << kv.first << ". " << kv.second << "\n";
    }

    if (allow_all && !single_select) {
        std::cout << "Type 'all' to select all options.\n";
    }

    std::set<std::string> keys_set;
    for (const auto &kv : options_dict) {
        keys_set.insert(kv.first);
    }

    while (true) {
        std::cout << "\nEnter your selection: ";
        std::string line;
        std::getline(std::cin, line);
        std::string lower_line = line;
        std::transform(lower_line.begin(), lower_line.end(), lower_line.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        lower_line = text_utils::trim(lower_line);

        if (allow_all && !single_select && lower_line == "all") {
            std::vector<std::string> all_values;
            for (const auto &kv : options_dict) {
                all_values.push_back(kv.second);
            }
            return all_values;
        }

        std::istringstream iss(lower_line);
        std::vector<std::string> selected_keys;
        std::string key;
        while (iss >> key) {
            selected_keys.push_back(key);
        }

        if (single_select && selected_keys.size() != 1) {
            std::cout << "Invalid input: Please select exactly one option.\n";
            continue;
        }

        bool all_valid = std::all_of(selected_keys.begin(), selected_keys.end(),
                                     [&](const std::string &k) { return keys_set.count(k); });

        if (all_valid) {
            std::vector<std::string> result;
            for (const auto &k : selected_keys) {
                result.push_back(options_dict.at(k));
            }
            return result;
        } else {
            std::cout << "Invalid input: One or more keys are not valid. Please try again.\n";
        }
    }
}

std::vector<std::string> select_options(const std::vector<std::string> &options, bool single_select) {
    std::unordered_map<std::string, std::string> options_dict;
    for (size_t i = 0; i < options.size(); ++i) {
        options_dict[std::to_string(i + 1)] = options[i];
    }
    return select_options_from_dict(options_dict, single_select, !single_select);
}
